/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_storage.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/01 2022 09:19
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/01 2022      create the file
 * 
 *     last modified: 06/01 2022 09:19
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_STORAGE_H_
#define __LIBHY_HAL_INCLUDE_HY_STORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 获取存储介质的空闲大小
 *
 * @param mount_path 存储介质的挂载路径
 * @param free_size 存储介质的空闲大小
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyStorageGetFree(const char *mount_path, uint32_t *free_size);

/**
 * @brief 获取存储介质中空闲大小所占的比例
 *
 * @param mount_path 存储介质的挂载路径
 * @param ratio 空闲大小占用的比例
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyStorageGetFreeRatio(const char *mount_path, float *ratio);

#ifdef __cplusplus
}
#endif

#endif

