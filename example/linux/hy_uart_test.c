/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 09:45
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 09:45
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_uart.h"

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_signal.h"
#include "hy_module.h"
#include "hy_hal_utils.h"
#include "hy_log.h"

typedef struct {
    void *log_handle;
    void *signal_handle;
    void *uart_handle;

    hy_s32_t exit_flag;
} _main_context_t;

static void _signal_error_cb(void *args)
{
    LOGE("------error cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _signal_user_cb(void *args)
{
    LOGI("------user cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"uart",    &context->uart_handle,      HyUartDestroy},
        {"signal",  &context->signal_handle,    HySignalDestroy},
        {"log",     &context->log_handle,       HyLogDestroy},
    };

    RUN_DESTROY(module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MEM_MALLOC_RET_VAL(_main_context_t *, sizeof(*context), NULL);

    HyLogConfig_t log_config;
    log_config.save_config.buf_len      = 512;
    log_config.save_config.level        = HY_LOG_LEVEL_TRACE;
    log_config.save_config.color_enable = HY_TYPE_FLAG_ENABLE;

    int8_t signal_error_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGFPE,
        SIGSEGV, SIGBUS, SIGSYS, SIGXCPU, SIGXFSZ,
    };

    int8_t signal_user_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGINT, SIGTERM, SIGUSR1, SIGUSR2,
    };

    HySignalConfig_t signal_config;
    memset(&signal_config, 0, sizeof(signal_config));
    HY_MEMCPY(signal_config.error_num, signal_error_num, sizeof(signal_error_num));
    HY_MEMCPY(signal_config.user_num, signal_user_num, sizeof(signal_user_num));
    signal_config.save_config.app_name      = "template";
    signal_config.save_config.coredump_path = "./";
    signal_config.save_config.error_cb      = _signal_error_cb;
    signal_config.save_config.user_cb       = _signal_user_cb;
    signal_config.save_config.args          = context;

    HyUartConfig_t uart_config;
    uart_config.save_config.speed           = HY_UART_SPEED_115200;
    uart_config.save_config.flow_control    = HY_UART_FLOW_CONTROL_NONE;
    uart_config.save_config.data_bit        = HY_UART_DATA_BIT_8;
    uart_config.save_config.parity_type     = HY_UART_PARITY_NONE;
    uart_config.save_config.stop_bit        = HY_UART_STOP_BIT_1;
    HY_STRNCPY(uart_config.dev_path_name, HY_UART_DEV_PATH_NAME_LEN, "/dev/ttyUSB0", HY_STRLEN("/dev/ttyUSB0"));

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,       &log_config,        (create_t)HyLogCreate,      HyLogDestroy},
        {"signal",  &context->signal_handle,    &signal_config,     (create_t)HySignalCreate,   HySignalDestroy},
        {"uart",    &context->uart_handle,      &uart_config,       (create_t)HyUartCreate,     HyUartDestroy},
    };

    RUN_CREATE(module);

    return context;
}

int main(int argc, char *argv[])
{
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGE("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    ssize_t ret;
    char buf[BUF_LEN] = {0};
    while (!context->exit_flag) {
        ret = HyUartRead(context->uart_handle, buf, 12);
        if (ret == -1) {
            break;
        }

        LOG_HEX_ASCII(buf, ret);

        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}

