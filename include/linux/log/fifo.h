/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    fifo.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/04 2022 15:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/04 2022      create the file
 * 
 *     last modified: 21/04 2022 15:44
 */
#ifndef __LIBHY_HAL_INCLUDE_FIFO_H_
#define __LIBHY_HAL_INCLUDE_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

/**
 * @brief 打印FIFO相关信息
 */
typedef enum {
    FIFO_DUMP_TYPE_ALL,             ///< 打印FIFO全部信息，按照开辟的空间打印
    FIFO_DUMP_TYPE_CONTENT,         ///< 打印FIFO里面的有效内容

    FIFO_DUMP_TYPE_MAX = 0xffffffff,
} fifo_dump_type_e;

/**
 * @brief fifo上下文
 */
typedef struct {
    hy_u32_t    len;                ///< FIFO长度
    char        *buf;               ///< FIFO数据

    hy_u32_t    read_pos;           ///< 读位置
    hy_u32_t    write_pos;          ///< 写位置
} fifo_context_s;

/**
 * @brief 创建fifo模块
 *
 * @param len fifo长度
 *
 * @return 成功返回句柄，失败返回NULL
 */
fifo_context_s *fifo_create(hy_u32_t len);

/**
 * @brief 销毁fifo模块
 *
 * @param context_pp 句柄的地址（二级指针）
 */
void fifo_destroy(fifo_context_s **context_pp);

/**
 * @brief 向fifo中写入数据
 *
 * @param context 句柄
 * @param buf 数据
 * @param len 长度
 *
 * @return 成功返回写入的长度，失败返回-1
 */
hy_s32_t fifo_write(fifo_context_s *context, const void *buf, hy_u32_t len);

/**
 * @brief 从fifo中读取数据
 *
 * @param context 句柄
 * @param buf 数据
 * @param len 长度
 *
 * @return 成功返回读取的长度，失败返回-1
 */
hy_s32_t fifo_read(fifo_context_s *context, void *buf, hy_u32_t len);

/**
 * @brief 从fifo中读取数据
 *
 * @param context 句柄
 * @param buf 数据
 * @param len 长度
 *
 * @return 成功返回读取的长度，失败返回-1
 *
 * @note 该操作不会删除数据
 */
hy_s32_t fifo_read_peek(fifo_context_s *context, void *buf, hy_u32_t len);

/**
 * @brief 打印fifo中的数据
 *
 * @param context 句柄
 * @param type 打印的类型
 */
void fifo_dump(fifo_context_s *context, fifo_dump_type_e type);

// 删除指定长度的数据
#define FIFO_READ_DEL(context)  (context)->read_pos += len

// 复位fifo
#define FIFO_RESET(context)                                 \
    do {                                                    \
        HY_MEMSET((context)->buf, (context)->len);          \
        (context)->write_pos = (context)->read_pos = 0;     \
    } while (0)

// 空返回1，否则返回0
#define FIFO_IS_EMPTY(context)      ((context)->read_pos == (context)->write_pos)

// 满返回1，否则返回0
#define FIFO_IS_FULL(context)       ((context)->len == FIFO_USED_LEN(context))

// fifo中有效数据的长度
#define FIFO_USED_LEN(context)      ((context)->write_pos - (context)->read_pos)

// fifo中剩余空间
#define FIFO_FREE_LEN(context)      ((context)->len - (FIFO_USED_LEN(context)))

// 读指针位置
#define FIFO_READ_POS(context)      ((context)->read_pos & ((context)->len - 1))    // 优化 context->read_pos % context->len

// 写指针位置
#define FIFO_WRITE_POS(context)     ((context)->write_pos & ((context)->len - 1))   // 优化 context->write_pos % context->len

#ifdef __cplusplus
}
#endif

#endif

