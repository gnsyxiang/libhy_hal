/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    01/11 2021 10:24
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        01/11 2021      create the file
 * 
 *     last modified: 01/11 2021 10:24
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_signal.h"
#include "hy_thread.h"
#include "hy_module.h"
#include "hy_hal_utils.h"
#include "hy_log.h"

#define _DEMO_THREAD_CNT (20)
#define _APP_NAME "hy_log_demo"

typedef struct {
    void        *thread_h[_DEMO_THREAD_CNT];

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

static void _module_destroy(_main_context_s **context_pp)
{
    HyModuleDestroyBool_s bool_module[] = {
        {"signal",          HySignalDestroy },
        {"log",             HyLogDeInit     },
    };

    HY_MODULE_RUN_DESTROY_BOOL(bool_module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_s *_module_create(void)
{
    _main_context_s *context = HY_MEM_MALLOC_RET_VAL(_main_context_s *, sizeof(*context), NULL);

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

    return context;
}

static void _demo_log(void)
{
    LOGT("hello world \n");
    LOGD("hello world \n");
    LOGI("hello world \n");
    LOGW("hello world \n");
    LOGE("hello world \n");
    LOGF("hello world \n");
}

static hy_s32_t _demo_loop_cb(void *args)
{
    usleep(500 * 1000);

    LOGI("---haha\n");

    return -1;
}

int main(int argc, char *argv[])
{
    _main_context_s *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGE("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    _demo_log();

    for (hy_u32_t i = 0; i < _DEMO_THREAD_CNT; ++i) {
        char name[32] = {0};
        snprintf(name, 32, "hy_log_demo_%d", i);

        context->thread_h[i] = HyThreadCreate_m(name,
                _demo_loop_cb, context);
        if (!context->thread_h[i]) {
            LOGE("HyThreadCreate_m fail \n");
        }
    }

    while (!context->is_exit) {
        sleep(1);
    }

    for (hy_u32_t i = 0; i < _DEMO_THREAD_CNT; ++i) {
        HyThreadDestroy(&context->thread_h[i]);
    }

    _module_destroy(&context);

    return 0;
}

