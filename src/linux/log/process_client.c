/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_client.c
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
#include "log_file.h"
#include "log_socket.h"
#include "process_handle_data.h"

#include "process_client.h"

typedef struct {
    process_handle_data_s   *terminal_handle_data;

    hy_s32_t                fd;
} _process_ipc_client_context_s;

void process_client_write(void *handle, log_write_info_s *log_write_info)
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

    if (context->fd > 0) {
        if (log_file_write(context->fd,
                    dynamic_array->buf, dynamic_array->cur_len) < 0) {
            log_info("the other party closes \n");

            close(context->fd);
            context->fd = -1;
        }
    }
}

static void _terminal_process_handle_data_cb(void *buf,
        hy_u32_t len, void *args)
{
    printf("%s", (char *)buf);
}

void process_client_destroy(void **handle)
{
    if (!handle || !*handle) {
        log_error("the param is error \n");
        return;
    }
    _process_ipc_client_context_s *context = *handle;
    log_info("process ipc client context: %p destroy, "
            "terminal_handle_data: %p, fd: %d \n",
            context, context->terminal_handle_data,
            context->fd);

    process_handle_data_destroy(&context->terminal_handle_data);

    close(context->fd);

    free(context);
    *handle = NULL;
}

void *process_client_create(hy_u32_t fifo_len)
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

        context->fd = log_socket_ipc_create(LOG_SOCKET_IPC_NAME,
                LOG_SOCKET_TYPE_CLIENT);
        if (context->fd < 0) {
            log_error("log_socket_ipc_create failed \n");
            break;
        }

        log_info("process ipc client context: %p create, "
                "terminal_handle_data: %p, fd: %d \n",
                context, context->terminal_handle_data, context->fd);
        return context;
    } while (0);

    log_error("process ipc client context: %p create failed \n", context);
    process_client_destroy((void *)&context);
    return NULL;
}

