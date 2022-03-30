/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_mutex.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/01 2022 19:46
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/01 2022      create the file
 * 
 *     last modified: 21/01 2022 19:46
 */
#include <stdio.h>
#include <pthread.h>

#include "hy_log.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"

#include "hy_thread_mutex.h"

typedef struct {
    HyThreadMutexSaveConfig_s   save_config;

    pthread_mutex_t             mutex;
} _thread_mutex_context_t;

hy_s32_t HyThreadMutexLock(void *handle)
{
    HY_ASSERT(handle);

    return pthread_mutex_lock(&((_thread_mutex_context_t *)handle)->mutex);
}

hy_s32_t HyThreadMutexUnLock(void *handle)
{
    HY_ASSERT(handle);

    return pthread_mutex_unlock(&((_thread_mutex_context_t *)handle)->mutex);
}

void HyThreadMutexDestroy(void **handle)
{
    LOGT("&context: %p, context: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle)

    _thread_mutex_context_t *context = *handle;

    if (0 != pthread_mutex_destroy(&context->mutex)) {
        LOGES("pthread_mutex_destroy failed \n");
    }

    LOGI("thread mutex destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyThreadMutexCreate(HyThreadMutexConfig_s *config)
{
    LOGT("thread mutex config: %p \n", config);
    HY_ASSERT_RET_VAL(!config, NULL);

    _thread_mutex_context_t *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_thread_mutex_context_t *, sizeof(*context));

        if (0 != pthread_mutex_init(&context->mutex, NULL)) {
            LOGES("pthread_mutex_init failed \n");
            break;
        }

        LOGI("thread mutex create, context: %p \n", context);
        return context;
    } while (0);

    LOGI("thread mutex create failed \n");
    return NULL;
}
