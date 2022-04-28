/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    process_ipc_client.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 13:55
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 13:55
 */
#ifndef __LIBHY_HAL_INCLUDE_PROCESS_IPC_CLIENT_H_
#define __LIBHY_HAL_INCLUDE_PROCESS_IPC_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

struct log_write_info_tag;

void *process_ipc_client_create(hy_u32_t fifo_len);
void process_ipc_client_destroy(void **handle);

void process_ipc_client_write(void *handle, struct log_write_info_tag *log_write_info);

#ifdef __cplusplus
}
#endif

#endif

