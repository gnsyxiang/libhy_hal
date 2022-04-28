/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_ipc_server.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/04 2022 17:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/04 2022      create the file
 * 
 *     last modified: 25/04 2022 17:27
 */
#include <stdio.h>
#include <stdlib.h>

#include "log_private.h"
#include "socket_ipc_server.h"
#include "hy_list.h"
#include "epoll_helper.h"
#include "process_handle_data.h"

#include "process_ipc_server.h"

typedef struct {
    epoll_helper_cb_param_s cb_param;
    struct hy_list_head     entry;
} _socket_node_s;

typedef struct {
    socket_ipc_server_s     *socket_ipc_server;
    epoll_helper_s          *epoll_helper;
    struct hy_list_head     list;

    process_handle_data_s   *tcp_handle_data;
    process_handle_data_s   *terminal_handle_data;
} _process_ipc_server_context_s;

static inline void socket_node_destroy(_socket_node_s **socket_node_pp)
{
    _socket_node_s *socket_node = *socket_node_pp;

    close(socket_node->cb_param.fd);

    free(socket_node);
    *socket_node_pp = NULL;
}

static inline _socket_node_s *socket_node_create(hy_s32_t fd, void *args)
{
    _socket_node_s *socket_node = calloc(1, sizeof(*socket_node));
    if (!socket_node) {
        log_error("calloc failed \n");
        return NULL;
    }
    socket_node->cb_param.fd = fd;
    socket_node->cb_param.args = args;

    return socket_node;
}

static void socket_node_list_destroy(_process_ipc_server_context_s *context,
        hy_s32_t fd)
{
    _socket_node_s *pos, *n;

    if (fd > 0) {
        hy_list_for_each_entry_safe(pos, n, &context->list, entry) {
            if (fd == pos->cb_param.fd) {
                hy_list_del(&pos->entry);
                socket_node_destroy(&pos);
                break;
            }
        }
    } else {
        hy_list_for_each_entry_safe(pos, n, &context->list, entry) {
            hy_list_del(&pos->entry);

            socket_node_destroy(&pos);
        }
    }
}

void process_ipc_server_write(void *handle, log_write_info_s *log_write_info)
{
    _process_ipc_server_context_s *context = handle;
    HyLogAddiInfo_s *addi_info = log_write_info->addi_info;
    dynamic_array_s *dynamic_array = log_write_info->dynamic_array;

    for (hy_u32_t i = 0; i < log_write_info->format_log_cb_cnt; ++i) {
        if (log_write_info->format_log_cb[i][0]) {
            log_write_info->format_log_cb[i][0](dynamic_array, addi_info);
        }
    }

    process_handle_data_write(context->terminal_handle_data,
            dynamic_array->buf, dynamic_array->cur_len);

    DYNAMIC_ARRAY_RESET(dynamic_array);

    for (hy_u32_t i = 0; i < log_write_info->format_log_cb_cnt; ++i) {
        if (log_write_info->format_log_cb[i][1]) {
            log_write_info->format_log_cb[i][1](dynamic_array, addi_info);
        }
    }

    process_handle_data_write(context->tcp_handle_data,
            dynamic_array->buf, dynamic_array->cur_len);
}

static void _epoll_handle_data(epoll_helper_cb_param_s *cb_param)
{
    _process_ipc_server_context_s *context = cb_param->args;
    hy_s32_t ret = 0;
    hy_s32_t flag = 0;
    char buf[1024] = {0};

    while (1) {
        ret = read(cb_param->fd, buf, sizeof(buf));
        if (ret < 0) {
            if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
            } else {
                log_error("read failed, fd: %d \n", cb_param->fd);
                flag = 1;
                break;
            }
        } else if (ret == 0) {
            log_error("fd close, fd: %d \n", cb_param->fd);
            flag = 1;
            break;
        } else {
            process_handle_data_write(context->tcp_handle_data, buf, ret);
            epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, cb_param);
            break;
        }
    }

    if (flag) {
        socket_node_list_destroy(context, cb_param->fd);
    }
}

static void _accept_cb(hy_s32_t fd, void *args)
{
    _process_ipc_server_context_s *context = args;
    _socket_node_s *socket_node = socket_node_create(fd, args);

    epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, &socket_node->cb_param);
    hy_list_add_tail(&socket_node->entry, &context->list);
}

static void _tcp_process_handle_data_cb(void *buf, hy_u32_t len, void *args)
{
    printf("%s", (char *)buf);
}

static void _terminal_process_handle_data_cb(void *buf, hy_u32_t len, void *args)
{
    // printf("%s", (char *)buf);
}

void process_ipc_server_destroy(void **handle)
{
    _process_ipc_server_context_s *context = *handle;
    log_info("process ipc server context: %p destroy \n", context);

    socket_ipc_server_destroy(&context->socket_ipc_server);

    epoll_helper_destroy(&context->epoll_helper);

    process_handle_data_destroy(&context->terminal_handle_data);
    process_handle_data_destroy(&context->tcp_handle_data);

    socket_node_list_destroy(context, -1);

    free(context);
    *handle = NULL;
}

void *process_ipc_server_create(hy_u32_t fifo_len)
{
    if (fifo_len <= 0) {
        log_error("the param is error \n");
        return NULL;
    }

    _process_ipc_server_context_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        HY_INIT_LIST_HEAD(&context->list);

        context->epoll_helper = epoll_helper_create("HY_EH_new_fd",
                100, _epoll_handle_data);
        if (!context->epoll_helper) {
            log_error("epoll_helper_create failed \n");
            break;
        }

        context->terminal_handle_data = process_handle_data_create("HY_SV_terminal",
                fifo_len, _terminal_process_handle_data_cb, context);
        if (!context->terminal_handle_data) {
            log_error("process_handle_data_create failed \n");
            break;
        }

        context->tcp_handle_data = process_handle_data_create("HY_SV_TCP",
                fifo_len, _tcp_process_handle_data_cb, context);
        if (!context->tcp_handle_data) {
            log_error("process_handle_data_create failed \n");
            break;
        }

        context->socket_ipc_server = socket_ipc_server_create(LOG_IPC_NAME,
                _accept_cb, context);
        if (!context->socket_ipc_server) {
            log_error("socket_ipc_server_create failed \n");
            break;
        }

        log_info("process ipc server context: %p create \n", context);
        return context;
    } while (0);

    log_error("process ipc server context: %p create failed \n", context);
    process_ipc_server_destroy((void **)&context);
    return NULL;
}

