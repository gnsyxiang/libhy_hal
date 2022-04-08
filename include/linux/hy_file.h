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

#include "hy_hal/hy_type.h"

/**
 * @brief 阻塞非阻塞状态
 */
typedef enum {
    HY_FILE_BLOCK_STATE_BLOCK,          ///< 阻塞
    HY_FILE_BLOCK_STATE_NOBLOCK,        ///< 非阻塞
} HyFileBlockState_e;

/**
 * @brief 判断文件是否存在
 *
 * @param file_path 文件路径
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyFileIsExist(const char *file_path);

/**
 * @brief 判断文件是否存在
 *
 * @param _file_path 文件路径
 *
 * @return 无
 */
#define HyFileIsExist_m(_file_path)                                     \
    do {                                                                \
        if (0 != HyFileIsExist(_file_path)) {                           \
            LOGE("HyFileIsExist failed, file_path: %s \n", _file_path); \
        }                                                               \
    } while (0)

/**
 * @brief 移除文件
 *
 * @param file_path 文件路径
 */
void HyFileRemove(const char *file_path);

/**
 * @brief 获取文件的长度
 *
 * @param file 文件
 *
 * @return 成功返回实际长度，失败返回-1
 */
hy_s64_t HyFileGetLen(const char *file);

/**
 * @brief 获取文件的内容
 *
 * @param file 文件
 * @param content 数据的地址
 *
 * @return 成功返回0，失败返回-1
 *
 * @note content在内部分配，使用完后切记要释放，否则造成内存泄露
 */
hy_s32_t HyFileGetContent(const char *file, char **content);

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
hy_s32_t HyFileRead(hy_s32_t fd, void *buf, hy_u32_t len);

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
hy_s32_t HyFileReadN(hy_s32_t fd, void *buf, hy_u32_t len);

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
hy_s32_t HyFileReadNTimeout(hy_s32_t fd, void *buf, hy_u32_t cnt, hy_u32_t ms);

/**
 * @brief 写入数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 成功返回写入的字节数，失败返回-1，写中断返回0
 */
hy_s32_t HyFileWrite(hy_s32_t fd, const void *buf, hy_u32_t len);

/**
 * @brief 写入数据
 *
 * @param fd 文件fd
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 成功返回len，失败返回-1
 */
hy_s32_t HyFileWriteN(hy_s32_t fd, const void *buf, hy_u32_t len);

/**
 * @brief 设置文件阻塞非阻塞
 *
 * @param fd 文件描述符
 * @param state 状态
 *
 * @return 成功返回0， 失败返回-1
 */
hy_s32_t HyFileBlockStateSet(hy_s32_t fd, HyFileBlockState_e state);

/**
 * @brief 获取文件阻塞状态
 *
 * @param fd 文件描述符
 *
 * @return 返回阻塞状态
 */
HyFileBlockState_e HyFileBlockStateGet(hy_s32_t fd);

#ifdef __cplusplus
}
#endif

#endif

