/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_cond.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 09:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 09:39
 */
#include <stdio.h>
#include <pthread.h>

#include "hy_log.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"

#include "hy_thread_mutex.h"
#include "hy_time.h"
#include "hy_thread_cond.h"

typedef struct {
    pthread_cond_t      cond;
} _cond_context_s;

hy_s32_t HyThreadCondSignal(void *handle)
{
    HY_ASSERT(handle);
    _cond_context_s *context = handle;

    return pthread_cond_signal(&context->cond) == 0 ? 0 : -1;
}

hy_s32_t HyThreadCondBroadcast(void *handle)
{
    HY_ASSERT(handle);
    _cond_context_s *context = handle;

    return pthread_cond_broadcast(&context->cond) == 0 ? 0 : -1;
}

hy_s32_t HyThreadCondWait(void *handle, void *mutex_h, hy_u32_t timeout_ms)
{
    HY_ASSERT(handle);
    HY_ASSERT(mutex_h);

    _cond_context_s *context = handle;
    hy_s32_t ret = 0;
    pthread_mutex_t *mutex = HyThreadMutexGetLock(mutex_h);

    if (timeout_ms == 0) {
        ret = pthread_cond_wait(&context->cond, mutex);
    } else {
        struct timespec ts = HyTimeGetTimespec(timeout_ms);
        ret = pthread_cond_timedwait(&context->cond, mutex, &ts);
    }

    return ret == 0 ? 0 : -1;
}

void HyThreadCondDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);
    _cond_context_s *context = *handle;

    if (0 != pthread_cond_destroy(&context->cond)) {
        LOGES("pthread_cond_destroy faield \n");
    }

    LOGI("thread cond destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyThreadCondCreate(HyThreadCondConfig_s *cond_c)
{
    LOGT("cond_c: %p \n", cond_c);
    HY_ASSERT_RET_VAL(!cond_c, NULL);
    _cond_context_s *context = NULL;

    do {
        context = HY_MEM_MALLOC_BREAK(_cond_context_s *, sizeof(*context));

        if (0 != pthread_cond_init(&context->cond, NULL)) {
            LOGES("pthread_cond_init failed \n");
            break;
        }

        LOGI("thread cond create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("thread cond create failed \n");
    HyThreadCondDestroy((void **)&context);
    return NULL;
}

