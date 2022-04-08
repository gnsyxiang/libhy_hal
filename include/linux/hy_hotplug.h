/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_hotplug.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/04 2022 09:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/04 2022      create the file
 * 
 *     last modified: 08/04 2022 09:32
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_HOTPLUG_H_
#define __LIBHY_HAL_INCLUDE_HY_HOTPLUG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HY_HOTPLUG_TYPE_UDISK,
    HY_HOTPLUG_TYPE_SDCARD,
} HyHotplugType_e;

typedef enum {
    HY_HOTPLUG_STATE_INSERT,
    HY_HOTPLUG_STATE_PULL_OUT,
} HyHotplugState_e;

typedef void (*HyHotplugCb_t)(HyHotplugType_e type,
        HyHotplugState_e state, void *args);

typedef struct {
    HyHotplugCb_t           hotplug_cb;
    void                    *args;
} HyHotplugSaveConfig_s;

typedef struct {
    HyHotplugSaveConfig_s   save_c;
} HyHotplugConfig_s;

void *HyHotplugCreate(HyHotplugConfig_s *hotplug_c);
void HyHotplugDestroy(void **handle);

#ifdef __cplusplus
}
#endif

#endif

