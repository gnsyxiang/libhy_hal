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
#include <errno.h>
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/prctl.h>

#include "hy_thread.h"

#include "hy_assert.h"
#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"

typedef struct {
    HyThreadSaveConfig_t    save_config;

    pthread_t               id;
    hy_u32_t                exit_flag;
} _thread_context_t;

static void *_thread_loop_cb(void *args)
{
    _thread_context_t *context = args;
    HyThreadSaveConfig_t *save_config = &context->save_config;
    int32_t ret = 0;

    usleep(1000);
    LOGI("%s thread loop start, tid: 0x%lx, pid: %ld \n",
            save_config->name, context->id, syscall(SYS_gettid));

#ifdef _GNU_SOURCE
    pthread_setname_np(context->id, save_config->name);
#endif

    while (0 == ret) {
        ret = save_config->thread_loop_cb(save_config->args);

        // pthread_testcancel();
    }

    context->exit_flag = 1;
    LOGD("%s thread loop stop \n", save_config->name);
    return NULL;
}

void HyThreadDestroy(void **handle)
{
    HY_ASSERT_VAL_RET(!handle || !*handle);

    _thread_context_t *context = *handle;
    hy_u32_t cnt = 0;

    if (context->save_config.flag == HY_THREAD_DESTROY_FORCE) {
        if (!context->exit_flag) {
            while (++cnt <= 10) {
                usleep(200 * 1000);
            }

            LOGW("force cancellation \n");
            pthread_cancel(context->id);
        }
    }

    pthread_join(context->id, NULL);

    LOGD("%s thread destroy, handle: %p \n",
            context->save_config.name, context);

    HY_MEM_FREE_PP(handle);
}

void *HyThreadCreate(HyThreadConfig_t *config)
{
    LOGD("config: %p \n", config);
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    _thread_context_t *context = NULL;

    do {
        context = HY_MEM_MALLOC_BREAK(_thread_context_t *, sizeof(*context));

        HyThreadSaveConfig_t *save_config = &context->save_config;
        HY_MEMCPY(save_config, &config->save_config, sizeof(config->save_config));

        if (0 != pthread_create(&context->id, NULL, _thread_loop_cb, context)) {
            LOGE("failed, error: %s \n", strerror(errno));
            break;
        }

        LOGD("%s thread create, handle: %p, tid: 0x%lu \n",
                save_config->name, context, context->id);
        return context;
    } while (0);

    HyThreadDestroy((void **)&context);
    return NULL;
}
