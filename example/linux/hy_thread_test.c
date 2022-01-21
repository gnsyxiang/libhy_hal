/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 08:35
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 08:35
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_thread.h"

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_signal.h"
#include "hy_module.h"
#include "hy_hal_utils.h"
#include "hy_log.h"

typedef struct {
    void        *log_handle;
    void        *signal_handle;

    void        *thread_handle;

    hy_s32_t    exit_flag;
} _main_context_t;

static int32_t _print_loop_cb(void *args)
{
    _main_context_t *context = args;

    char name[HY_THREAD_NAME_LEN_MAX] = {0};
    pthread_t id;
    long pid;

    HyThreadGetInfo(context->thread_handle, HY_THREAD_INFO_NAME, name);
    HyThreadGetInfo(context->thread_handle, HY_THREAD_INFO_PID, &pid);
    HyThreadGetInfo(context->thread_handle, HY_THREAD_INFO_ID, &id);
    LOGI("name: %s, id: 0x%lx, pid: %ld \n", name, id, pid);

    while (!context->exit_flag) {
        LOGI("haha \n");
        sleep(1);
    }

    return -1;
}

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

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"thread",  &context->thread_handle,    HyThreadDestroy},
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
    log_config.save_config.buf_len_min  = 512;
    log_config.save_config.buf_len_max  = 512;
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

    HyThreadConfig_s thread_config;
    HY_MEMSET(&thread_config, sizeof(thread_config));
    thread_config.save_config.thread_loop_cb    = _print_loop_cb;
    thread_config.save_config.args              = context;
    HY_STRNCPY(thread_config.save_config.name,
            HY_THREAD_NAME_LEN_MAX, "print", HY_STRLEN("print"));

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,       &log_config,        (create_t)HyLogCreate,      HyLogDestroy},
        {"signal",  &context->signal_handle,    &signal_config,     (create_t)HySignalCreate,   HySignalDestroy},
        {"thread",  &context->thread_handle,    &thread_config,     (create_t)HyThreadCreate,   HyThreadDestroy},
    };

    RUN_CREATE(module);

    return context;
}

static hy_s32_t _thread_detach_loop_cb(void *args)
{
    LOGI("thread detach \n");

    return -1;
}

int main(int argc, char *argv[])
{
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGE("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    HyThreadConfig_s thread_config;
    memset(&thread_config, '\0', sizeof(thread_config));
    thread_config.save_config.destroy_flag      = HY_THREAD_DESTROY_GRACE;
    thread_config.save_config.detach_flag       = HY_THREAD_DETACH_YES;
    thread_config.save_config.reserved          = 0;
    thread_config.save_config.thread_loop_cb    = _thread_detach_loop_cb;
    thread_config.save_config.args              = context;
    HY_STRNCPY(thread_config.save_config.name, HY_THREAD_NAME_LEN_MAX,
            "hy_thd_test_detach", HY_STRLEN("hy_thd_test_detach"));
    if (NULL == HyThreadCreate(&thread_config)) {
        LOGE("HyThreadCreate_m fail \n");
    }

    while (!context->exit_flag) {
        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}
