/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    dynamic_array.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/04 2022 09:04
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/04 2022      create the file
 * 
 *     last modified: 22/04 2022 09:04
 */
#ifndef __LIBHY_HAL_INCLUDE_DYNAMIC_ARRAY_H_
#define __LIBHY_HAL_INCLUDE_DYNAMIC_ARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

/**
 * @brief 创建动态数组
 *
 * @param min_len 最小长度
 * @param max_len 最大长度
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *dynamic_array_create(hy_u32_t min_len, hy_u32_t max_len);

/**
 * @brief 销毁动态数组
 *
 * @param handle 句柄的地址（二级指针）
 */
void dynamic_array_destroy(void **handle);

/**
 * @brief 重置动态数组
 *
 * @param handle 句柄
 */
void dynamic_array_reset(void *handle);

/**
 * @brief 获取动态数组的长度
 *
 * @param handle 句柄
 *
 * @return 长度 
 */
hy_s32_t dynamic_array_get_len(void *handle);

/**
 * @brief 从动态数组中读取
 *
 * @param handle 句柄
 * @param buf 数组
 * @param len 长度
 *
 * @return 实际返回的长度
 */
hy_s32_t dynamic_array_read(void *handle, void *buf, hy_u32_t len);

/**
 * @brief 向动态数组中写入
 *
 * @param handle 句柄
 * @param buf 数组
 * @param len 长度
 *
 * @return 成功返回写入的长度，失败返回-1
 */
hy_s32_t dynamic_array_write(void *handle, const void *buf, hy_u32_t len);

#ifdef __cplusplus
}
#endif

#endif

