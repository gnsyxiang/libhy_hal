/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket_server_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/03 2022 10:14
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/03 2022      create the file
 * 
 *     last modified: 30/03 2022 10:14
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

typedef struct {
    void        *log_h;
    void        *signal_h;
    void        *socket_h;

    hy_s32_t    exit_flag;
} _main_context_s;

#define _APP_NAME "hy_socket_server_demo"

static void _new_client_cb(void *handle, const char *ip, void *args)
{
    LOGD("handle: %p, ip: %s, args: %p \n", handle, ip, args);

    char buf[16] = {0};
    hy_s32_t ret = 0;
    while (1) {
        memset(buf, '\0', sizeof(buf));
        ret = HYSocketRead(handle, buf, 15);
        if (ret == -1) {
            break;
        }

        LOGE("buf: %s \n", buf);
    }

    HySocketDestroy(&handle);
}

static void _signal_error_cb(void *args)
{
    LOGE("------error cb\n");

    _main_context_s *context = args;
    context->exit_flag = 1;
}

static void _signal_user_cb(void *args)
{
    LOGW("------user cb\n");

    _main_context_s *context = args;
    context->exit_flag = 1;
}

static void _module_destroy(_main_context_s **context_pp)
{
    _main_context_s *context = *context_pp;

    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {"client server",   &context->socket_h,     HySocketDestroy},
        {"signal",          &context->signal_h,     HySignalDestroy},
        {"log",             &context->log_h,        HyLogDestroy},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_s *_module_create(void)
{
    _main_context_s *context = HY_MEM_MALLOC_RET_VAL(_main_context_s *, sizeof(*context), NULL);

    HyLogConfig_s log_c;
#if 0
    log_c.save_c.buf_len_min  = 512;
    log_c.save_c.buf_len_max  = 512;
    log_c.save_c.level        = HY_LOG_LEVEL_TRACE;
    log_c.save_c.color_enable = HY_TYPE_FLAG_ENABLE;
#endif

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
    signal_c.save_c.app_name      = _APP_NAME;
    signal_c.save_c.coredump_path = "./";
    signal_c.save_c.error_cb      = _signal_error_cb;
    signal_c.save_c.user_cb       = _signal_user_cb;
    signal_c.save_c.args          = context;

    HySocketConfig_t socket_c;
    HY_MEMSET(&socket_c, sizeof(socket_c));
    socket_c.type = HY_SOCKET_TYPE_SERVER;

    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {"log",             &context->log_h,        &log_c,         (HyModuleCreateHandleCb_t)HyLogCreate,          HyLogDestroy},
        {"signal",          &context->signal_h,     &signal_c,      (HyModuleCreateHandleCb_t)HySignalCreate,       HySignalDestroy},
        {"client server",   &context->socket_h,     &socket_c,      (HyModuleCreateHandleCb_t)HySocketCreate,       HySocketDestroy},
    };

    HY_MODULE_RUN_CREATE_HANDLE(module);

    return context;
}

int main(int argc, char *argv[])
{
    _main_context_s *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGE("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    HySocketWaitConnect(context->socket_h, 7890, _new_client_cb, context);

    while (!context->exit_flag) {
        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}

