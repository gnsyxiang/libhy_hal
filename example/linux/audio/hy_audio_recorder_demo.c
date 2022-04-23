/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_audio_recorder_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 14:42
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 14:42
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_signal.h"
#include "hy_module.h"
#include "hy_hal_utils.h"
#include "hy_log.h"

#include "hy_audio_recorder.h"

#define _APP_NAME "hy_audio_recorder_demo"

typedef struct {
    void        *log_h;
    void        *signal_h;

    void        *audio_recorder_h;
    hy_s32_t    fd;

    hy_s32_t    exit_flag;
} _main_context_t;

static void _audio_recorder_data_cb(const void *buf, hy_u32_t len, void *args)
{
    _main_context_t *context = args;

    write(context->fd, buf, len);
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

    // note: 增加或删除要同步到HyModuleCreateHandle_s中
    HyModuleDestroyHandle_s module[] = {
        {"audio recorder",  &context->audio_recorder_h,     HyAudioRecorderDestroy},
        {"signal",          &context->signal_h,             HySignalDestroy},
        {"log",             &context->log_h,                HyLogDestroy},
    };

    HY_MODULE_RUN_DESTROY_HANDLE(module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MEM_MALLOC_RET_VAL(_main_context_t *, sizeof(*context), NULL);

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

    HyAudioRecorderConfig_s audio_recorder_c;
    HY_MEMSET(&audio_recorder_c, sizeof(audio_recorder_c));
    audio_recorder_c.rate                   = 16 * 1000;
    audio_recorder_c.channel                = 1;
    audio_recorder_c.bit                    = 16;
    audio_recorder_c.save_c.period_size     = 1024;
    audio_recorder_c.save_c.period_count    = 4;
    audio_recorder_c.save_c.data_cb         = _audio_recorder_data_cb;
    audio_recorder_c.save_c.args            = context;

    // note: 增加或删除要同步到HyModuleDestroyHandle_s中
    HyModuleCreateHandle_s module[] = {
        {"log",             &context->log_h,                &log_c,                 (HyModuleCreateHandleCb_t)HyLogCreate,              HyLogDestroy},
        {"signal",          &context->signal_h,             &signal_c,              (HyModuleCreateHandleCb_t)HySignalCreate,           HySignalDestroy},
        {"audio recorder",  &context->audio_recorder_h,     &audio_recorder_c,      (HyModuleCreateHandleCb_t)HyAudioRecorderCreate,    HyAudioRecorderDestroy},
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

    context->fd = open("./audio_recorder.pcm", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (context->fd == -1) {
        LOGES("open failed \n");
    }

    HyAudioRecorderStart(context->audio_recorder_h);

    while (!context->exit_flag) {
        sleep(1);
    }

    HyAudioRecorderStop(context->audio_recorder_h);

    close(context->fd);

    _module_destroy(&context);

    return 0;
}

