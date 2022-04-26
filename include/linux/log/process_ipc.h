/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    process_ipc.h
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
#ifndef __LIBHY_HAL_INCLUDE_PROCESS_IPC_H_
#define __LIBHY_HAL_INCLUDE_PROCESS_IPC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_log.h"

#define LOG_IPC_NAME            "log_ipc_socket"

void *process_ipc_server_create(hy_u32_t fifo_len);
void process_ipc_server_destroy(void **handle);

#ifdef __cplusplus
}
#endif

#endif

