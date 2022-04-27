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

#include "fifo_async.h"
#include "log_private.h"
#include "socket_ipc_server.h"
#include "hy_list.h"

#include "process_ipc_server.h"

typedef struct {
    hy_s32_t                fd;
    pthread_t               id;
    hy_s32_t                is_exit;
    void                    *args;

    struct hy_list_head     entry;
} _socket_node_s;

typedef struct {
    hy_s32_t                is_terminal_exit;
    pthread_t               terminal_thread_id;
    fifo_async_s            *terminal_fifo_async;

    hy_s32_t                is_tcp_exit;
    pthread_t               tcp_thread_id;
    fifo_async_s            *tcp_fifo_async;

    socket_ipc_server_s     *socket_ipc_server;

    struct hy_list_head     list;
} _process_ipc_server_context_s;

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

    fifo_async_write(context->terminal_fifo_async,
            dynamic_array->buf, dynamic_array->cur_len);

    DYNAMIC_ARRAY_RESET(dynamic_array);

    for (hy_u32_t i = 0; i < log_write_info->format_log_cb_cnt; ++i) {
        if (log_write_info->format_log_cb[i][1]) {
            log_write_info->format_log_cb[i][1](dynamic_array, addi_info);
        }
    }

    fifo_async_write(context->tcp_fifo_async,
            dynamic_array->buf, dynamic_array->cur_len);
}

static void *_read_socket_data_cb(void *args)
{
    hy_s32_t ret = 0;
    _socket_node_s *socket_node = args;
    _process_ipc_server_context_s *context = socket_node->args;
    char buf[1024] = {0};

    while (!socket_node->is_exit) {
        memset(buf, '\0', sizeof(buf));
        ret = read(socket_node->fd, buf, sizeof(buf));
        if (ret < 0) {
            if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
            } else {
                log_error("read failed \n");
                break;
            }
        } else if (ret == 0) {
            log_error("fd close, fd: %d \n", socket_node->fd);
            break;
        } else {
            fifo_async_write(context->tcp_fifo_async, buf, ret);
        }
    }

    return NULL;
}

static void _accept_cb(hy_s32_t fd, void *args)
{
    _process_ipc_server_context_s *context = args;

    _socket_node_s *socket_node = calloc(1, sizeof(*socket_node));
    if (!socket_node) {
        log_error("calloc failed \n");
        return;
    }

    socket_node->fd = fd;
    socket_node->args = args;

    if (0 != pthread_create(&socket_node->id, NULL,
                _read_socket_data_cb, socket_node)) {
        log_error("pthread_create failed \n");
        return;
    }

    hy_list_add_tail(&socket_node->entry, &context->list);
}

static void *_tcp_msg_cb(void *args)
{
    _process_ipc_server_context_s *context = args;
    hy_s32_t len = 0;
    char buf[1024] = {0};

#ifdef _GNU_SOURCE
    pthread_setname_np(context->terminal_thread_id, "HY_async_log");
#endif

    while (!context->is_tcp_exit) {
        len = fifo_async_read(context->tcp_fifo_async, buf, sizeof(buf));
        if (len > 0) {
            /* @fixme: <22-04-22, uos> 多种方式处理数据 */
            printf("%s", buf);
        }
    }

    return NULL;
}

static void *_terminal_msg_cb(void *args)
{
    _process_ipc_server_context_s *context = args;
    hy_s32_t len = 0;
    char buf[1024] = {0};

#ifdef _GNU_SOURCE
    pthread_setname_np(context->terminal_thread_id, "HY_async_log");
#endif

    while (!context->is_terminal_exit) {
        len = fifo_async_read(context->terminal_fifo_async, buf, sizeof(buf));
        if (len > 0) {
            /* @fixme: <22-04-22, uos> 多种方式处理数据 */
            // printf("%s", buf);
        }
    }

    return NULL;
}

void process_ipc_server_destroy(void **handle)
{
    _process_ipc_server_context_s *context = *handle;
    log_info("process ipc server context: %p destroy \n", context);

    while (!fifo_async_is_empty(context->terminal_fifo_async)) {
        usleep(100 * 1000);
    }
    context->is_terminal_exit = 1;
    fifo_async_destroy(&context->terminal_fifo_async);
    pthread_join(context->terminal_thread_id, NULL);

    while (!fifo_async_is_empty(context->tcp_fifo_async)) {
        usleep(100 * 1000);
    }
    context->is_tcp_exit = 1;
    fifo_async_destroy(&context->tcp_fifo_async);
    pthread_join(context->tcp_thread_id, NULL);

    _socket_node_s *pos, *n;
    hy_list_for_each_entry_safe(pos, n, &context->list, entry) {
        hy_list_del(&pos->entry);

        pos->is_exit = 1;
        pthread_join(pos->id, NULL);

        close(pos->fd);
        free(pos);
    }

    socket_ipc_server_destroy(&context->socket_ipc_server);

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

        context->terminal_fifo_async = fifo_async_create(fifo_len);
        if (!context->terminal_fifo_async) {
            log_error("fifo_async_create failed \n");
            break;
        }

        context->tcp_fifo_async = fifo_async_create(fifo_len);
        if (!context->tcp_fifo_async) {
            log_error("fifo_async_create failed \n");
            break;
        }

        if (0 != pthread_create(&context->terminal_thread_id,
                    NULL, _terminal_msg_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

        if (0 != pthread_create(&context->tcp_thread_id,
                    NULL, _tcp_msg_cb, context)) {
            log_error("pthread_create failed \n");
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

    log_error("process ipc server create failed \n");
    process_ipc_server_destroy((void **)&context);
    return NULL;
}

