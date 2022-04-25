/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_pipe_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/03 2022 19:05
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/03 2022      create the file
 * 
 *     last modified: 03/03 2022 19:05
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
#include "hy_thread.h"
#include "hy_log.h"
#include "hy_file.h"
#include "hy_pipe.h"

#define _APP_NAME "hy_pipe_demo"

typedef struct {
    void        *pipe_h;
    void        *pipe_read_thread_h;
    void        *pipe_write_thread_h;

    hy_s32_t    is_exit;
} _main_context_s;

static hy_s32_t _pipe_read_cb(void *args)
{
    _main_context_s *context = args;
    hy_s32_t id = 0;
    hy_s32_t cnt = 0;
    hy_s32_t ret = 0;

    while (cnt++ < 5) {
        ret = HyPipeRead(context->pipe_h, &id, sizeof(id));
        LOGD("ret: %d, id: %d \n", ret, id);
        if (ret > 0) {
            break;
        }
        sleep(1);
    }

    return -1;
}

static hy_s32_t _pipe_write_cb(void *args)
{
    _main_context_s *context = args;
    hy_s32_t id = 4;

    sleep(3);
    HyPipeWrite(context->pipe_h, &id, sizeof(id));

    return -1;
}

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
    _main_context_s *context = *context_pp;

    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {"pipe write thread",   &context->pipe_write_thread_h,  HyThreadDestroy},
        {"pipe read thread",    &context->pipe_read_thread_h,   HyThreadDestroy},
        {"pipe",                &context->pipe_h,               HyPipeDestroy},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);

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
    signal_c.save_c.app_name      = _APP_NAME;
    signal_c.save_c.coredump_path = "./";
    signal_c.save_c.error_cb      = _signal_error_cb;
    signal_c.save_c.user_cb       = _signal_user_cb;
    signal_c.save_c.args          = context;

    HyModuleCreateBool_s bool_module[] = {
        {"log",         &log_c,         (HyModuleCreateBoolCb_t)HyLogInit,          HyLogDeInit},
        {"signal",      &signal_c,      (HyModuleCreateBoolCb_t)HySignalCreate,     HySignalDestroy},
    };

    HY_MODULE_RUN_CREATE_BOOL(bool_module);

    HyPipeConfig_s pipe_c;
    HY_MEMSET(&pipe_c, sizeof(pipe_c));
    pipe_c.read_block_state = HY_PIPE_BLOCK_STATE_NOBLOCK;

    HyThreadConfig_s pipe_read_thread_c;
    HY_MEMSET(&pipe_read_thread_c, sizeof(pipe_read_thread_c));
    pipe_read_thread_c.save_c.thread_loop_cb    = _pipe_read_cb;
    pipe_read_thread_c.save_c.args              = context;
    HY_STRNCPY(pipe_read_thread_c.save_c.name, HY_THREAD_NAME_LEN_MAX,
            "hy_pipe_read", HY_STRLEN("hy_pipe_read"));

    HyThreadConfig_s pipe_write_thread_c;
    HY_MEMSET(&pipe_write_thread_c, sizeof(pipe_write_thread_c));
    pipe_write_thread_c.save_c.thread_loop_cb   = _pipe_write_cb;
    pipe_write_thread_c.save_c.args             = context;
    HY_STRNCPY(pipe_write_thread_c.save_c.name, HY_THREAD_NAME_LEN_MAX,
            "hy_pipe_write", HY_STRLEN("hy_pipe_write"));

    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {"pipe",                &context->pipe_h,               &pipe_c,                (HyModuleCreateHandleCb_t)HyPipeCreate,     HyPipeDestroy},
        {"pipe read thread",    &context->pipe_read_thread_h,   &pipe_read_thread_c,    (HyModuleCreateHandleCb_t)HyThreadCreate,   HyThreadDestroy},
        {"pipe write thread",   &context->pipe_write_thread_h,  &pipe_write_thread_c,   (HyModuleCreateHandleCb_t)HyThreadCreate,   HyThreadDestroy},
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

    while (!context->is_exit) {
        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}

