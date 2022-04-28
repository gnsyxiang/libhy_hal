/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_handle_data.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 08:38
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 08:38
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_private.h"

#include "process_handle_data.h"

hy_s32_t process_handle_data_write(process_handle_data_s *context,
        const void *buf, hy_u32_t len)
{
    assert(context);
    assert(buf);
    hy_s32_t ret = 0;

    pthread_mutex_lock(&context->mutex);
    ret = log_fifo_write(context->fifo, buf, len);
    pthread_mutex_unlock(&context->mutex);

    pthread_cond_signal(&context->cond);

    return ret;
}

static void *_thread_cb(void *args)
{
    #define _ITEM_LEN_MAX   (4 * 1024)
    process_handle_data_s *context = args;
    hy_s32_t ret = 0;

    char *buf = calloc(1, _ITEM_LEN_MAX);
    if (!buf) {
        log_error("calloc failed \n");
        return NULL;
    }

    while (!context->is_exit) {
        pthread_mutex_lock(&context->mutex);
        if (LOG_FIFO_IS_EMPTY(context->fifo)) {
            pthread_cond_wait(&context->cond, &context->mutex);
        }
        pthread_mutex_unlock(&context->mutex);

        memset(buf, '\0', _ITEM_LEN_MAX);
        ret = log_fifo_read(context->fifo, buf, _ITEM_LEN_MAX);

        if (ret > 0 && context->cb) {
            context->cb(buf, ret, context->args);
        }
    }

    if (buf) {
        free(buf);
    }

    return NULL;
}

void process_handle_data_destroy(process_handle_data_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is NULL \n");
        return;
    }
    process_handle_data_s *context = *context_pp;
    log_info("process handle data context: %p destroy, fifo: %p, id: %0lx \n",
            context, context->fifo, context->id);

    while (!LOG_FIFO_IS_EMPTY(context->fifo)) {
        usleep(10 * 1000);
    }
    context->is_exit = 1;
    pthread_cond_signal(&context->cond);
    usleep(10 * 1000);
    pthread_join(context->id, NULL);

    pthread_mutex_destroy(&context->mutex);
    pthread_cond_destroy(&context->cond);

    log_fifo_destroy(&context->fifo);

    free(context);
    *context_pp = NULL;
}

process_handle_data_s *process_handle_data_create(const char *name,
        hy_u32_t fifo_len, process_handle_data_cb_t cb, void *args)
{
    if (!name || fifo_len <= 0 || !cb) {
        log_error("the param is NULL \n");
        return NULL;
    }

    process_handle_data_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }
        context->cb     = cb;
        context->args   = args;

        if (0 != pthread_mutex_init(&context->mutex, NULL)) {
            log_error("pthread_mutex_init failed \n");
            break;
        }

        if (0 != pthread_cond_init(&context->cond, NULL)) {
            log_error("pthread_cond_init failed \n");
            break;
        }

        context->fifo = log_fifo_create(fifo_len);
        if (!context->fifo) {
            log_info("fifo_create failed \n");
            break;
        }

        if (0 != pthread_create(&context->id, NULL, _thread_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

#ifdef _GNU_SOURCE
        pthread_setname_np(context->id, name);
#endif

        log_info("process handle data context: %p create, fifo: %p, id: %0lx \n",
                context, context->fifo, context->id);
        return context;
    } while (0);

    log_error("process handle data context: %p create failed \n", context);
    process_handle_data_destroy(&context);
    return NULL;
}

