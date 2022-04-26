/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    fifo_async.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 09:24
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 09:24
 */
#include <stdio.h>
#include <stdlib.h>

#include "log_private.h"

#include "fifo_async.h"

hy_s32_t fifo_async_is_empty(fifo_async_s *fifo_async)
{
    return FIFO_IS_EMPTY(fifo_async->fifo);
}

hy_s32_t fifo_async_write(fifo_async_s *fifo_async,
        const void *buf, hy_u32_t len)
{
    hy_s32_t ret = 0;

    pthread_mutex_lock(&fifo_async->mutex);
    ret = fifo_write(fifo_async->fifo, buf, len);
    pthread_mutex_unlock(&fifo_async->mutex);

    pthread_cond_signal(&fifo_async->cond);

    return ret;
}

hy_s32_t fifo_async_read(fifo_async_s *fifo_async, void *buf, hy_u32_t len)
{
    pthread_mutex_lock(&fifo_async->mutex);
    if (FIFO_IS_EMPTY(fifo_async->fifo)) {
        pthread_cond_wait(&fifo_async->cond, &fifo_async->mutex);
    }
    pthread_mutex_unlock(&fifo_async->mutex);

    memset(buf, '\0', len);
    return fifo_read(fifo_async->fifo, buf, sizeof(buf));
}

void fifo_async_destroy(fifo_async_s **fifo_async_pp)
{
    fifo_async_s *fifo_async = *fifo_async_pp;

    pthread_cond_signal(&fifo_async->cond);

    pthread_mutex_destroy(&fifo_async->mutex);
    pthread_cond_destroy(&fifo_async->cond);

    fifo_destroy(&fifo_async->fifo);

    free(fifo_async);
    *fifo_async_pp = NULL;
}

fifo_async_s *fifo_async_create(hy_u32_t fifo_len)
{
    fifo_async_s *fifo_async = NULL;
    do {
        fifo_async = calloc(1, sizeof(*fifo_async));
        if (!fifo_async) {
            log_error("calloc failed \n");
            break;
        }

        if (0 != pthread_mutex_init(&fifo_async->mutex, NULL)) {
            log_error("pthread_mutex_init failed \n");
            break;
        }

        if (0 != pthread_cond_init(&fifo_async->cond, NULL)) {
            log_error("pthread_cond_init failed \n");
            break;
        }

        fifo_async->fifo = fifo_create(fifo_len);
        if (!fifo_async->fifo) {
            log_info("fifo_create failed \n");
            break;
        }

        return fifo_async;
    } while (0);

    fifo_async_destroy(&fifo_async);
    return NULL;
}

