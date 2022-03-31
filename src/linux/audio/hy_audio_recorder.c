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
#include "tinyalsa/mixer.h"

#include "hy_thread_sem.h"
#include "hy_audio_recorder.h"

typedef struct {
    HyAudioRecordSaveConfig_s   save_c;

    struct pcm                  *pcm;
    struct mixer                *mixer;

    hy_u32_t                    card;
    hy_u32_t                    device;
    hy_u32_t                    pcm_flags;

    hy_u32_t                    bytes_per_frame;
    hy_u32_t                    frames_per_sec;
    hy_u32_t                    buffer_size;

    HyAudioRecorderState_e      state;
    void                        *wait_start_sem_h;
    void                        *wait_stop_sem_h;
    void                        *read_data_thread_h;
    hy_s32_t                    exit_flag;
} _audio_recorder_context_t;

#define _set_state_m(context, _state)                                   \
    do {                                                                \
        LOGI("state change from %d to %d \n", context->state, _state);  \
        context->state = _state;                                        \
    } while (0)

static hy_s32_t _check_state(_audio_recorder_context_t *context,
        HyAudioRecorderState_e state)
{
    return (context->state == state);
}

static hy_s32_t _read_data_thread_cb(void *args)
{
    _audio_recorder_context_t *context = args;
    HyAudioRecordSaveConfig_s *save_c = &context->save_c;
    char *buf = NULL;
    hy_s32_t frame = 0;
    hy_u32_t len = save_c->period_size * save_c->period_count;

    LOGE("len: %d, %d \n", len, context->bytes_per_frame);

    if (len % context->bytes_per_frame) {
        LOGE("the frame is error \n");
        context->exit_flag = 1;
        return -1;
    }

    HyThreadSemWait(context->wait_start_sem_h);

    buf = HY_MEM_MALLOC_RET_VAL(char *, len, -1);

    while (!context->exit_flag) {
        if (_check_state(context, HY_AUDIO_RECORDER_STATE_STOP)) {
            _set_state_m(context, HY_AUDIO_RECORDER_STATE_IDEL);

            HyThreadSemPost_m(context->wait_stop_sem_h);
            LOGI("audio recorder force stop \n");

            HyThreadSemWait_m(context->wait_start_sem_h);
            LOGI("audio recorder start \n");

            continue;
        }

        frame = pcm_readi(context->pcm, buf, len / context->bytes_per_frame);

        if (save_c->data_cb) {
            save_c->data_cb(buf,
                    pcm_frames_to_bytes(context->pcm, frame), save_c->args);
        }
    }

    HY_MEM_FREE_PP(&buf);

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

static inline void _dump_audio_recorder_config(struct pcm_config *config,
        hy_s32_t bit)
{
    char *format_str[] = {
        "PCM_FORMAT_S16_LE",
        "PCM_FORMAT_S8",
        "PCM_FORMAT_S16_BE",
        "PCM_FORMAT_S24_LE",
        "PCM_FORMAT_S24_BE",
        "PCM_FORMAT_S24_3LE",
        "PCM_FORMAT_S24_3BE",
        "PCM_FORMAT_S32_LE",
        "PCM_FORMAT_S32_BE",
        "PCM_FORMAT_MAX",
    };

    LOGT("capture \n");
    LOGT("channel:\t\t%d \n",          config->channels);
    LOGT("rate:\t\t\t%d \n",           config->rate);
    LOGT("bit:\t\t\t%s \n",            format_str[bit]);

    LOGT("period_size:\t\t%d \n",      config->period_size);
    LOGT("period_count:\t\t%d \n",     config->period_count);
}

static void _tinyalsa_destroy(_audio_recorder_context_t *context)
{
    if (context->mixer) {
        mixer_close(context->mixer);
        context->mixer = NULL;
    }

    if (context->pcm) {
        pcm_close(context->pcm);
        context->pcm = NULL;
    }
}

static hy_s32_t _tinyalsa_create(_audio_recorder_context_t *context,
        HyAudioRecorderConfig_s *audio_recorder_c)
{
    do {
        HyAudioRecordSaveConfig_s *save_c = &context->save_c;

        hy_s32_t format[] = {
            PCM_FORMAT_S16_LE,
            PCM_FORMAT_S8,
            PCM_FORMAT_S16_BE,
            PCM_FORMAT_S24_LE,
            PCM_FORMAT_S24_BE,
            PCM_FORMAT_S24_3LE,
            PCM_FORMAT_S24_3BE,
            PCM_FORMAT_S32_LE,
            PCM_FORMAT_S32_BE,
            PCM_FORMAT_MAX
        };

        struct pcm_config pcm_config = {0};

        pcm_config.channels     = audio_recorder_c->channel;
        pcm_config.rate         = audio_recorder_c->rate;
        pcm_config.format       = format[audio_recorder_c->bit];

        pcm_config.period_size  = save_c->period_size;
        pcm_config.period_count = save_c->period_count;

        _dump_audio_recorder_config(&pcm_config, audio_recorder_c->bit);

        context->card            = 0;
        context->device          = 0;
        context->pcm_flags       = PCM_IN;

        context->pcm = pcm_open(context->card, context->device, context->pcm_flags, &pcm_config);
        if(NULL == context->pcm) {
            LOGE("failed to allocate memory for PCM \n");
            break;
        } else if (!pcm_is_ready(context->pcm)){
            LOGE("failed to open PCM, %s\n", pcm_get_error(context->pcm));
            break;
        }

        context->mixer = mixer_open(context->card);
        if(NULL == context->mixer){
            LOGE("mixer_open failed (%s)\n", pcm_get_error(context->pcm));
            break;
        }

        context->bytes_per_frame = pcm_frames_to_bytes(context->pcm, 1);
        context->frames_per_sec = pcm_get_rate(context->pcm);
        context->buffer_size = pcm_frames_to_bytes(context->pcm, pcm_get_buffer_size(context->pcm));

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

