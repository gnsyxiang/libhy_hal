/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_ipc_client.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 13:58
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 13:58
 */
#include <stdio.h>
#include <stdlib.h>

#include "fifo_async.h"
#include "log_private.h"
#include "socket_ipc_client.h"

#include "process_ipc_client.h"

typedef struct {
    hy_s32_t                is_exit;
    pthread_t               thread;

    socket_ipc_client_s     *socket_ipc_client;
    fifo_async_s            *fifo_async;
} _process_ipc_client_context_s;

void process_ipc_client_write(void *handle, log_write_info_s *log_write_info)
{
    _process_ipc_client_context_s *context = handle;
    HyLogAddiInfo_s *addi_info = log_write_info->addi_info;
    dynamic_array_s *dynamic_array = log_write_info->dynamic_array;

    for (hy_u32_t i = 0; i < log_write_info->format_log_cb_cnt; ++i) {
        if (log_write_info->format_log_cb[i][0]) {
            log_write_info->format_log_cb[i][0](dynamic_array, addi_info);
        }
    }

    fifo_async_write(context->fifo_async,
            dynamic_array->buf, dynamic_array->cur_len);
}

static void *_thread_cb(void *args)
{
    _process_ipc_client_context_s *context = args;
    hy_s32_t len = 0;
    char buf[1024] = {0};

#ifdef _GNU_SOURCE
    pthread_setname_np(context->thread, "HY_async_log");
#endif

    while (!context->is_exit) {
        len = fifo_async_read(context->fifo_async, buf, sizeof(buf));
        if (len > 0) {
            /* @fixme: <22-04-22, uos> 多种方式处理数据 */
            printf("%s", buf);
        }
    }

    return NULL;
}


void process_ipc_client_destroy(void **handle)
{
    if (!handle || !*handle) {
        log_error("the param is error \n");
        return;
    }
    _process_ipc_client_context_s *context = *handle;

    while (!fifo_async_is_empty(context->fifo_async)) {
        usleep(100 * 1000);
    }
    context->is_exit = 1;

    fifo_async_destroy(&context->fifo_async);
    pthread_join(context->thread, NULL);

    free(context);
    *handle = NULL;
}

void *process_ipc_client_create(hy_u32_t fifo_len)
{
    if (fifo_len <= 0) {
        log_error("the param is error \n");
        return NULL;
    }

    _process_ipc_client_context_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->fifo_async = fifo_async_create(fifo_len);
        if (!context->fifo_async) {
            log_error("fifo_async_create failed \n");
            break;
        }

        if (0 != pthread_create(&context->thread, NULL, _thread_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

        context->socket_ipc_client = socket_ipc_client_create(LOG_IPC_NAME);
        if (!context->socket_ipc_client) {
            log_error("socket_ipc_client_create failed \n");
            break;
        }

        return context;
    } while (0);

    process_ipc_client_destroy((void *)&context);
    return NULL;
}
