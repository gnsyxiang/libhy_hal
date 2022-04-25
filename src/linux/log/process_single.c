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

#include "fifo.h"
#include "dynamic_array.h"

#include "process_single.h"

typedef struct {
    hy_s32_t            is_exit;
    pthread_t           thread;

    fifo_context_s      *fifo;
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
} _process_single_context_s;

static _process_single_context_s _process_context;

void process_single_write(log_write_info_s *log_write_info)
{
    _process_single_context_s *context = &_process_context;
    HyLogAddiInfo_s *addi_info = log_write_info->addi_info;
    dynamic_array_s *dynamic_array = log_write_info->dynamic_array;

    for (hy_u32_t i = 0; i < log_write_info->format_log_cb_cnt; ++i) {
        if (log_write_info->format_log_cb[i][0]) {
            log_write_info->format_log_cb[i][0](dynamic_array, addi_info);
        }
    }

    pthread_mutex_lock(&context->mutex);
    fifo_write(context->fifo, dynamic_array->buf, dynamic_array->cur_len);
    pthread_mutex_unlock(&context->mutex);

    pthread_cond_signal(&context->cond);
}

static void *_thread_cb(void *args)
{
    _process_single_context_s *context = args;
    hy_u32_t len = 0;
    char buf[1024] = {0};

#ifdef _GNU_SOURCE
    pthread_setname_np(context->thread, "HY_async_log");
#endif

    while (!context->is_exit) {
        pthread_mutex_lock(&context->mutex);
        if (FIFO_IS_EMPTY(context->fifo)) {
            pthread_cond_wait(&context->cond, &context->mutex);
        }
        pthread_mutex_unlock(&context->mutex);

        memset(buf, '\0', sizeof(buf));
        len = fifo_read(context->fifo, buf, sizeof(buf));

        /* @fixme: <22-04-22, uos> 多种方式处理数据 */
        printf("%s", buf);
    }

    return NULL;
}

void process_single_destroy(void)
{
    _process_single_context_s *context = &_process_context;

    context->is_exit = 1;
    pthread_cond_signal(&context->cond);
    pthread_join(context->thread, NULL);

    pthread_mutex_destroy(&context->mutex);
    pthread_cond_destroy(&context->cond);

    fifo_destroy(&context->fifo);
}

hy_s32_t process_single_create(hy_u32_t fifo_len)
{
    _process_single_context_s *context = &_process_context;

    do {
        memset(context, '\0', sizeof(*context));

        if (0 != pthread_mutex_init(&context->mutex, NULL)) {
            log_error("pthread_mutex_init failed \n");
            break;
        }

        if (0 != pthread_cond_init(&context->cond, NULL)) {
            log_error("pthread_cond_init failed \n");
            break;
        }

        context->fifo = fifo_create(fifo_len);
        if (!context->fifo) {
            log_info("fifo_create failed \n");
            break;
        }

        if (0 != pthread_create(&context->thread, NULL, _thread_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

        return 0;
    } while (0);

    process_single_destroy();
    return -1;
}

