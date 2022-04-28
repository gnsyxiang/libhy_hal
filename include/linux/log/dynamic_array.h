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

#include "hy_type.h"

/**
 * @brief 动态数组
 *
 * @note 空间不够时，自动开辟空间，直到开辟到最大的空间
 */
typedef struct {
    char        *buf;       ///< 存放数据的地方
    hy_u32_t    len;        ///< 开辟空间的大小
    hy_u32_t    cur_len;    ///< 已经存储数据的大小

    hy_u32_t    write_pos;  ///< 写位置
    hy_u32_t    read_pos;   ///< 读位置

    hy_u32_t    min_len;    ///< 开辟空间的最小长度
    hy_u32_t    max_len;    ///< 开辟空间的最大长度
} dynamic_array_s;

/**
 * @brief 创建动态数组
 *
 * @param min_len 最小长度
 * @param max_len 最大长度
 *
 * @return 成功返回句柄，失败返回NULL
 */
dynamic_array_s *dynamic_array_create(hy_u32_t min_len, hy_u32_t max_len);

/**
 * @brief 销毁动态数组
 *
 * @param context_pp 句柄的地址（二级指针）
 */
void dynamic_array_destroy(dynamic_array_s **context_pp);

/**
 * @brief 从动态数组中读取
 *
 * @param context 句柄
 * @param buf 数组
 * @param len 长度
 *
 * @return 返回实际读取到的长度
 */
hy_s32_t dynamic_array_read(dynamic_array_s *context, void *buf, hy_u32_t len);

/**
 * @brief 向动态数组中写入
 *
 * @param context 句柄
 * @param buf 数组
 * @param len 长度
 *
 * @return 成功返回写入的长度，失败返回-1
 */
hy_s32_t dynamic_array_write(dynamic_array_s *context,
        const void *buf, hy_u32_t len);

/**
 * @brief 向动态数组中写入
 *
 * @param context 句柄
 * @param format 用户格式
 * @param args 用户数据
 *
 * @return 成功返回写入的长度，失败返回-1
 */
hy_s32_t dynamic_array_write_vprintf(dynamic_array_s *context,
        const char *format, va_list *args);

/**
 * @brief 复位动态数组，清空里面的数据
 *
 * @param _context 句柄
 */
#define DYNAMIC_ARRAY_RESET(_context)                       \
    do {                                                    \
        memset((_context)->buf, '\0', (_context)->len);     \
        (_context)->cur_len     = 0;                        \
        (_context)->write_pos   = 0;                        \
        (_context)->read_pos    = 0;                        \
    } while (0);

#ifdef __cplusplus
}
#endif

#endif

