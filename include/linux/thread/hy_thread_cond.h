/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_cond.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 09:31
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 09:31
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_THREAD_COND_H_
#define __LIBHY_HAL_INCLUDE_HY_THREAD_COND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

typedef struct {
    hy_s32_t    reserved;   ///< 预留
} HyThreadCondConfig_s;

void *HyThreadCondCreate(HyThreadCondConfig_s *cond_c);
void HyThreadCondDestroy(void **handle);

hy_s32_t HyThreadCondSignal(void *handle);
hy_s32_t HyThreadCondBroadcast(void *handle);

hy_s32_t HyThreadCondWait(void *handle, void *thread_mutex_h, hy_u32_t timeout);

#ifdef __cplusplus
}
#endif

#endif

