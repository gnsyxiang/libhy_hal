/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread_sem.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    31/03 2022 08:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        31/03 2022      create the file
 * 
 *     last modified: 31/03 2022 08:48
 */
#include <stdio.h>
#include <semaphore.h>

#include "hy_log.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"

#include "hy_thread_sem.h"

typedef struct {
    sem_t sem;
} _sem_context_t;

hy_s32_t HyThreadSemPost(void *handle)
{
    HY_ASSERT(handle);

    return sem_post(&((_sem_context_t *)handle)->sem);
}

hy_s32_t HyThreadSemWait(void *handle)
{
    HY_ASSERT(handle);

    return sem_wait(&((_sem_context_t *)handle)->sem);
}

void HyThreadSemDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _sem_context_t *context = *handle;

    if (0 != sem_destroy(&context->sem)) {
        LOGE("sem_destroy failed \n");
    }

    LOGI("thread sem destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyThreadSemCreate(HyThreadSemConfig_s *sem_c)
{
    LOGT("sem_c: %p \n", sem_c);
    HY_ASSERT_RET_VAL(!sem_c, NULL);

    _sem_context_t *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_sem_context_t *, sizeof(context));

        if (0 != sem_init(&context->sem, 0, sem_c->value)) {
            LOGE("sem_init failed \n");
            break;
        }

        LOGI("thread sem create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("thread sem create failed \n");
    HyThreadSemDestroy((void **)&context);
    return NULL;
}

