/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    module_a.h
 * @brief   模块A对外提供的操作接口
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/03 2021 10:46
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/03 2021      create the file
 * 
 *     last modified: 05/03 2021 10:46
 */
#ifndef __AUTOTOOLS_DEMO_INCLUDE_MODULE_A_H_
#define __AUTOTOOLS_DEMO_INCLUDE_MODULE_A_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化模块A
 */
void HyModuleAInit(void);

/**
 * @brief 反初始化模块A
 */
void HyModuleADeInit(void);

#ifdef __cplusplus
}
#endif

#endif

