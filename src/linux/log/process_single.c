/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_single.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/04 2022 14:36
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/04 2022      create the file
 * 
 *     last modified: 21/04 2022 14:36
 */
#include <stdio.h>

#include "format_cb.h"
#include "log_private.h"
#include "process_handle_data.h"

#include "process_single.h"

typedef struct {
    process_handle_data_s   *terminal_handle_data;
} _process_single_context_s;

void process_single_write(void *handle, log_write_info_s *log_write_info)
{
    _process_single_context_s *context = handle;
    HyLogAddiInfo_s *addi_info = log_write_info->addi_info;
    dynamic_array_s *dynamic_array = log_write_info->dynamic_array;

    for (hy_u32_t i = 0; i < log_write_info->format_cb_cnt; ++i) {
        if (log_write_info->format_cb[i][0]) {
            log_write_info->format_cb[i][0](dynamic_array, addi_info);
        }
    }

    process_handle_data_write(context->terminal_handle_data,
            dynamic_array->buf, dynamic_array->cur_len);
}

static void _terminal_process_handle_data_cb(void *buf, hy_u32_t len, void *args)
{
    printf("%s", (char *)buf);
}

void process_single_destroy(void **handle)
{
    _process_single_context_s *context = *handle;
    log_info("process single context: %p destroy \n", context);

    process_handle_data_destroy(&context->terminal_handle_data);

    free(context);
    *handle = NULL;
}

void *process_single_create(hy_u32_t fifo_len)
{
    if (fifo_len <= 0) {
        log_error("the param is error \n");
        return NULL;
    }

    _process_single_context_s *context = NULL;

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

        log_info("process single context: %p create \n", context);
        return context;
    } while (0);

    log_error("process single create failed \n");
    process_single_destroy((void **)&context);
    return NULL;
}

