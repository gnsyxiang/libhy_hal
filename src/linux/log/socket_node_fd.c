/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    socket_node_fd.c
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

#include "socket_node_fd.h"

void socket_node_fd_list_destroy(struct hy_list_head *list, hy_s32_t fd)
{
    socket_node_fd_s *pos, *n;

    if (fd > 0) {
        hy_list_for_each_entry_safe(pos, n, list, entry) {
            if (fd == pos->cb_param->fd) {
                hy_list_del(&pos->entry);
                socket_node_fd_destroy(&pos);
                break;
            }
        }
    } else {
        hy_list_for_each_entry_safe(pos, n, list, entry) {
            hy_list_del(&pos->entry);

            socket_node_fd_destroy(&pos);
        }
    }
}

void socket_node_fd_destroy(socket_node_fd_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is NULL \n");
        return;
    }
    socket_node_fd_s *context = *context_pp;
    log_info("socket node fd context: %p destroy, fd: %d \n",
            context, context->cb_param->fd);

    close(context->cb_param->fd);

    free(context->cb_param);

    free(context);
    *context_pp = NULL;
}

socket_node_fd_s *socket_node_fd_create(hy_s32_t fd, void *args)
{
    socket_node_fd_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->cb_param = calloc(1, sizeof(epoll_helper_cb_param_s));
        if (!context->cb_param) {
            log_error("calloc failed \n");
            break;
        }

        context->cb_param->fd = fd;
        context->cb_param->args = args;

        log_info("socket node fd context: %p create, fd: %d \n",
                context, context->cb_param->fd);
        return context;
    } while (0);

    log_error("socket node fd context: %p create failed \n", context);
    socket_node_fd_destroy(&context);
    return NULL;
}

