/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    ipc_socket_server.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 09:23
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 09:23
 */
#ifndef __LIBHY_HAL_INCLUDE_IPC_SOCKET_SERVER_H_
#define __LIBHY_HAL_INCLUDE_IPC_SOCKET_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ipc_socket.h"

void *ipc_socket_server_create(const char *ipc_name, HyIpcSocketType_e type);
void ipc_socket_server_destroy(void **handle);

hy_s32_t ipc_socket_server_accept(void *handle,
        HyIpcSocketAcceptCb_t accept_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif
