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
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_error.h"
#include "hy_log.h"
#include "hy_file.h"

#include "hy_uart.h"

typedef struct {
    HyUartSaveConfig_t  save_c;
    hy_s32_t            fd;
} _uart_context_s;

hy_s32_t HyUartWrite(void *handle, const void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    return HyFileWriteN(((_uart_context_s *)handle)->fd, buf, len);
}

hy_s32_t HyUartRead(void *handle, void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    return read(((_uart_context_s *)handle)->fd, buf, len);
}

hy_s32_t HyUartReadN(void *handle, void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    _uart_context_s *context = handle;
    hy_s32_t ret = 0;

    ret = HyFileReadN(context->fd, buf, len);
    if (-1 == ret) {
        LOGE("hy file read n failed \n");

        close(context->fd);
        context->fd = -1;
        return -1;
    } else if (ret >= 0 && ret != (hy_s32_t)len) {
        LOGE("hy file read n error, ret: %d \n", ret);

        return -1;
    } else {
        return len;
    }
}

static inline void _uart_destroy(_uart_context_s *context)
{
    if (context->fd > 0) {
        close(context->fd);
    }
}

static hy_s32_t _uart_create(_uart_context_s *context, char *name)
{
    HyUartSaveConfig_t *save_c = &context->save_c;
    struct termios options;

    do {
        // O_NONBLOCK和O_NDELAY区别:
        // 它们的差别在于设立O_NDELAY会使I/O函式马上回传0，但是又衍生出一个问题，
        // 因为读取到档案结尾时所回传的也是0，这样无法得知是哪中情况；
        // 因此，O_NONBLOCK就产生出来，它在读取不到数据时会回传-1，并且设置errno为EAGAIN。
        hy_s32_t flags = O_RDWR | O_NOCTTY;
        // hy_s32_t flags = O_RDWR | O_NOCTTY | O_NONBLOCK;
        // hy_s32_t flags = O_RDWR | O_NOCTTY | O_NDELAY;

        context->fd = open(name, flags);
        if (-1 == context->fd) {
            LOGE("open %s failed \n", name);
            break;
        }
        LOGI("open %s successful \n", name);

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
        speed_t speed_2_speed[HY_UART_SPEED_MAX] = {
            B2400,      B4800,      B9600,      B19200,     B38400,
            B57600,     B115200,    B230400,    B460800,    B500000,
            B576000,    B921600,    B1000000,   B1152000,   B1500000,
            B2000000,   B2500000,   B3000000,   B3500000,   B4000000,
        };
        cfsetispeed(&options, speed_2_speed[save_c->speed]);
        cfsetospeed(&options, speed_2_speed[save_c->speed]);

        // 设置数据位
        hy_s32_t data_bit_2_bit[HY_UART_DATA_BIT_MAX] = {CS5, CS6, CS7, CS8};
        options.c_cflag &= ~CSIZE;  // 清零
        options.c_cflag |= data_bit_2_bit[save_c->data_bit];

        // 设置校验位
        switch (save_c->parity_type) {
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
        switch (save_c->stop_bit) {
            case HY_UART_STOP_BIT_1:    options.c_cflag &= ~CSTOPB;         break;
            case HY_UART_STOP_BIT_2:    options.c_cflag |= CSTOPB;          break;
            case HY_UART_STOP_BIT_MAX:  LOGE("the stop_bit is error \n");   break;
        }

        // 设置数据流控
        switch (save_c->flow_control) {
            case HY_UART_FLOW_CONTROL_NONE:     options.c_cflag &= ~CRTSCTS;             break;
            case HY_UART_FLOW_CONTROL_HARDWARE: options.c_cflag |= CRTSCTS;              break;
            case HY_UART_FLOW_CONTROL_SOFT:     options.c_cflag |= IXON | IXOFF | IXANY; break;
            case HY_UART_FLOW_CONTROL_MAX:      LOGE("the flow_control is error \n");    break;
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
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _uart_context_s *context = *handle;

    _uart_destroy(context);

    LOGI("uart destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyUartCreate(HyUartConfig_t *uart_c)
{
    LOGT("uart_c: %p \n", uart_c);
    HY_ASSERT_RET_VAL(!uart_c, NULL);

    _uart_context_s *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_uart_context_s *, sizeof(*context));
        HY_MEMCPY(&context->save_c, &uart_c->save_c, sizeof(context->save_c));

        if (HY_ERR_OK != _uart_create(context, uart_c->dev_path)) {
            LOGE("_uart_create failed \n");
            break;
        }

        LOGI("uart create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("uart create failed \n");
    HyUartDestroy((void **)&context);
    return NULL;
}

