/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_server.c
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
#include <unistd.h>

#include "hy_list.h"
#include "log_file.h"
#include "log_file.h"
#include "format_cb.h"
#include "log_socket.h"
#include "log_private.h"
#include "epoll_helper.h"
#include "socket_fd_node.h"
#include "process_handle_data.h"

#include "process_server.h"

typedef struct {
    epoll_helper_s          *epoll_helper;
    socket_fd_node_s        *socket_listen_fd;
    socket_fd_node_s        *socket_ipc_listen_fd;

    struct hy_list_head     list;
    struct hy_list_head     socket_list;

    socket_ipc_server_s     *socket_ipc_server;

    log_socket_context_s    *log_sockt;

    process_handle_data_s   *tcp_handle_data;
    process_handle_data_s   *terminal_handle_data;
} _process_ipc_server_context_s;

static void _log_socket_accept_cb(hy_s32_t fd, void *args)
{
    _process_ipc_server_context_s *context = args;
    socket_node_fd_s *socket_node_fd = socket_node_fd_create(fd, args);

    hy_list_add_tail(&socket_node_fd->entry, &context->socket_list);
}

void process_ipc_server_write(void *handle, log_write_info_s *log_write_info)
{
    _process_ipc_server_context_s *context = handle;
    HyLogAddiInfo_s *addi_info = log_write_info->addi_info;
    dynamic_array_s *dynamic_array = log_write_info->dynamic_array;

    for (hy_u32_t i = 0; i < log_write_info->format_cb_cnt; ++i) {
        if (log_write_info->format_cb[i][0]) {
            log_write_info->format_cb[i][0](dynamic_array, addi_info);
        }
    }

    process_handle_data_write(context->terminal_handle_data,
            dynamic_array->buf, dynamic_array->cur_len);

    DYNAMIC_ARRAY_RESET(dynamic_array);

    for (hy_u32_t i = 0; i < log_write_info->format_cb_cnt; ++i) {
        if (log_write_info->format_cb[i][1]) {
            log_write_info->format_cb[i][1](dynamic_array, addi_info);
        }
    }

    process_handle_data_write(context->tcp_handle_data,
            dynamic_array->buf, dynamic_array->cur_len);
}

static void _epoll_handle_data(epoll_helper_cb_param_s *cb_param)
{
    _process_ipc_server_context_s *context = cb_param->args;
    hy_s32_t ret = 0;
    char buf[1024] = {0};

    ret = log_file_read(&cb_param->fd, buf, sizeof(buf));
    if (ret > 0) {
        process_handle_data_write(context->tcp_handle_data, buf, ret);
        epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, cb_param);
    } else {
        socket_node_fd_list_destroy(&context->list, cb_param->fd);
    }
}

static void _accept_cb(hy_s32_t fd, void *args)
{
    _process_ipc_server_context_s *context = args;
    socket_node_fd_s *socket_node_fd = socket_node_fd_create(fd, args);

    epoll_helper_set(context->epoll_helper, EPOLLIN | EPOLLET, socket_node_fd->cb_param);
    hy_list_add_tail(&socket_node_fd->entry, &context->list);
}

static void _tcp_process_handle_data_cb(void *buf, hy_u32_t len, void *args)
{
    printf("%s", (char *)buf);

    socket_node_fd_s *pos;
    _process_ipc_server_context_s *context = args;

    hy_list_for_each_entry(pos, &context->socket_list, entry) {
        log_file_write(&pos->cb_param->fd, buf, len);
    }
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

    log_socket_destroy(&context->log_sockt);

    process_handle_data_destroy(&context->terminal_handle_data);
    process_handle_data_destroy(&context->tcp_handle_data);

    socket_node_fd_list_destroy(&context->socket_list, -1);

    socket_node_fd_list_destroy(&context->list, -1);

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
    hy_s32_t fd = -1;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        HY_INIT_LIST_HEAD(&context->list);
        HY_INIT_LIST_HEAD(&context->socket_list);

        context->epoll_helper = epoll_helper_create("HY_EH_new_fd",
                100, _epoll_handle_data);
        if (!context->epoll_helper) {
            log_error("epoll_helper_create failed \n");
            break;
        }

        fd = log_socket_ipc_create(LOG_SOCKET_IPC_NAME, LOG_SOCKET_IPC_TYPE_SERVER);
        if (fd < 0) {
            log_error("log_socket_ipc_create failed \n");
            break;
        }

        context->socket_ipc_listen_fd = socket_fd_node_create(fd, );

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

        context->log_sockt = log_socket_create(8080,
                _log_socket_accept_cb, context);
        if (!context->log_sockt) {
            log_error("log_socket_create failed \n");
            break;
        }

        log_info("process ipc server context: %p create \n", context);
        return context;
    } while (0);

    log_error("process ipc server context: %p create failed \n", context);
    process_ipc_server_destroy((void **)&context);
    return NULL;
}

