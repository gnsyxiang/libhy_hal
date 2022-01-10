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

/**
 * @brief 从文件中读取数据
 *
 * @param fd 文件fd
 * @param buf 存放数据的地址
 * @param len 读取的长度
 *
 * @return 返回读到的字节数，失败放回-1，被中断返回0
 */
ssize_t HyFileRead(int fd, void *buf, size_t len);

/**
 * @brief 从文件中读取数据
 *
 * @param fd 文件fd
 * @param buf 存放数据的地址
 * @param len 读取的长度
 *
 * @return 返回读到指定的字节数，失败放回-1或0
 */
ssize_t HyFileReadN(int fd, void *buf, size_t len);

/**
 * @brief 从文件中读取数据
 *
 * @param fd 文件fd
 * @param buf 存放数据的地址
 * @param cnt 读取数据的长度
 * @param ms 超时时间
 *
 * @return 返回读到指定的字节数，失败放回-1或0
 */
ssize_t HyFileReadNTimeout(int fd, void *buf, size_t cnt, size_t ms);

/**
 * @brief 向文件写入数据
 *
 * @param fd 文件fd
 * @param buf 存放数据的地址
 * @param len 写入的数据长度
 *
 * @return 成功返回len，失败返回-1
 */
ssize_t HyFileWriteN(int fd, const void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif

