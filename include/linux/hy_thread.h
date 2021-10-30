/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 08:26
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 08:26
 */
#ifndef __LIBHY_HAL_INCLULDE_HY_THREAD_H_
#define __LIBHY_HAL_INCLULDE_HY_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define HY_THREAD_NAME_LEN_MAX (16)

/**
 * @brief 线程回调函数
 *
 * @param 上层传递参数
 *
 * @return 返回0，线程继续运行，否则线程退出
 *
 * @note 创建线程里面有while循环，也可以在回调函数中增加while循环，但是要控制好退出
 */
typedef int32_t (*HyThreadLoopCb_t)(void *args);

/**
 * @brief 模块配置参数
 */
typedef struct {
    HyThreadLoopCb_t        thread_loop_cb;                     ///< 线程执行函数
    char                    name[HY_THREAD_NAME_LEN_MAX];       ///< 线程名字
    void                    *args;                              ///< 上层传递参数
} HyThreadSaveConfig_t;

/**
 * @brief 模块配置参数
 */
typedef struct {
    HyThreadSaveConfig_t    save_config;                        ///< 参数，详见HyThreadSaveConfig_t
} HyThreadConfig_t;

/**
 * @brief 创建线程
 *
 * @param config 配置参数，详见HyThreadConfig_t
 *
 * @return 线程句柄
 */
void *HyThreadCreate(HyThreadConfig_t *config);

/**
 * @brief 销毁线程
 *
 * @param handle 线程句柄的地址
 */
void HyThreadDestroy(void **handle);

#ifdef __cplusplus
}
#endif

#endif

