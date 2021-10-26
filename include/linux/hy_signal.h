/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_signal.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2021 19:00
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2021      create the file
 * 
 *     last modified: 25/10 2021 19:00
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_SIGNAL_H_
#define __LIBHY_HAL_INCLUDE_HY_SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <signal.h>

#define HY_SIGNAL_NUM_MAX_32   (64)         ///< 系统信号最大值

/**
 * @brief 配置结构体
 */
typedef struct {
    const char *app_name;                   ///< app名字
    const char *coredump_path;              ///< 生成coredump路径

    void (*user_cb)(void *args);            ///< 用户关心信号的回调函数
    void (*error_cb)(void *args);           ///< 程序运行错误信号的回调函数
    void *args;                             ///< 上层传递的参数
} HySignalSaveConfig_t;

/**
 * @brief 配置结构体
 */
typedef struct {
    int8_t user_num[HY_SIGNAL_NUM_MAX_32];  ///< 注册用户信号
    int8_t error_num[HY_SIGNAL_NUM_MAX_32]; ///< 注册系统错误信号

    HySignalSaveConfig_t    save_config;    ///< 参数，详见HySignalSaveConfig_t
} HySignalConfig_t;

/**
 * @brief 创建信号处理模块
 *
 * @param config 配置参数，详见HySignalConfig_t
 *
 * @return 模块句柄，失败返回NULL
 */
void *HySignalCreate(HySignalConfig_t *config);

/**
 * @brief 销毁信号处理模块
 *
 * @param handle 模块句柄的地址
 */
void HySignalDestroy(void **handle);

#ifdef __cplusplus
}
#endif

#endif

