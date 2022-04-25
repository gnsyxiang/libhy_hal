/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    ipc_socket_server.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 09:34
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 09:34
 */
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

#include "hy_hal/hy_assert.h"
#include "hy_hal/hy_log.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_string.h"

#include "ipc_socket_server.h"

typedef struct {
    ipc_socket_s    ipc_socket;         // 放在前面，用于强制类型转换

    hy_s32_t        pipe_fd[2];
    hy_s32_t        exit_flag:1;
    hy_s32_t        wait_exit_flag:1;
    hy_s32_t        reserved;
} _ipc_socket_server_s;

hy_s32_t ipc_socket_server_accept(void *handle,
        HyIpcSocketAcceptCb_t accept_cb, void *args)
{
    LOGT("handle: %p, accept_cb: %p, args: %p \n", handle, accept_cb, args);
    HY_ASSERT_RET_VAL(!handle || !accept_cb, -1);

    hy_s32_t fd = -1;
    fd_set read_fs;
    ipc_socket_s *new_socket = NULL;
    _ipc_socket_server_s *socket_server = handle;
    ipc_socket_s *ipc_socket = &socket_server->ipc_socket;

    if (listen(ipc_socket->fd, SOMAXCONN) < 0) {
        LOGES("listen failed, fd: %d \n", ipc_socket->fd);
        return -1;
    }

    LOGI("ipc socket server accept start \n");

    while (!socket_server->exit_flag) {
        FD_ZERO(&read_fs);
        FD_SET(ipc_socket->fd, &read_fs);
        FD_SET(socket_server->pipe_fd[0], &read_fs);

        if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) < 0) {
            LOGES("select failed \n");
            break;
        }

        if (FD_ISSET(socket_server->pipe_fd[0], &read_fs)) {
            // char buf;
            // read(socket_server->pipe_fd[0], &buf, sizeof(buf));

            LOGW("pipe break while for accept\n");
            break;
        }

        if (FD_ISSET(ipc_socket->fd, &read_fs)) {
            fd = accept(ipc_socket->fd, NULL, NULL);
            if (fd < 0) {
                LOGES("accept failed, fd: %d \n", ipc_socket->fd);
                break;
            }

            new_socket = ipc_socket_create(fd, ipc_socket->ipc_name,
                    HY_IPC_SOCKET_TYPE_CLIENT);
            if (!new_socket) {
                LOGE("ipc socket create failed \n");
                break;
            }

            new_socket->connect_state = HY_IPC_SOCKET_CONNECT_STATE_CONNECT;

            LOGI("accept new client ipc socket, ipc_socket: %p, fd: %d \n",
                    new_socket, new_socket->fd);

            accept_cb(new_socket, args);
        }
    }

    LOGI("ipc socket server accept stop \n");
    socket_server->wait_exit_flag = 1;

    return -1;
}

void ipc_socket_server_destroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _ipc_socket_server_s *socket_server = *handle;

    socket_server->exit_flag = 1;
    write(socket_server->pipe_fd[1], socket_server, sizeof(*socket_server));

    while (!socket_server->wait_exit_flag) {
        usleep(10 * 1000);
    }

    close(socket_server->pipe_fd[0]);
    close(socket_server->pipe_fd[1]);

    ipc_socket_destroy_2(&socket_server->ipc_socket);

    LOGI("ipc socket server destroy, socket_server: %p, "
            "pipe_fd[0]: %d, pipe_fd[1]: %d \n",
            socket_server,
            socket_server->pipe_fd[0], socket_server->pipe_fd[1]);
    HY_MEM_FREE_PP(handle);
}

void *ipc_socket_server_create(const char *ipc_name, HyIpcSocketType_e type)
{
    LOGT("ipc_name: %s, type: %d \n", ipc_name, type);
    HY_ASSERT_RET_VAL(!ipc_name, NULL);

    hy_u32_t addr_len;
    struct sockaddr_un addr;
    char ipc_path[HY_IPC_SOCKET_NAME_LEN_MAX] = {0};
    _ipc_socket_server_s *socket_server = NULL;

    do {
        socket_server = HY_MEM_MALLOC_BREAK(_ipc_socket_server_s *,
                sizeof(*socket_server));

        if (0 != pipe(socket_server->pipe_fd)) {
            LOGES("pipe failed \n");
            break;
        }

        ipc_socket_s *ipc_socket = &socket_server->ipc_socket;
        if (0 != ipc_socket_create_2(ipc_socket, ipc_name, type)) {
            LOGE("ipc socket create 2 failed \n");
            break;
        }

        HY_IPC_SOCKADDR_UN_INIT_(addr, addr_len, ipc_name);

        snprintf(ipc_path, HY_IPC_SOCKET_NAME_LEN_MAX,
                "%s/%s", HY_IPC_SOCKET_PATH_, ipc_name);
        if (0 == access(ipc_path, F_OK)) {
            remove(ipc_path);
        }

        if (bind(ipc_socket->fd, (const struct sockaddr *)&addr, addr_len) < 0) {
            LOGES("bind failed, fd: %d \n", ipc_socket->fd);
            break;
        }

        ipc_socket->connect_state = HY_IPC_SOCKET_CONNECT_STATE_CONNECT;

        LOGI("ipc socket server create, socket_server: %p, "
                "pipe_fd[0]: %d, pipe_fd[1]: %d \n",
                socket_server,
                socket_server->pipe_fd[0], socket_server->pipe_fd[1]);
        return socket_server;
    } while (0);

    LOGE("ipc socket server create failed \n");
    ipc_socket_server_destroy((void **)&socket_server);
    return NULL;
}
