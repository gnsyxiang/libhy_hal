/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    socket_ipc_server.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/04 2022 19:04
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/04 2022      create the file
 * 
 *     last modified: 25/04 2022 19:04
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

#include "log_private.h"

#include "socket_ipc_server.h"

static void *_thread_cb(void *args)
{
    socket_ipc_server_s *context = args;
    hy_s32_t fd = -1;
    fd_set read_fs;

    if (listen(context->fd, SOMAXCONN) < 0) {
        log_error("listen failed, fd: %d \n", context->fd);
        return NULL;
    }

    while (!context->is_exit) {
        FD_ZERO(&read_fs);
        FD_SET(context->fd, &read_fs);
        FD_SET(context->pipe_fd[0], &read_fs);

        if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) < 0) {
            log_error("select failed \n");
            break;
        }

        if (FD_ISSET(context->pipe_fd[0], &read_fs)) {
            log_error("pipe break while for accept\n");
            break;
        }

        if (FD_ISSET(context->fd, &read_fs)) {
            fd = accept(context->fd, NULL, NULL);
            if (fd < 0) {
                log_error("accept failed, fd: %d \n", context->fd);
                break;
            }

            if (context->accept_cb) {
                context->accept_cb(fd);
            }
        }
    }

    return NULL;
}

void socket_ipc_server_destroy(socket_ipc_server_s **socket_ipc_pp)
{
    if (!socket_ipc_pp || !*socket_ipc_pp) {
        log_error("the param is error \n");
        return;
    }

    socket_ipc_server_s *context = *socket_ipc_pp;

    close(context->fd);

    close(context->pipe_fd[0]);
    close(context->pipe_fd[1]);

    free(context);
    *socket_ipc_pp = NULL;
}

socket_ipc_server_s *socket_ipc_server_create(const char *name,
        socket_ipc_server_accept_cb_t accept_cb)
{
    if (!name || strlen(name) <= 0 || !accept_cb) {
        log_error("the param is error \n");
        return NULL;
    }

    socket_ipc_server_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        if (0 != pipe(context->pipe_fd)) {
            log_error("pipe failed \n");
            break;
        }

        if (0 != pthread_create(&context->id, NULL, _thread_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

        context->fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (context->fd < 0) {
            log_error("socket failed \n");
            break;
        }

        char ipc_path[SOCKET_IPC_SERVER_NAME_LEN_MAX] = {0};
        snprintf(ipc_path, sizeof(ipc_path), "%s/%s", "/tmp", name);
        if (0 == access(ipc_path, F_OK)) {
            remove(ipc_path);
        }

        hy_u32_t addr_len;
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, ipc_path);
        addr_len = strlen(ipc_path) + offsetof(struct sockaddr_un, sun_path);
        if (bind(context->fd, (const struct sockaddr *)&addr, addr_len) < 0) {
            log_error("bind failed, fd: %d \n", context->fd);
            break;
        }

        context->accept_cb = accept_cb;

        return context;
    } while (0);

    return NULL;
}

