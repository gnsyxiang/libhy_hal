/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    log_file.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 13:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 13:51
 */
#ifndef __LIBHY_HAL_INCLUDE_LOG_FILE_H_
#define __LIBHY_HAL_INCLUDE_LOG_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

hy_s32_t log_file_write(hy_s32_t fd, const void *buf, hy_u32_t len);
hy_s32_t log_file_read(hy_s32_t fd, void *buf, hy_u32_t len);

#ifdef __cplusplus
}
#endif

#endif

