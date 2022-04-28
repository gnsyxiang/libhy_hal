/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    thread_specific_data.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 10:47
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 10:47
 */
#include <stdio.h>

#include "log_private.h"

#include "thread_specific_data.h"

static thread_specific_data_s context;

static hy_s32_t _thread_specific_data_set(void *handle)
{
    if (!handle) {
        log_error("the param is error \n");
        return -1;
    }

    if (0 != pthread_setspecific(context.thread_key, handle)) {
        log_error("pthread_setspecific fail \n");
        return -1;
    } else {
        return 0;
    }
}

static void *_thread_specific_data_get(void)
{
    void *handle = NULL;

    handle = pthread_getspecific(context.thread_key);
    if (!handle) {
        log_info("pthread_getspecific failed \n");
        return NULL;
    } else {
        return handle;
    }
}

static void _thread_specific_data_destroy(void *args)
{
    if (context.destroy_cb) {
        context.destroy_cb(args);
    }
}

static void _main_thread_specific_data_destroy(void)
{
    _thread_specific_data_destroy(_thread_specific_data_get());
}

void *thread_specific_data_fetch(void)
{
    void *handle = _thread_specific_data_get();
    if (!handle) {
        if (context.destroy_cb) {
            handle = context.create_cb();
            if (!handle) {
                log_error("context.create_cb failed \n");
                return NULL;
            }
        }

        _thread_specific_data_set(handle);
    } else {
        if (context.reset_cb) {
            context.reset_cb(handle);
        }
    }

    return handle;
}

void thread_specific_data_destroy(void)
{
    if (0 != atexit(_main_thread_specific_data_destroy)) {
        log_error("atexit fail \n");
    }
}

hy_s32_t thread_specific_data_create(
        thread_specific_data_create_cb_t create_cb,
        thread_specific_data_destroy_cb_t destroy_cb,
        thread_specific_data_reset_cb_t reset_cb)
{
    assert(create_cb);
    assert(destroy_cb);

    do {
        memset(&context, '\0', sizeof(context));

        context.create_cb  = create_cb;
        context.destroy_cb = destroy_cb;
        context.reset_cb   = reset_cb;

        if (0 != pthread_key_create(&context.thread_key,
                    _thread_specific_data_destroy)) {
            log_error("pthread_key_create failed \n");
            break;
        }

        return 0;
    } while (0);

    return -1;
}

