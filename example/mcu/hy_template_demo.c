/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_template_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 19:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 19:30
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_module.h"
#include "hy_uart.h"
#include "hy_hal_utils.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    void        *debug_uart_h;
    void        *log_h;

    hy_s32_t    exit_flag;
} _main_context_t;

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {"log",         &context->log_h,           HyLogDestroy},
        {"debug_uart",  &context->debug_uart_h,    HyUartDestroy},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MEM_MALLOC_RET_VAL(_main_context_t *, sizeof(*context), NULL);

    HyUartConfig_t debug_uart_config;
    debug_uart_config.config_save.num       = DEBUG_UART_NUM;
    debug_uart_config.config_save.read_cb   = NULL;
    debug_uart_config.config_save.args      = context;
    debug_uart_config.rate                  = HY_UART_RATE_115200;
    debug_uart_config.flow_control          = HY_UART_FLOW_CONTROL_NONE;
    debug_uart_config.bits                  = HY_UART_BITS_8;
    debug_uart_config.parity                = HY_UART_PARITY_N;
    debug_uart_config.stop                  = HY_UART_STOP_1;

    HyLogConfig_t log_config;
    log_c.save_c.buf_len      = 512;
    log_c.save_c.level        = HY_LOG_LEVEL_TRACE;
    log_c.save_c.color_enable = HY_TYPE_FLAG_ENABLE;

    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {"debug_uart",  &context->debug_uart_h,    &debug_uart_config,     (HyModuleCreateHandleCb_t)HyUartCreate,     HyUartDestroy},
        {"log",         &context->log_h,           &log_config,            (HyModuleCreateHandleCb_t)HyLogCreate,      HyLogDestroy},
    };

    HY_MODULE_RUN_CREATE_HANDLE(module);

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

    for (;;) {
    }

    _module_destroy(&context);

    return 0;
}

