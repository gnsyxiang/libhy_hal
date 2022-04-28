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
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <unistd.h>

#include "log_private.h"
#include "epoll_helper.h"

#include "socket_ipc_server.h"

static hy_s32_t _ipc_server_fd_create(socket_ipc_server_s *context, const char *name)
{
    do {
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

        if (listen(context->fd, SOMAXCONN) < 0) {
            log_error("listen failed, fd: %d \n", context->fd);
            return -1;
        }

        return 0;
    } while (0);

    return -1;
}

static void _epoll_handle_data(epoll_helper_cb_param_s *cb_param)
{
    assert(cb_param);
    hy_s32_t fd;
    socket_ipc_server_s *context = cb_param->args;

    fd = accept(context->fd, NULL, NULL);
    if (fd < 0) {
        log_error("accept failed, fd: %d \n", context->fd);
        return;
    }
    log_info("new client fd: %d \n", fd);

    if (context->accept_cb) {
        context->accept_cb(fd, context->args);
    }

    epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, cb_param);
}

void socket_ipc_server_destroy(socket_ipc_server_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is error \n");
        return;
    }

    socket_ipc_server_s *context = *context_pp;
    log_info("socket ipc server context: %p destroy, fd: %d, epoll_helper: %p \n",
            context, context->fd, context->epoll_helper);

    epoll_helper_destroy(&context->epoll_helper);

    close(context->fd);

    free(context);
    *context_pp = NULL;
}

socket_ipc_server_s *socket_ipc_server_create(const char *name,
        socket_ipc_accept_cb_t accept_cb, void *args)
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

        if (0 != _ipc_server_fd_create(context, name)) {
            log_error("_ipc_server_fd_create failed \n");
            break;
        }

        context->epoll_helper = epoll_helper_create("HY_EH_listen_fd",
                100, _epoll_handle_data);
        if (!context->epoll_helper) {
            log_error("epoll_helper_create failed \n");
            break;
        }

        context->list_fd_cb_param.fd = context->fd;
        context->list_fd_cb_param.args = context;
        epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, &context->list_fd_cb_param);

        context->accept_cb  = accept_cb;
        context->args       = args;

        log_info("socket ipc server context: %p create, fd: %d, epoll_helper: %p \n",
                context, context->fd, context->epoll_helper);
        return context;
    } while (0);

    log_error("socket ipc server context: %p create failed \n", context);
    socket_ipc_server_destroy(&context);
    return NULL;
}

