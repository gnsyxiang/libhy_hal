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

/**
 * @brief 热插拔设备类型
 */
typedef enum {
    HY_HOTPLUG_TYPE_UNKNOW,                 ///< 无效设备

    HY_HOTPLUG_TYPE_UDISK,                  ///< U盘
    HY_HOTPLUG_TYPE_SDCARD,                 ///< sd卡

    HY_HOTPLUG_TYPE_MAX,
} HyHotplugType_e;

/**
 * @brief 设备状态
 */
typedef enum {
    HY_HOTPLUG_STATE_INSERT,                ///< 插入
    HY_HOTPLUG_STATE_PULL_OUT,              ///< 拔出
} HyHotplugState_e;

/**
 * @brief 设备状态改变时的回调函数
 *
 * @param type 设备类型
 * @param name 设备节点名
 * @param state 设备状态
 * @param args 上层传递参数
 *
 * @return 无
 */
typedef void (*HyHotplugCb_t)(HyHotplugType_e type, const char *name,
        HyHotplugState_e state, void *args);

/**
 * @brief 配置结构体
 */
typedef struct {
    HyHotplugCb_t           hotplug_cb;     ///< 回调函数
    void                    *args;          ///< 上层参数
} HyHotplugSaveConfig_s;

/**
 * @brief 配置结构体
 */
typedef struct {
    HyHotplugSaveConfig_s   save_c;         ///< 配置参数
} HyHotplugConfig_s;

/**
 * @brief 创建模块
 *
 * @param hotplug_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyHotplugCreate(HyHotplugConfig_s *hotplug_c);

/**
 * @brief 销毁模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HyHotplugDestroy(void **handle);

/**
 * @brief 创建模块
 *
 * @param _hotplug_cb 回调函数
 * @param _args 上层传递参数
 *
 * @return 
 */
#define HyHotplugCreate_m(_hotplug_cb, _args)           \
    ({                                                  \
        HyHotplugConfig_s hotplug_c;                    \
        hotplug_c.save_c.hotplug_cb     = _hotplug_cb;  \
        hotplug_c.save_c.args           = _args;        \
        HyHotplugCreate(&hotplug_c);                    \
     })

#ifdef __cplusplus
}
#endif

#endif

