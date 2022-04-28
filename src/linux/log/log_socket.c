/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    log_socket.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 15:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 15:51
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#include "log_private.h"
#include "epoll_helper.h"

#include "log_socket.h"

static void _epoll_handle_data(epoll_helper_cb_param_s *cb_param)
{
    hy_s32_t fd;
    log_socket_context_s *context = cb_param->args;

    fd = accept(context->fd, NULL, NULL);
    if (fd < 0) {
        log_error("accept failed, fd: %d \n", context->fd);
        return;
    }
    log_info("new socket client fd: %d \n", fd);

    if (context->accept_cb) {
        context->accept_cb(fd, context->args);
    }

    epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, cb_param);
}

void log_socket_destroy(log_socket_context_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is NULL \n");
        return;
    }

    log_socket_context_s *context = *context_pp;

    epoll_helper_destroy(&context->epoll_helper);

    close(context->fd);

    free(context->cb_param);

    free(context);
    *context_pp = NULL;
}

log_socket_context_s *log_socket_create(hy_u16_t port,
        log_socket_accept_cb_t accept_cb, void *args)
{
    if (!accept_cb) {
        log_error("the param is NULL \n");
        return NULL;
    }

    log_socket_context_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }
        context->accept_cb = accept_cb;
        context->args = args;

        context->cb_param = calloc(1, sizeof(epoll_helper_cb_param_s));
        if (!context->cb_param) {
            log_error("calloc failed \n");
            break;
        }

        context->fd = socket(AF_INET, SOCK_STREAM, 0);
        if (context->fd < 0) {
            log_error("socket faild \n");
            break;
        }

        int opt = SO_REUSEADDR;
        setsockopt(context->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in server;
        memset(&server, '\0', sizeof(server));
        server.sin_family       = AF_INET;
        server.sin_port         = htons(port);
        server.sin_addr.s_addr  = htonl(INADDR_ANY);

        if (bind(context->fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
            log_error("bind failed \n");
            break;
        }

        if (listen(context->fd, 10) == -1) {
            log_error("listen failed \n");
            break;
        }

        context->epoll_helper = epoll_helper_create("HY_SV_socket", 100,
                _epoll_handle_data);

        context->cb_param->fd = context->fd;
        context->cb_param->args = context;

        epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, context->cb_param);

        return context;
    } while (0);

    return NULL;
}

