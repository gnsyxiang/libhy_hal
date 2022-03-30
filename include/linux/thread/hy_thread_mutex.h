/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_mutex.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/01 2022 19:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/01 2022      create the file
 * 
 *     last modified: 21/01 2022 19:40
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_THREAD_MUTEX_H_
#define __LIBHY_HAL_INCLUDE_HY_THREAD_MUTEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

typedef struct {
} HyThreadMutexSaveConfig_s;

typedef struct {
    HyThreadMutexSaveConfig_s   save_c;
} HyThreadMutexConfig_s;

void *HyThreadMutexCreate(HyThreadMutexConfig_s *thread_mutex_c);
void HyThreadMutexDestroy(void **handle);

hy_s32_t HyThreadMutexLock(void *handle);
hy_s32_t HyThreadMutexUnLock(void *handle);

#define HyThreadMutexCreate_m()                 \
    ({                                          \
        HyThreadMutexConfig_s thread_mutex_c;   \
        HyThreadMutexCreate(&thread_mutex_c);   \
     })

#ifdef __cplusplus
}
#endif

#endif

