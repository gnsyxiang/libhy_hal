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
    pthread_mutex_t             mutex;
} _mutex_context_s;

hy_s32_t HyThreadMutexTryLock(void *handle)
{
    HY_ASSERT(handle);

    return pthread_mutex_trylock(&((_mutex_context_s *)handle)->mutex);
}

hy_s32_t HyThreadMutexLock(void *handle)
{
    HY_ASSERT(handle);

    return pthread_mutex_lock(&((_mutex_context_s *)handle)->mutex);
}

hy_s32_t HyThreadMutexUnLock(void *handle)
{
    HY_ASSERT(handle);

    return pthread_mutex_unlock(&((_mutex_context_s *)handle)->mutex);
}

void *HyThreadMutexGetLock(void *handle)
{
    HY_ASSERT(handle);

    return &((_mutex_context_s *)handle)->mutex;
}

void HyThreadMutexDestroy(void **handle)
{
    LOGT("&context: %p, context: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _mutex_context_s *context = *handle;

    if (0 != pthread_mutex_destroy(&context->mutex)) {
        LOGES("pthread_mutex_destroy failed \n");
    }

    LOGI("thread mutex destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyThreadMutexCreate(HyThreadMutexConfig_s *mutex_c)
{
    LOGT("mutex_c: %p \n", mutex_c);
    HY_ASSERT_RET_VAL(!mutex_c, NULL);

    _mutex_context_s *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_mutex_context_s *, sizeof(*context));

        if (0 != pthread_mutex_init(&context->mutex, NULL)) {
            LOGES("pthread_mutex_init failed \n");
            break;
        }

        LOGI("thread mutex create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("thread mutex create failed \n");
    HyThreadMutexDestroy((void **)&context);
    return NULL;
}

