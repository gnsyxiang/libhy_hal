/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_audio_recorder.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 10:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 10:13
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_AUDIO_RECORDER_H_
#define __LIBHY_HAL_INCLUDE_HY_AUDIO_RECORDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

typedef enum {
    HY_AUDIO_RECORDER_STATE_IDEL,
    HY_AUDIO_RECORDER_STATE_RUNNING,
    HY_AUDIO_RECORDER_STATE_STOP,

    HY_AUDIO_RECORDER_STATE_MAX,
} HyAudioRecorderState_e;

typedef void (*HyAudioRecorderDataCb_t)(const void *buf, hy_u32_t len, void *args);

typedef struct {
    hy_s32_t                    period_size;
    hy_s32_t                    period_count;

    HyAudioRecorderDataCb_t     data_cb;
    void                        *args;
} HyAudioRecordSaveConfig_s;

typedef struct {
    HyAudioRecordSaveConfig_s   save_c;

    hy_s32_t                    rate;
    hy_s32_t                    channel;
    hy_s32_t                    bit;
} HyAudioRecorderConfig_s;

void *HyAudioRecorderCreate(HyAudioRecorderConfig_s *audio_recorder_c);
void HyAudioRecorderDestroy(void **handle);

hy_s32_t HyAudioRecorderStart(void *handle);
hy_s32_t HyAudioRecorderStop(void *handle);

#ifdef __cplusplus
}
#endif

#endif

