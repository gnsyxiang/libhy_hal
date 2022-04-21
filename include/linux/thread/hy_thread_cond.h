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

/**
 * @brief 配置参数
 */
typedef struct {
    hy_s32_t    reserved;       ///< 预留
} HyThreadCondConfig_s;

/**
 * @brief 创建条件变量模块
 *
 * @param cond_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyThreadCondCreate(HyThreadCondConfig_s *cond_c);

#define HyThreadCondCreate_m()                          \
    ({                                                  \
        HyThreadCondConfig_s cond_c;                    \
        HY_MEMSET(&cond_c, sizeof(cond_c));             \
        HyThreadCondCreate(&cond_c);                    \
     })

/**
 * @brief 销毁天剑变量模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HyThreadCondDestroy(void **handle);

/**
 * @brief 发送信号
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyThreadCondSignal(void *handle);

#define HyThreadCondSignal_m(_handle)                   \
    do {                                                \
        if (0 != HyThreadCondSignal(_handle)) {         \
            LOGES("HyThreadCondSignal failed \n");      \
        }                                               \
    } while (0)

/**
 * @brief 广播信号
 *
 * @param handle 句柄
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyThreadCondBroadcast(void *handle);

#define HyThreadCondBroadcast_m(_handle)                \
    do {                                                \
        if (0 != HyThreadCondBroadcast(_handle)) {      \
            LOGES("HyThreadCondBroadcast failed \n");   \
        }                                               \
    } while (0)

/**
 * @brief 等待信号
 *
 * @param handle 句柄
 * @param mutex_h 锁句柄
 * @param timeout_ms 超时时间，0表示阻塞等待
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyThreadCondWait(void *handle, void *_mutex_h, hy_u32_t timeout_ms);

#define HyThreadCondWait_m(_handle, _mutex_h, _timeout_ms)              \
    do {                                                                \
        if (0 != HyThreadCondWait(_handle, _mutex_h, _timeout_ms)) {    \
            LOGES("HyThreadCondWait failed \n");                        \
        }                                                               \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif

