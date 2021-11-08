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

ssize_t HyFileRead(int fd, void *buf, size_t len);

ssize_t HyFileWrite(int fd, const void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif

