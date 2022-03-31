/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_sem.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 08:37
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 08:37
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_THREAD_SEM_H_
#define __LIBHY_HAL_INCLUDE_HY_THREAD_SEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

/**
 * @brief 配置参数
 */
typedef struct {
    hy_u32_t    value;      ///< 信号量初始值
} HyThreadSemConfig_s;

/**
 * @brief 创建模块
 *
 * @param sem_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyThreadSemCreate(HyThreadSemConfig_s *sem_c);

/**
 * @brief 销毁模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HyThreadSemDestroy(void **handle);

/**
 * @brief 发送信号
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyThreadSemPost(void *handle);

/**
 * @brief 等待信号
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyThreadSemWait(void *handle);

/**
 * @brief 创建模块
 *
 * @param _value 信号量初始值
 *
 * @return 成功返回句柄，失败返回NULL
 */
#define HyThreadSemCreate_m(_value)                 \
    ({                                              \
        HyThreadSemConfig_s sem_c;                  \
        sem_c.value = _value;                       \
        HyThreadSemCreate(&sem_c);                  \
     })

/**
 * @brief 等待信号
 *
 * @param _handle 句柄
 *
 * @return 无
 */
#define HyThreadSemWait_m(_handle)                  \
    do {                                            \
        if (0 != HyThreadSemWait(_handle)) {        \
            LOGES("HyThreadSemWait failed \n");     \
        }                                           \
    } while (0)

/**
 * @brief 发送信号
 *
 * @param _handle 句柄
 *
 * @return 无
 */
#define HyThreadSemPost_m(_handle)                  \
    do {                                            \
        if (0 != HyThreadSemPost(_handle)) {        \
            LOGES("HyThreadSemPost failed \n");     \
        }                                           \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif

