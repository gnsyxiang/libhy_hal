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

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_thread.h"
#include "hy_thread_mutex.h"
#include "hy_thread_cond.h"

#include "process_single.h"
#include "fifo.h"
#include "dynamic_array.h"

typedef struct {
    hy_s32_t            is_exit;
    void                *thread_h;

    fifo_context_s      *fifo;
    void                *mutex_h;
    void                *cond_h;
} _process_single_context_s;

static _process_single_context_s _process_context;

void process_single_write(_thread_private_data_s *thread_private_data,
        char *fmt, va_list str_args)
{
    _process_single_context_s *context = &_process_context;
    HyLogAddiInfo_s *addi_info = thread_private_data->addi_info;
    dynamic_array_s *dynamic_array = thread_private_data->dynamic_array;

    dynamic_array_write(dynamic_array, addi_info->file, HY_STRLEN(addi_info->file));
    dynamic_array_write(dynamic_array, "\n", 1);

    HyThreadMutexLock_m(context->mutex_h);
    fifo_write(context->fifo, dynamic_array->buf, dynamic_array->cur_len);
    DYNAMIC_ARRAY_RESET(dynamic_array);
    HyThreadMutexUnLock_m(context->mutex_h);
}

static hy_s32_t _thread_cb(void *args)
{
    _process_single_context_s *context = args;
    hy_u32_t len = 0;
    char buf[1024] = {0};

    while (!context->is_exit) {
        HyThreadMutexLock_m(context->mutex_h);
        if (FIFO_IS_EMPTY(context->fifo)) {
            HyThreadCondWait_m(context->cond_h, context->mutex_h, 0);
        }
        HyThreadMutexUnLock_m(context->mutex_h);

        HY_MEMSET(buf, sizeof(buf));
        len = fifo_read(context->fifo, buf, sizeof(buf));

        /* @fixme: <22-04-22, uos> 多种方式处理数据 */
        printf("------------------buf-------%s", buf);
    }

    return -1;
}

void process_single_destroy(void)
{
    _process_single_context_s *context = &_process_context;

    context->is_exit = 1;
    HyThreadDestroy(&context->thread_h);

    HyThreadMutexDestroy(&context->mutex_h);
    HyThreadCondDestroy(&context->cond_h);

    fifo_destroy(&context->fifo);
}

hy_s32_t process_single_create(hy_u32_t fifo_len)
{
    _process_single_context_s *context = &_process_context;

    do {
        HY_MEMSET(context, sizeof(*context));

        context->mutex_h = HyThreadMutexCreate_m();
        if (!context->mutex_h) {
            printf("HyThreadMutexCreate_m failed \n");
            break;
        }

        context->cond_h = HyThreadCondCreate_m();
        if (!context->cond_h) {
            printf("HyThreadCondCreate_m failed \n");
            break;
        }

        context->fifo = fifo_create(fifo_len);
        if (!context->fifo) {
            printf("fifo_create failed \n");
            break;
        }

        context->thread_h = HyThreadCreate_m("HY_async_log", _thread_cb, context);
        if (!context->thread_h) {
            printf("HyThreadCreate_m failed \n");
            break;
        }

        return 0;
    } while (0);

    process_single_destroy();
    return -1;
}

