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

/**
 * @brief 配置参数
 */
typedef struct {
    hy_s32_t    reserved;   ///< 预留
} HyThreadMutexConfig_s;

/**
 * @brief 创建模块
 *
 * @param mutex_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyThreadMutexCreate(HyThreadMutexConfig_s *mutex_c);

/**
 * @brief 销毁模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HyThreadMutexDestroy(void **handle);

/**
 * @brief 关锁
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回其他值
 */
hy_s32_t HyThreadMutexLock(void *handle);

/**
 * @brief 开锁
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回其他值
 */
hy_s32_t HyThreadMutexUnLock(void *handle);

/**
 * @brief 尝试开锁
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回其他值
 */
hy_s32_t HyThreadMutexTryLock(void *handle);

/**
 * @brief 创建模块
 *
 * @return 
 */
#define HyThreadMutexCreate_m()                 \
    ({                                          \
        HyThreadMutexConfig_s thread_mutex_c;   \
        HyThreadMutexCreate(&thread_mutex_c);   \
     })

#ifdef __cplusplus
}
#endif

#endif

