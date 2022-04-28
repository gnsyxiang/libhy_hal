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

#include "format_cb.h"
#include "log_private.h"
#include "socket_ipc_client.h"
#include "process_handle_data.h"

#include "process_ipc_client.h"

typedef struct {
    process_handle_data_s   *terminal_handle_data;

    socket_ipc_client_s     *socket_ipc_client;
} _process_ipc_client_context_s;

void process_ipc_client_write(void *handle, log_write_info_s *log_write_info)
{
    _process_ipc_client_context_s *context = handle;
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

    socket_ipc_client_write(context->socket_ipc_client,
            dynamic_array->buf, dynamic_array->cur_len);
}

static void _terminal_process_handle_data_cb(void *buf,
        hy_u32_t len, void *args)
{
    printf("%s", (char *)buf);
}

void process_ipc_client_destroy(void **handle)
{
    if (!handle || !*handle) {
        log_error("the param is error \n");
        return;
    }
    _process_ipc_client_context_s *context = *handle;
    log_info("process ipc client context: %p destroy, "
            "terminal_handle_data: %p, socket_ipc_client: %p \n",
            context, context->terminal_handle_data,
            context->socket_ipc_client);

    process_handle_data_destroy(&context->terminal_handle_data);

    socket_ipc_client_destroy(&context->socket_ipc_client);

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

        context->terminal_handle_data = process_handle_data_create("HY_CL_terminal",
                fifo_len, _terminal_process_handle_data_cb, context);
        if (!context->terminal_handle_data) {
            log_error("process_handle_data_create failed \n");
            break;
        }

        context->socket_ipc_client = socket_ipc_client_create(LOG_IPC_NAME);
        if (!context->socket_ipc_client) {
            log_error("socket_ipc_client_create failed \n");
            break;
        }

        log_info("process ipc client context: %p create, "
                "terminal_handle_data: %p, socket_ipc_client: %p \n",
                context, context->terminal_handle_data,
                context->socket_ipc_client);
        return context;
    } while (0);

    log_error("process ipc client context: %p create failed \n", context);
    process_ipc_client_destroy((void *)&context);
    return NULL;
}

