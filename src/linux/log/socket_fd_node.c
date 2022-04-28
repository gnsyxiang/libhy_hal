/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    socket_fd_node.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 15:18
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 15:18
 */
#include <stdio.h>
#include <unistd.h>

#include "log_private.h"
#include "epoll_helper.h"

#include "socket_fd_node.h"

void socket_fd_node_list_destroy(struct hy_list_head *list, hy_s32_t fd)
{
    socket_fd_node_s *pos, *n;

    if (fd > 0) {
        hy_list_for_each_entry_safe(pos, n, list, entry) {
            if (fd == pos->cb_param.fd) {
                hy_list_del(&pos->entry);
                socket_fd_node_destroy(&pos);
                break;
            }
        }
    } else {
        hy_list_for_each_entry_safe(pos, n, list, entry) {
            hy_list_del(&pos->entry);

            socket_fd_node_destroy(&pos);
        }
    }
}

void socket_fd_node_destroy(socket_fd_node_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is NULL \n");
        return;
    }
    socket_fd_node_s *context = *context_pp;
    log_info("socket fd node context: %p destroy, fd: %d \n",
            context, context->cb_param.fd);

    close(context->cb_param.fd);

    free(context);
    *context_pp = NULL;
}

socket_fd_node_s *socket_fd_node_create(hy_s32_t fd, hy_s32_t type, void *args)
{
    socket_fd_node_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->cb_param.fd    = fd;
        context->cb_param.type  = type;
        context->cb_param.args  = args;

        log_info("socket fd node context: %p create, fd: %d \n",
                context, context->cb_param.fd);
        return context;
    } while (0);

    log_error("socket fd node context: %p create failed \n", context);
    socket_fd_node_destroy(&context);
    return NULL;
}

