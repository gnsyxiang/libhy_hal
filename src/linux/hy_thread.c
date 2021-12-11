/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_thread.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 08:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 08:29
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "hy_thread.h"

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

typedef struct {
    HyThreadSaveConfig_t    save_config;

    pthread_t               id;
} _thread_context_t;

static void *_thread_loop_cb(void *args)
{
    _thread_context_t *context = args;
    HyThreadSaveConfig_t *save_config = &context->save_config;
    int32_t ret = 0;

    usleep(1000);
    LOGI("%s thread loop start \n", save_config->name);

#ifdef _GNU_SOURCE
        pthread_setname_np(context->id, save_config->name);
#endif

    while (!ret) {
        ret = save_config->thread_loop_cb(save_config->args);
    }

    LOGI("%s thread loop stop \n", save_config->name);
    return NULL;
}

void HyThreadDestroy(void **handle)
{
    HY_ASSERT_VAL_RET(!handle || !*handle);

    _thread_context_t *context = *handle;

    usleep(20 * 1000);
    pthread_cancel(context->id);
    pthread_join(context->id, NULL);

    LOGI("%s thread destroy successful \n", context->save_config.name);

    HY_MEM_FREE_PP(handle);
}

void *HyThreadCreate(HyThreadConfig_t *config)
{
    _thread_context_t *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_thread_context_t *, sizeof(*context));
        HY_MEMCPY(&context->save_config, &config->save_config, sizeof(config->save_config));

        pthread_create(&context->id, NULL, _thread_loop_cb, context);

        LOGI("%s thread create successful \n", context->save_config.name);
        return context;
    } while (0);

    HyThreadDestroy((void **)&context);
    return NULL;
}

