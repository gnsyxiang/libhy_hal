/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_ipc_socket_client_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    19/01 2022 08:45
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        19/01 2022      create the file
 * 
 *     last modified: 19/01 2022 08:45
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_hal/hy_type.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_string.h"
#include "hy_hal/hy_signal.h"
#include "hy_hal/hy_module.h"
#include "hy_hal/hy_hal_utils.h"
#include "hy_hal/hy_log.h"

#include "hy_ipc_socket.h"

#define _APP_NAME "hy_ipc_socket_client_demo"
#define _IPC_SOCKET_IPC_NAME    "hy_ipc_server"

typedef struct {
    void        *ipc_socket_h;

    hy_s32_t    exit_flag;
} _main_context_t;

static void _signal_error_cb(void *args)
{
    LOGE("------error cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _signal_user_cb(void *args)
{
    LOGW("------user cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {"socket client",   &context->ipc_socket_h,         HyIpcSocketDestroy},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);

    HyModuleDestroyBool_s bool_module[] = {
        {"signal",          HySignalDestroy },
        {"log",             HyLogDeInit     },
    };

    HY_MODULE_RUN_DESTROY_BOOL(bool_module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MEM_MALLOC_RET_VAL(_main_context_t *, sizeof(*context), NULL);

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

    HyIpcSocketConfig_s ipc_socket_config;
    HY_MEMSET(&ipc_socket_config, sizeof(ipc_socket_config));
    ipc_socket_config.type      = HY_IPC_SOCKET_TYPE_CLIENT;
    ipc_socket_config.ipc_name  = _IPC_SOCKET_IPC_NAME;

    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {"socket client",   &context->ipc_socket_h,     &ipc_socket_config,     (HyModuleCreateHandleCb_t)HyIpcSocketCreate,    HyIpcSocketDestroy},
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

    if (0 != HyIpcSocketConnect(context->ipc_socket_h, 10)) {
        LOGE("connect server failed \n");
        context->exit_flag = 1;
    }

    const char *buf = "haha";
    hy_s32_t ret = 0;

    while (!context->exit_flag) {
        ret = HyIpcSocketWrite(context->ipc_socket_h, buf, HY_STRLEN(buf));
        if (ret < 0) {
            LOGE("HyIpcSocketWrite failed, ipc_socket_h: %p \n",
                    context->ipc_socket_h);
            break;
        }
        LOGI("buf: %s \n", buf);

        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}
