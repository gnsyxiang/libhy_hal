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
 * @brief 获取SD卡的空闲大小
 *
 * @param mount_path SD卡的挂载路径
 * @param free_size SD卡的空闲大小，单位M
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyStorageSdGetFree(const char *mount_path, uint32_t *free_size);

/**
 * @brief 获取SD卡中空闲大小所占的比例
 *
 * @param mount_path SD卡的挂载路径
 * @param free_ratio SD卡空闲大小占用的比例
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyStorageSdGetFreeRatio(const char *mount_path, float *free_ratio);

/**
 * @brief 获取SD卡容量相关信息
 *
 * @param mount_path SD卡的挂载路径
 * @param total_size SD卡总大小哦，单位M
 * @param free_size SD卡空闲大小，单位M
 * @param free_ratio SD卡空闲大小占用的比例
 *
 * @return 成功返回0，失败返回-1
 */
int32_t HyStorageSdGetInfo(const char *mount_path,
        uint32_t *total_size, uint32_t *free_size, float *free_ratio);

#ifdef __cplusplus
}
#endif

#endif
