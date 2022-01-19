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
#include <sched.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/prctl.h>

#include "hy_assert.h"
#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"

#include "hy_thread.h"

typedef struct {
    HyThreadSaveConfig_t    save_config;

    pthread_t               id;
    hy_u32_t                exit_flag;
} _thread_context_t;

void HyThreadGetInfo(void *handle,
        char *name, uint32_t name_len, pthread_t *id, long *pid)
{
    HY_ASSERT_RET(!handle || !name || !id || !pid || name_len == 0);

    _thread_context_t *context = handle;

    if (name_len >= HY_THREAD_NAME_LEN_MAX) {
        name_len -= 1;
    }

    HY_MEMCPY(name, context->save_config.name, name_len);

    *id = context->id;
    *pid = syscall(SYS_gettid);
}

static void *_thread_loop_cb(void *args)
{
    _thread_context_t *context = args;
    HyThreadSaveConfig_t *save_config = &context->save_config;
    int32_t ret = 0;

    usleep(1000);
    LOGI("%s thread loop start, id: 0x%lx, pid: %ld \n",
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

    if (save_config->detach_flag) {
        HyThreadDestroy((void **)&context);
    }

    return NULL;
}

void HyThreadDestroy(void **handle)
{
    _thread_context_t *context = *handle;
    hy_u32_t cnt = 0;

    LOGT("handle: %p, *handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    if (context->save_config.destroy_flag == HY_THREAD_DESTROY_FORCE) {
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
    _thread_context_t *context = NULL;
    pthread_attr_t attr;

    LOGD("config: %p \n", config);
    HY_ASSERT_RET_VAL(!config, NULL);

    do {
        context = HY_MEM_MALLOC_BREAK(_thread_context_t *, sizeof(*context));

        HyThreadSaveConfig_t *save_config = &config->save_config;
        HY_MEMCPY(&context->save_config, save_config, sizeof(*save_config));

        if (0 != pthread_attr_init(&attr)) {
            LOGES("pthread init fail \n");
            break;
        }

        if (save_config->detach_flag
                && 0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) {
            LOGES("set detach state fail \n");
            break;
        }

        if (0 != pthread_create(&context->id, &attr, _thread_loop_cb, context)) {
            LOGES("pthread create fail \n");
            break;
        }

        if (save_config->detach_flag
                && 0 != pthread_attr_destroy(&attr)) {
            LOGES("destroy attr fail \n");
            break;
        }

        LOGD("%s thread create, handle: %p, id: 0x%lx \n",
                save_config->name, context, context->id);
        return context;
    } while (0);

    HyThreadDestroy((void **)&context);
    return NULL;
}

/*
 * pthread_t pthread_self(void)     <进程级别>是pthread 库给每个线程定义的进程内唯一标识，是 pthread 库维护的，是进程级而非系统级
 * syscall(SYS_gettid)              <系统级别>这个系统全局唯一的“ID”叫做线程PID（进程ID），或叫做TID（线程ID），也有叫做LWP（轻量级进程=线程）的。
 */

// 设置的名字可以在proc文件系统中查看: cat /proc/PID/task/tid/comm

// 如果当前线程没有被设定成DETACHED的话，
// 线程结束后，需要使用pthread_join来触发该一小段内存回收。
// pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if 0
    hal_int32_t sched_priority[][2] = {
        {HAL_THREAD_PRIORITY_NORMAL,    50},
        {HAL_THREAD_PRIORITY_LOW,       30},
        {HAL_THREAD_PRIORITY_HIGH,      70},
        {HAL_THREAD_PRIORITY_REALTIME,  99},
        {HAL_THREAD_PRIORITY_IDLE,      10},
    };

    struct sched_param param;
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = sched_priority[config->priority][1];
    pthread_attr_setschedparam(&attr, &param);
#endif

#if 0
出现如下提示，表示线程资源没有释放，可能的原因如下: 

1, 创建的是非分离线程，线程结束后，需要使用pthread_join来触发该一小段内存回收。
2, 创建的是分离线程，但是主线程优先执行完退出程序，导致被创建的线程没有执行完，导致资源的泄露

==40360== HEAP SUMMARY:
==40360==     in use at exit: 272 bytes in 1 blocks
==40360==   total heap usage: 3 allocs, 2 frees, 1,344 bytes allocated
==40360==
==40360== 272 bytes in 1 blocks are possibly lost in loss record 1 of 1
==40360==    at 0x4C31B25: calloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==40360==    by 0x40134A6: allocate_dtv (dl-tls.c:286)
==40360==    by 0x40134A6: _dl_allocate_tls (dl-tls.c:530)
==40360==    by 0x4E44227: allocate_stack (allocatestack.c:627)
==40360==    by 0x4E44227: pthread_create@@GLIBC_2.2.5 (pthread_create.c:644)
==40360==    by 0x108F1C: HalLinuxThreadInit (hal_linux_thread.c:111)
==40360==    by 0x108CC1: HalThreadInit (hal_thread.c:85)
==40360==    by 0x108AD4: main (main.c:50)
==40360==
==40360== LEAK SUMMARY:
==40360==    definitely lost: 0 bytes in 0 blocks
==40360==    indirectly lost: 0 bytes in 0 blocks
==40360==      possibly lost: 272 bytes in 1 blocks
==40360==    still reachable: 0 bytes in 0 blocks
==40360==         suppressed: 0 bytes in 0 blocks
#endif
