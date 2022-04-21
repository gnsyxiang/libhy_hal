/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_hal_misc.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/04 2022 10:36
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/04 2022      create the file
 * 
 *     last modified: 12/04 2022 10:36
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_HAL_MISC_H_
#define __LIBHY_HAL_INCLUDE_HY_HAL_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 1，去掉文件路径，只获取文件名
 *  1.1，使用strrchr函数，包含头文件#include <string.h>
 *      #define HY_STRRCHR_FILE (strrchr(__FILE__, '/'))
 *      #define HY_FILENAME     (HY_STRRCHR_FILE ? (HY_STRRCHR_FILE + 1) : __FILE__)
 *  1.2，使用basename函数，包含头文件#include <libgen.h>
 *      basename(__FILE__)
 */
#define HY_STRRCHR_FILE (strrchr(__FILE__, '/'))
#define HY_FILENAME     (HY_STRRCHR_FILE ? (HY_STRRCHR_FILE + 1) : __FILE__)

#ifdef __cplusplus
}
#endif

#endif

