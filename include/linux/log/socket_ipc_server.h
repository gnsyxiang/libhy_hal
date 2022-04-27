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

#include "log_private.h"
#include "epoll_helper.h"

typedef void (*socket_ipc_accept_cb_t)(hy_s32_t fd, void *args);

typedef struct {
    hy_s32_t                fd;

    epoll_helper_cb_param_s list_fd_cb_param;
    epoll_helper_context_s  *epoll_helper;

    socket_ipc_accept_cb_t  accept_cb;
    void                    *args;
} socket_ipc_server_s;

socket_ipc_server_s *socket_ipc_server_create(const char *name,
        socket_ipc_accept_cb_t accept_cb, void *args);
void socket_ipc_server_destroy(socket_ipc_server_s **socket_ipc_server_pp);

#ifdef __cplusplus
}
#endif

#endif

