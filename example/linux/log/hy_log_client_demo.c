/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log_client_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/05 2022 14:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/05 2022      create the file
 * 
 *     last modified: 03/05 2022 14:48
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_signal.h"
#include "hy_module.h"
#include "hy_hal_utils.h"
#include "hy_log.h"
#include "hy_socket.h"

#define _APP_NAME "hy_template_demo"

typedef struct {
    void        *client_h;
    hy_s32_t    is_exit;
} _main_context_s;

static void _signal_error_cb(void *args)
{
    LOGE("------error cb\n");

    _main_context_s *context = args;
    context->is_exit = 1;
}

static void _signal_user_cb(void *args)
{
    LOGW("------user cb\n");

    _main_context_s *context = args;
    context->is_exit = 1;
}

static void _bool_module_destroy(void)
{
    HyModuleDestroyBool_s bool_module[] = {
        {"signal",          HySignalDestroy },
        {"log",             HyLogDeInit     },
    };

    HY_MODULE_RUN_DESTROY_BOOL(bool_module);
}

static hy_s32_t _bool_module_create(_main_context_s *context)
{
    HyLogConfig_s log_c;
    HY_MEMSET(&log_c, sizeof(log_c));
    log_c.fifo_len                  = 10 * 1024;
    log_c.save_c.mode               = HY_LOG_MODE_PROCESS_SINGLE;
    log_c.save_c.level              = HY_LOG_LEVEL_TRACE;
    log_c.save_c.output_format      = HY_LOG_OUTFORMAT_ALL;

    int8_t signal_error_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGFPE,
        SIGSEGV, SIGBUS, SIGSYS, SIGXCPU, SIGXFSZ,
    };

    int8_t signal_user_num[HY_SIGNAL_NUM_MAX_32] = {
        SIGINT, SIGTERM, SIGUSR1, SIGUSR2,
    };

    HySignalConfig_t signal_c;
    memset(&signal_c, 0, sizeof(signal_c));
    HY_MEMCPY(signal_c.error_num, signal_error_num, sizeof(signal_error_num));
    HY_MEMCPY(signal_c.user_num, signal_user_num, sizeof(signal_user_num));
    signal_c.save_c.app_name        = _APP_NAME;
    signal_c.save_c.coredump_path   = "./";
    signal_c.save_c.error_cb        = _signal_error_cb;
    signal_c.save_c.user_cb         = _signal_user_cb;
    signal_c.save_c.args            = context;

    HyModuleCreateBool_s bool_module[] = {
        {"log",         &log_c,         (HyModuleCreateBoolCb_t)HyLogInit,          HyLogDeInit},
        {"signal",      &signal_c,      (HyModuleCreateBoolCb_t)HySignalCreate,     HySignalDestroy},
    };

    HY_MODULE_RUN_CREATE_BOOL(bool_module);
}

static void _handle_module_destroy(_main_context_s *context)
{
    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {"socket client",    &context->client_h,   HySocketDestroy},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);
}

static hy_s32_t _handle_module_create(_main_context_s *context)
{
    HySocketConfig_t socket_c;
    HY_MEMSET(&socket_c, sizeof(socket_c));
    socket_c.type = HY_SOCKET_TYPE_CLIENT;

    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {"socket client",    &context->client_h,   &socket_c,        (HyModuleCreateHandleCb_t)HySocketCreate,  HySocketDestroy},
    };

    HY_MODULE_RUN_CREATE_HANDLE(module);
}

int main(int argc, char *argv[])
{
    _main_context_s *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_main_context_s *, sizeof(*context));

        if (0 != _bool_module_create(context)) {
            printf("_bool_module_create failed \n");
            break;
        }

        if (0 != _handle_module_create(context)) {
            LOGE("_handle_module_create failed \n");
            break;
        }

        LOGE("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

        HySocketConnect(context->client_h, "192.168.1.58", 7890);

        char buf[1024] = {0};
        while (!context->is_exit) {
            HY_MEMSET(buf, sizeof(buf));
            HYSocketRead(context->client_h, buf, sizeof(buf));

            printf("%s", buf);
        }
    } while (0);

    _handle_module_destroy(context);
    _bool_module_destroy();
    HY_MEM_FREE_PP(&context);

    return 0;
}
