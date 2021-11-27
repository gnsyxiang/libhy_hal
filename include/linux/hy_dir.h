/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_dir.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 08:59
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 08:59
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_DIR_H_
#define __LIBHY_HAL_INCLUDE_HY_DIR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 类型
 */
typedef enum {
    HY_DIR_TYPE_FILE,       ///< 文件
    HY_DIR_TYPE_DIR,        ///< 目录

    HY_DIR_TYPE_MAX,
} HyDirType_t;

/**
 * @brief 读取到文件的回调函数
 *
 * @param path 文件所在的路径
 * @param name 文件名
 * @param type 文件的类型，详见HyDirType_t
 * @param args 上层传递的参数
 *
 * @return 返回0表示继续查找，返回-1表示退出查找(找到了需要的文件，直接退出)
 */
typedef int32_t (*HyDirReadCb_t)(const char *path, const char *name,
        uint8_t type, void *args);

/**
 * @brief 读取目录下的所有文件
 *
 * @param path 需要读取的路径名
 * @param filter 过滤后缀，NULL为不过滤
 * @param read_cb 读取到文件的回调函数，详见HyDirReadCb_t
 * @param args 上层传递的参数
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyDirRead(const char *path, const char *filter,
        HyDirReadCb_t read_cb, void *args);

/**
 * @brief 递归读取目录下的所有文件
 *
 * @param path 需要读取的路径名
 * @param filter 过滤后缀，NULL为不过滤
 * @param read_cb 读取到文件的回调函数，详见HyDirReadCb_t
 * @param args 上层传递的参数
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyDirReadRecurse(const char *path, const char *filter,
        HyDirReadCb_t read_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif

