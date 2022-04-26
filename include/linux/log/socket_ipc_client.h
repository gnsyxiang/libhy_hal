/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    socket_ipc_client.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 11:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 11:34
 */
#ifndef __LIBHY_HAL_INCLUDE_SOCKET_IPC_CLIENT_H_
#define __LIBHY_HAL_INCLUDE_SOCKET_IPC_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "log_private.h"

typedef struct {
    hy_s32_t        fd;
} socket_ipc_client_s;

socket_ipc_client_s *socket_ipc_client_create(const char *name);
void socket_ipc_client_destroy(socket_ipc_client_s **socket_ipc_client_pp);

hy_s32_t socket_ipc_client_write(socket_ipc_client_s *socket_ipc_client,
        const void *buf, hy_u32_t len);

#ifdef __cplusplus
}
#endif

#endif

