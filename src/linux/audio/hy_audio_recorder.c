/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_audio_recorder.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 10:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 10:43
 */
#include <stdio.h>

#include "hy_log.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_thread.h"

#include "tinyalsa/pcm.h"

#include "hy_thread_sem.h"
#include "hy_audio_recorder.h"

#define _dump_tinyalsa_config(_audio_recorder_c, _save_c)                   \
    do {                                                                    \
        LOGD("capture \n");                                                 \
        LOGD("channel:\t\t%d \n",          _audio_recorder_c->channel);     \
        LOGD("rate:\t\t\t%d \n",           _audio_recorder_c->rate);        \
        LOGD("bit:\t\t\t%d \n",            _audio_recorder_c->bit);         \
                                                                            \
        LOGD("period_size:\t\t%d \n",      _save_c->period_size);           \
        LOGD("period_count:\t\t%d \n",     _save_c->period_count);          \
    } while (0)

#define _set_state_m(_context, _state)                                      \
    do {                                                                    \
        LOGI("state change from %d to %d \n", _context->state, _state);     \
        _context->state = _state;                                           \
    } while (0)

typedef struct {
    HyAudioRecordSaveConfig_s   save_c;

    struct pcm                  *pcm;
    char                        *buf;

    HyAudioRecorderState_e      state;
    void                        *wait_start_sem_h;
    void                        *wait_stop_sem_h;
    void                        *read_data_thread_h;
    hy_s32_t                    exit_flag;
} _audio_recorder_context_t;

static hy_s32_t _check_state(_audio_recorder_context_t *context,
        HyAudioRecorderState_e state)
{
    return (context->state == state);
}

static hy_s32_t _read_data_thread_cb(void *args)
{
    _audio_recorder_context_t *context = args;
    HyAudioRecordSaveConfig_s *save_c = &context->save_c;
    hy_s32_t frame = 0;

    HyThreadSemWait(context->wait_start_sem_h);

    while (!context->exit_flag) {
        if (_check_state(context, HY_AUDIO_RECORDER_STATE_STOP)) {
            _set_state_m(context, HY_AUDIO_RECORDER_STATE_IDEL);

            HyThreadSemPost_m(context->wait_stop_sem_h);
            LOGI("audio recorder force stop \n");

            HyThreadSemWait_m(context->wait_start_sem_h);
            LOGI("audio recorder start \n");

            continue;
        }

        frame = pcm_readi(context->pcm, context->buf, pcm_get_buffer_size(context->pcm));

        if (save_c->data_cb) {
            save_c->data_cb(context->buf,
                    pcm_frames_to_bytes(context->pcm, frame), save_c->args);
        }
    }

    return -1;
}

hy_s32_t HyAudioRecorderStart(void *handle)
{
    LOGT("handle: %p \n", handle);
    HY_ASSERT_RET_VAL(!handle, -1);

    _audio_recorder_context_t *context = handle;

    if (_check_state(context, HY_AUDIO_RECORDER_STATE_RUNNING)) {
        LOGE("wrong state \n");
        return -1;
    }

    _set_state_m(context, HY_AUDIO_RECORDER_STATE_RUNNING);
    HyThreadSemPost_m(context->wait_start_sem_h);

    LOGI("audio recorder start \n");
    return 0;
}

hy_s32_t HyAudioRecorderStop(void *handle)
{
    LOGT("handle: %p \n", handle);
    HY_ASSERT_RET_VAL(!handle, -1);

    _audio_recorder_context_t *context = handle;

    if (_check_state(context, HY_AUDIO_RECORDER_STATE_STOP)) {
        LOGE("wrong state \n");
        return -1;
    }

    _set_state_m(context, HY_AUDIO_RECORDER_STATE_STOP);
    if (!context->exit_flag) {
        HyThreadSemWait_m(context->wait_stop_sem_h);
    }

    return 0;
}

static void _tinyalsa_destroy(_audio_recorder_context_t *context)
{
    if (context->pcm) {
        pcm_close(context->pcm);
        context->pcm = NULL;
    }

    if (context->buf) {
        HY_MEM_FREE_PP(&context->buf);
    }
}

static hy_s32_t _tinyalsa_create(_audio_recorder_context_t *context,
        HyAudioRecorderConfig_s *audio_recorder_c)
{
    do {
        HyAudioRecordSaveConfig_s *save_c = &context->save_c;
        struct pcm_config pcm_config;
        hy_u32_t size = 0;

        HY_MEMSET(&pcm_config, sizeof(pcm_config));

        pcm_config.channels     = audio_recorder_c->channel;
        pcm_config.rate         = audio_recorder_c->rate;
        pcm_config.period_size  = save_c->period_size;
        pcm_config.period_count = save_c->period_count;
        if (audio_recorder_c->bit == 8) {
            pcm_config.format   = PCM_FORMAT_S8;
        } else if (audio_recorder_c->bit == 16) {
            pcm_config.format   = PCM_FORMAT_S16_LE;
        } else if (audio_recorder_c->bit == 24) {
            pcm_config.format   = PCM_FORMAT_S24_LE;
        } else if (audio_recorder_c->bit == 32) {
            pcm_config.format   = PCM_FORMAT_S32_LE;
        }

        context->pcm = pcm_open(0, 0, PCM_IN, &pcm_config);
        if(!context->pcm || !pcm_is_ready(context->pcm)) {
            LOGE("failed to open PCM, %s \n", pcm_get_error(context->pcm));
            break;
        }

        size = pcm_frames_to_bytes(context->pcm, pcm_get_buffer_size(context->pcm));
        context->buf = HY_MEM_MALLOC_BREAK(char *, size);

        _dump_tinyalsa_config(audio_recorder_c, save_c);

        return 0;
    } while (0);

    _tinyalsa_destroy(context);
    return -1;
}

void HyAudioRecorderDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _audio_recorder_context_t *context = *handle;

    context->exit_flag = 1;
    HyThreadSemPost_m(context->wait_start_sem_h);
    HyThreadDestroy(&context->read_data_thread_h);

    _tinyalsa_destroy(context);

    HyThreadSemDestroy(&context->wait_start_sem_h);
    HyThreadSemDestroy(&context->wait_stop_sem_h);

    LOGI("audio recorder destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyAudioRecorderCreate(HyAudioRecorderConfig_s *audio_recorder_c)
{
    LOGT("audio_recorder_c: %p \n", audio_recorder_c);
    HY_ASSERT_RET_VAL(!audio_recorder_c, NULL);

    _audio_recorder_context_t *context = NULL;

    do {
        context = HY_MEM_MALLOC_BREAK(_audio_recorder_context_t *,
                sizeof(*context));

        HY_MEMCPY(&context->save_c,
                &audio_recorder_c->save_c, sizeof(context->save_c));

        if (0 != _tinyalsa_create(context, audio_recorder_c)) {
            LOGE("tinyalsa creaete failed \n");
            break;
        }

        context->wait_start_sem_h = HyThreadSemCreate_m(0);
        if (!context->wait_start_sem_h) {
            LOGE("HyThreadSemCreate_m failed \n");
            break;
        }

        context->wait_stop_sem_h = HyThreadSemCreate_m(0);
        if (!context->wait_stop_sem_h) {
            LOGE("HyThreadSemCreate_m failed \n");
            break;
        }

        context->read_data_thread_h = HyThreadCreate_m("HYAR_read_data",
                _read_data_thread_cb, context);
        if (!context->read_data_thread_h) {
            LOGE("HyThreadCreate_m failed \n");
            break;
        }

        LOGI("audio recorder create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("audio recorder create failed \n");
    HyAudioRecorderDestroy((void **)&context);
    return NULL;
}

