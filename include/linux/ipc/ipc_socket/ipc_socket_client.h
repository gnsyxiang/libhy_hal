/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    ipc_socket_client.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 16:37
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 16:37
 */
#ifndef __LIBHY_HAL_INCLUDE_IPC_SOCKET_CLIENT_H_
#define __LIBHY_HAL_INCLUDE_IPC_SOCKET_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ipc_socket.h"

void *ipc_socket_client_create(const char *ipc_name, HyIpcSocketType_e type);
void ipc_socket_client_destroy(void **handle);

hy_s32_t ipc_socket_client_connect(void *handle, hy_u32_t timeout_s);

#ifdef __cplusplus
}
#endif

#endif

