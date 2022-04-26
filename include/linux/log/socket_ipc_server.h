/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    socket_ipc_server.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/04 2022 19:02
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/04 2022      create the file
 * 
 *     last modified: 25/04 2022 19:02
 */
#ifndef __LIBHY_HAL_INCLUDE_SOCKET_IPC_SERVER_H_
#define __LIBHY_HAL_INCLUDE_SOCKET_IPC_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#include "hy_type.h"

#define SOCKET_IPC_SERVER_NAME_LEN_MAX     (64)

typedef void (*socket_ipc_server_accept_cb_t)(hy_s32_t fd);

typedef struct {
    hy_s32_t        fd;
    char            name[SOCKET_IPC_SERVER_NAME_LEN_MAX / 2];

    pthread_t       id;
    hy_s32_t        is_exit;
    hy_s32_t        pipe_fd[2];

    socket_ipc_server_accept_cb_t accept_cb;
} socket_ipc_server_s;

socket_ipc_server_s *socket_ipc_server_create(const char *name,
        socket_ipc_server_accept_cb_t accept_cb);
void socket_ipc_server_destroy(socket_ipc_server_s **socket_ipc_pp);

#ifdef __cplusplus
}
#endif

#endif

