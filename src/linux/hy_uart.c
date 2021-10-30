/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 09:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 09:13
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

#include "hy_uart.h"

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_error.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    HyUartSaveConfig_t  save_config;
    int32_t             fd;
} _uart_context_t;

ssize_t HyUartWrite(void *handle, const void *buf, size_t len)
{
    HY_ASSERT_VAL_RET_VAL(!handle || !buf, -1);
    _uart_context_t *context = handle;

    return write(context->fd, buf, len);
}

ssize_t HyUartRead(void *handle, void *buf, size_t len)
{
    HY_ASSERT_VAL_RET_VAL(!handle || !buf, -1);
    _uart_context_t *context = handle;

    return read(context->fd, buf, len);
}

static inline void _uart_destroy(_uart_context_t *context)
{
    close(context->fd);
}

static int32_t _uart_create(_uart_context_t *context, char *name)
{
    HyUartSaveConfig_t *save_config = &context->save_config;
    struct termios options;

    do {
        // O_NONBLOCK和O_NDELAY区别:
        // 它们的差别在于设立O_NDELAY会使I/O函式马上回传0，但是又衍生出一个问题，
        // 因为读取到档案结尾时所回传的也是0，这样无法得知是哪中情况；
        // 因此，O_NONBLOCK就产生出来，它在读取不到数据时会回传-1，并且设置errno为EAGAIN。
        int32_t flags = O_RDWR | O_NOCTTY;
        // int32_t flags = O_RDWR | O_NOCTTY | O_NONBLOCK;
        // int32_t flags = O_RDWR | O_NOCTTY | O_NDELAY;

        context->fd = open(name, flags);
        if (-1 == context->fd) {
            LOGE("open %s failed \n", name);
            break;
        }

        if (tcgetattr(context->fd, &options)) {
            LOGE("tcgetattr failed \n");
            break;
        }

        // 配置为原始模式 (两种方式任选其一)
#if 0
        cfmakeraw(&options);
#else
        options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP| INLCR | IGNCR | ICRNL | IXON);
        options.c_oflag &= ~OPOST;
        options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        options.c_cflag &= ~(CSIZE | PARENB);
        options.c_cflag |= CS8;
#endif

        // CLOCAL和CREAD分别用于本地连接和接收使能
        options.c_cflag |= (CLOCAL | CREAD); 

        // 设置波特率
        speed_t speed_2_speed[HY_UART_SPEED_MAX] = {B4800, B9600, B115200};
        cfsetispeed(&options, speed_2_speed[save_config->speed]);
        cfsetospeed(&options, speed_2_speed[save_config->speed]);

        // 设置数据位
        int32_t data_bit_2_bit[HY_UART_DATA_BIT_MAX] = {CS5, CS6, CS7, CS8};
        options.c_cflag &= ~CSIZE;  // 清零
        options.c_cflag |= data_bit_2_bit[save_config->data_bit];

        // 设置校验位
        switch (save_config->parity_type) {
            case HY_UART_PARITY_NONE:
                options.c_cflag &= ~PARENB;
                options.c_iflag &= ~INPCK;
                break;
            case HY_UART_PARITY_ODD:
                options.c_cflag |= (PARENB | PARODD);
                options.c_iflag |= INPCK;
                break;
            case HY_UART_PARITY_EVEN:
                options.c_cflag |= PARENB;
                options.c_cflag &= ~PARODD;
                options.c_iflag |= INPCK;
                break;
            case HY_UART_PARITY_MAX:
                LOGE("the parity_type is error \n");
                break;
        }

        // 设置停止位
        switch (save_config->stop_bit) {
            case HY_UART_STOP_BIT_1:    options.c_cflag &= ~CSTOPB;         break;
            case HY_UART_STOP_BIT_2:    options.c_cflag |= CSTOPB;          break;
            case HY_UART_STOP_BIT_MAX: LOGE("the stop_bit is error \n");    break;
        }

        // 设置数据流控
        switch (save_config->flow_control) {
            case HY_UART_FLOW_CONTROL_NONE:     options.c_cflag &= ~CRTSCTS;             break;
            case HY_UART_FLOW_CONTROL_HARDWARE: options.c_cflag |= CRTSCTS;              break;
            case HY_UART_FLOW_CONTROL_SOFT:     options.c_cflag |= IXON | IXOFF | IXANY; break;
            case HY_UART_FLOW_CONTROL_MAX: LOGE("the flow_control is error \n");     break;
        }

        // open的时候没有设置O_NONBLOCK或O_NDELAY，下面两个参数起效
        // 再接收到第一个字节后，开始计算超时时间
        // 接收到VMIN字节后，read直接返回
        // 在VTIME时间内，没有接收到VIMIN字节，read也直接返回
        options.c_cc[VTIME] = UART_READ_VTIME_100MS;    // 百毫秒, 是一个unsigned char变量
        options.c_cc[VMIN]  = UART_READ_VMIN_LEN;       // read at least 1 byte

        // TCIFLUSH刷清输入队列
        // TCOFLUSH刷清输出队列
        // TCIOFLUSH刷清输入、输出队列
        tcflush (context->fd, TCIOFLUSH);

        // TCSANOW立即生效,
        // TCSADRAIN：Wait until everything has been transmitted；
        // TCSAFLUSH：Flush input and output buffers and make the change
        if (tcsetattr(context->fd, TCSANOW, &options) != 0) {
            LOGE("tty set error!\n");
            break;
        }

        return HY_ERR_OK;
    } while (0);

    _uart_destroy(context);
    return HY_ERR_FAILD;
}

void HyUartDestroy(void **handle)
{
    HY_ASSERT_VAL_RET(!handle || !*handle);

    _uart_context_t *context = *handle;

    _uart_destroy(context);

    HY_MEM_FREE_PP(handle);

    LOGI("uart destroy successful \n");
}

void *HyUartCreate(HyUartConfig_t *config)
{
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    _uart_context_t *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_uart_context_t *, sizeof(*context));
        HY_MEMCPY(&context->save_config, &config->save_config, sizeof(context->save_config));

        if (HY_ERR_OK != _uart_create(context, config->dev_path_name)) {
            LOGE("_uart_create failed \n");
            break;
        }

        LOGI("uart create successful \n");
        return context;
    } while (0);

    HyUartDestroy((void **)&context);
    return NULL;
}

