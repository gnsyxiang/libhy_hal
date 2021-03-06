/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    process_server.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/04 2022 17:25
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/04 2022      create the file
 * 
 *     last modified: 25/04 2022 17:25
 */
#ifndef __LIBHY_HAL_INCLUDE_PROCESS_SERVER_H_
#define __LIBHY_HAL_INCLUDE_PROCESS_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

struct log_write_info_tag;

void *process_server_create(hy_u32_t fifo_len);
void process_server_destroy(void **handle);

void process_server_write(void *handle, struct log_write_info_tag *log_write_info);

#ifdef __cplusplus
}
#endif

#endif

