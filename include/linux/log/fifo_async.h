/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    fifo_async.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 09:19
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 09:19
 */
#ifndef __LIBHY_HAL_INCLUDE_FIFO_ASYNC_H_
#define __LIBHY_HAL_INCLUDE_FIFO_ASYNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#include "fifo.h"

#define FIFO_ITEM_LEN_MAX   (4 * 1024)

typedef struct {
    fifo_context_s      *fifo;
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
} fifo_async_s;

fifo_async_s *fifo_async_create(hy_u32_t fifo_len);
void fifo_async_destroy(fifo_async_s **fifo_async_pp);

hy_s32_t fifo_async_write(fifo_async_s *fifo_async,
        const void *buf, hy_u32_t len);
hy_s32_t fifo_async_read(fifo_async_s *fifo_async, void *buf, hy_u32_t len);

#define FIFO_ASYNC_IS_EMPTY(fifo_async) FIFO_IS_EMPTY((fifo_async)->fifo)

#ifdef __cplusplus
}
#endif

#endif

