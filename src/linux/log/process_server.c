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
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include "hy_list.h"
#include "log_file.h"
#include "log_file.h"
#include "log_epoll.h"
#include "format_cb.h"
#include "log_socket.h"
#include "log_socket.h"
#include "log_private.h"
#include "socket_fd_node.h"
#include "process_handle_data.h"

#include "process_server.h"

typedef struct {
    log_epoll_s             *log_epoll;
    socket_fd_node_s        *socket_listen_fd;
    socket_fd_node_s        *socket_ipc_listen_fd;

    struct hy_list_head     list;
    pthread_mutex_t         list_mutex;

    struct hy_list_head     socket_list;
    pthread_mutex_t         socket_list_mutex;
    process_handle_data_s   *tcp_handle_data;
    process_handle_data_s   *terminal_handle_data;
    pthread_mutex_t         terminal_mutex;
} _process_server_context_s;

void process_server_write(void *handle, log_write_info_s *log_write_info)
{
    _process_server_context_s *context = handle;
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

static void _accept_client_fd(log_epoll_cb_param_s *cb_param,
        hy_s32_t type, struct hy_list_head *list, pthread_mutex_t *mutex)
{
    hy_s32_t fd;
    socket_fd_node_s *socket_fd_node = NULL;
    _process_server_context_s *context = cb_param->args;

    do {
        fd = accept(cb_param->fd, NULL, NULL);
        if (fd < 0) {
            log_error("accept failed, fd: %d \n", fd);
            break;
        }

        if (0 != log_epoll_add(context->log_epoll,
                    EPOLLIN | EPOLLET, cb_param)) {
            log_error("log_epoll_add add listen fd failed \n");
            break;
        }

        socket_fd_node = socket_fd_node_create(fd, type, context);
        if (!socket_fd_node) {
            log_error("socket_fd_node_create failed \n");
            break;
        }

        if (0 != log_epoll_add(context->log_epoll,
                    EPOLLIN | EPOLLET, &socket_fd_node->cb_param)) {
            log_error("log_epoll_add add client fd failed \n");
            break;
        }

        pthread_mutex_lock(mutex);
        hy_list_add_tail(&socket_fd_node->entry, list);
        pthread_mutex_unlock(mutex);
    } while (0);
}

static void _epoll_handle_data(log_epoll_cb_param_s *cb_param)
{
    _process_server_context_s *context = cb_param->args;
    hy_s32_t ret = 0;
    char buf[1024] = {0};

    switch (cb_param->type) {
        case LOG_SOCKET_TYPE_SERVER:
            _accept_client_fd(cb_param, LOG_SOCKET_TYPE_CLIENT,
                    &context->socket_list, &context->socket_list_mutex);
            break;
        case LOG_SOCKET_TYPE_IPC_SERVER:
            _accept_client_fd(cb_param, LOG_SOCKET_TYPE_IPC_CLIENT,
                    &context->list, &context->list_mutex);
            break;
        case LOG_SOCKET_TYPE_IPC_CLIENT:
            ret = log_file_read(cb_param->fd, buf, sizeof(buf));
            if (ret > 0) {
                process_handle_data_write(context->tcp_handle_data, buf, ret);
                log_epoll_add(context->log_epoll, EPOLLIN | EPOLLET, cb_param);
            } else {
                socket_fd_node_list_destroy(&context->list, cb_param->fd);
            }
            break;
        default:
            log_error("error type \n");
            break;
    }
}

static void _tcp_process_handle_data_cb(void *buf, hy_u32_t len, void *args)
{
    socket_fd_node_s *pos, *n;
    hy_s32_t ret = 0;
    _process_server_context_s *context = args;

    pthread_mutex_lock(&context->terminal_mutex);
    printf("%s", (char *)buf);
    pthread_mutex_unlock(&context->terminal_mutex);

    pthread_mutex_lock(&context->socket_list_mutex);
    hy_list_for_each_entry_safe(pos, n, &context->socket_list, entry) {
        ret = log_file_write(pos->cb_param.fd, buf, len);
        if (ret < 0) {
            log_info("the other party closes, fd: %d \n", pos->cb_param.fd);

            hy_list_del(&pos->entry);
            log_epoll_del(context->log_epoll, &pos->cb_param);
            socket_fd_node_destroy(&pos);
        }
    }
    pthread_mutex_unlock(&context->socket_list_mutex);
}

static void _terminal_process_handle_data_cb(void *buf, hy_u32_t len, void *args)
{
    return;
    _process_server_context_s *context = args;

    pthread_mutex_lock(&context->terminal_mutex);
    printf("%s", (char *)buf);
    pthread_mutex_unlock(&context->terminal_mutex);
}

void process_server_destroy(void **handle)
{
    if (!handle || !*handle) {
        log_error("the param is NULL \n");
        return;
    }
    _process_server_context_s *context = *handle;
    log_info("process ipc server context: %p destroy \n", context);

    log_epoll_destroy(&context->log_epoll);

    socket_fd_node_destroy(&context->socket_ipc_listen_fd);
    socket_fd_node_destroy(&context->socket_listen_fd);

    process_handle_data_destroy(&context->terminal_handle_data);
    process_handle_data_destroy(&context->tcp_handle_data);

    socket_fd_node_list_destroy(&context->socket_list, -1);
    socket_fd_node_list_destroy(&context->list, -1);

    pthread_mutex_destroy(&context->terminal_mutex);
    pthread_mutex_destroy(&context->socket_list_mutex);
    pthread_mutex_destroy(&context->list_mutex);

    free(context);
    *handle = NULL;
}

void *process_server_create(hy_u32_t fifo_len)
{
    if (fifo_len <= 0) {
        log_error("the param is error \n");
        return NULL;
    }

    _process_server_context_s *context = NULL;
    hy_s32_t fd = -1;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        HY_INIT_LIST_HEAD(&context->list);
        HY_INIT_LIST_HEAD(&context->socket_list);

        if (0 != pthread_mutex_init(&context->terminal_mutex, NULL)) {
            log_error("pthread_mutex_init failed \n");
            break;
        }

        if (0 != pthread_mutex_init(&context->socket_list_mutex, NULL)) {
            log_error("pthread_mutex_init failed \n");
            break;
        }

        if (0 != pthread_mutex_init(&context->list_mutex, NULL)) {
            log_error("pthread_mutex_init failed \n");
            break;
        }

        context->log_epoll = log_epoll_create("hy_server_epoll",
                100, _epoll_handle_data);
        if (!context->log_epoll) {
            log_error("log_epoll_create failed \n");
            break;
        }

        fd = log_socket_ipc_create(LOG_SOCKET_IPC_NAME,
                LOG_SOCKET_TYPE_IPC_SERVER);
        if (fd < 0) {
            log_error("log_socket_ipc_create failed \n");
            break;
        }

        context->socket_ipc_listen_fd = socket_fd_node_create(fd,
                LOG_SOCKET_TYPE_IPC_SERVER, context);
        if (!context->socket_ipc_listen_fd) {
            log_error("socket_fd_node_create failed \n");
            break;
        }

        if (0 != log_epoll_add(context->log_epoll,
                EPOLLIN | EPOLLET, &context->socket_ipc_listen_fd->cb_param)) {
            log_error("log_epoll_add failed \n");
            break;
        }

        fd = log_socket_create("127.0.0.1",
                LOG_SOCKET_PORT, LOG_SOCKET_TYPE_SERVER);
        if (fd < 0) {
            log_error("log_socket_create failed \n");
            break;
        }

        context->socket_listen_fd = socket_fd_node_create(fd,
                LOG_SOCKET_TYPE_SERVER, context);
        if (!context->socket_listen_fd) {
            log_error("socket_fd_node_create failed \n");
            break;
        }

        if (0 != log_epoll_add(context->log_epoll,
                EPOLLIN | EPOLLET, &context->socket_listen_fd->cb_param)) {
            log_error("log_epoll_add failed \n");
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

        log_info("process ipc server context: %p create \n", context);
        return context;
    } while (0);

    log_error("process ipc server context: %p create failed \n", context);
    process_server_destroy((void **)&context);
    return NULL;
}

