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
 * @brief 线程退出方式
 */
typedef enum {
    HY_THREAD_DESTROY_GRACE,                                ///< 优雅退出，等待线程执行完
    HY_THREAD_DESTROY_FORCE,                                ///< 强制退出，等待一定时间(1s)后强制退出

    HY_THREAD_DESTROY_MAX,
} HyThreadDestroyFlag_t;

/**
 * @brief 线程回调函数
 *
 * @param 上层传递参数
 *
 * @return 返回0，线程继续运行，否则线程退出
 *
 * @note 创建线程里面有while循环，
 *       也可以在回调函数中增加while循环，但是要控制好退出
 */
typedef int32_t (*HyThreadLoopCb_t)(void *args);

/**
 * @brief 模块配置参数
 */
typedef struct {
    char                    name[HY_THREAD_NAME_LEN_MAX];   ///< 线程名字
    HyThreadLoopCb_t        thread_loop_cb;                 ///< 线程执行函数，详见HyThreadLoopCb_t
    int32_t                 flag;                           ///< 线程退出方式，详见HyThreadDestroyFlag_t
    void                    *args;                          ///< 上层传递参数
} HyThreadSaveConfig_t;

/**
 * @brief 模块配置参数
 */
typedef struct {
    HyThreadSaveConfig_t    save_config;                    ///< 模块配置参数，详见HyThreadSaveConfig_t
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

/**
 * @brief 创建线程宏
 *
 * @param _name 名字
 * @param _thread_loop_cb 回调函数，详见HyThreadLoopCb_t
 * @param _flag 线程退出方式，详见HyThreadDestroyFlag_t
 * @param _args 上层传递参数
 *
 * @return 线程句柄
 */
#define HyThreadCreate_m(_name, _thread_loop_cb, _flag, _args)      \
    ({                                                              \
        HyThreadConfig_t config;                                    \
        memset(&config, '\0', sizeof(config));                      \
        config.save_config.flag             = _flag;                \
        config.save_config.thread_loop_cb   = _thread_loop_cb;      \
        config.save_config.args             = _args;                \
        HY_STRNCPY(config.save_config.name,                         \
                HY_THREAD_NAME_LEN_MAX, _name, HY_STRLEN(_name));   \
        HyThreadCreate(&config);                                    \
     })

#ifdef __cplusplus
}
#endif

#endif
