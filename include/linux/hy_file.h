/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_file.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    04/11 2021 08:09
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        04/11 2021      create the file
 * 
 *     last modified: 04/11 2021 08:09
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_FILE_H_
#define __LIBHY_HAL_INCLUDE_HY_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

/**
 * @brief 阻塞非阻塞状态
 */
typedef enum {
    HY_FILE_BLOCK_STATE_BLOCK,          ///< 阻塞
    HY_FILE_BLOCK_STATE_NOBLOCK,        ///< 非阻塞
} HyFileBlockState_e;

/**
 * @brief 读取数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return
 *   1，成功返回读到的字节数
 *   2，被中断返回0
 *   3，失败返回-1
 */
ssize_t HyFileRead(int fd, void *buf, size_t len);

/**
 * @brief 读取数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 
 *   1，功返回len
 *   2，返回-1，表示read函数出现了系统相关错误
 *   3，返回字节数不等于len时，表示对端已经关闭
 */
ssize_t HyFileReadN(int fd, void *buf, size_t len);

/**
 * @brief 读取数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param cnt 数据的长度
 * @param ms 超时时间
 *
 * @return
 *   1，返回读到指定的字节数
 *   2，失败放回-1
 *   3，超时返回0
 */
ssize_t HyFileReadNTimeout(int fd, void *buf, size_t cnt, size_t ms);

/**
 * @brief 写入数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 成功返回写入的字节数，失败返回-1，写中断返回0
 */
ssize_t HyFileWrite(int fd, const void *buf, size_t len);

/**
 * @brief 写入数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 成功返回len，失败返回-1
 */
ssize_t HyFileWriteN(int fd, const void *buf, size_t len);

/**
 * @brief 设置文件阻塞非阻塞
 *
 * @param fd 文件描述符
 * @param state 状态
 *
 * @return 成功返回0， 失败返回-1
 */
int32_t HyFileBlockStateSet(int32_t fd, HyFileBlockState_e state);

/**
 * @brief 获取文件阻塞状态
 *
 * @param fd 文件描述符
 *
 * @return 返回阻塞状态
 */
HyFileBlockState_e HyFileBlockStateGet(int32_t fd);

/**
 * @brief 获取文件的长度
 *
 * @param file 文件
 *
 * @return 成功返回实际长度，失败返回-1
 */
long HyFileGetLen(const char *file);

#ifdef __cplusplus
}
#endif

#endif

