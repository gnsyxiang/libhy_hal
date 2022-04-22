/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/10 2021 20:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/10 2021      create the file
 * 
 *     last modified: 29/10 2021 20:30
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/time.h>

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_printf.h"
#include "hy_thread.h"
#include "hy_thread_mutex.h"

#include "hy_log.h"
#include "log_private.h"
#include "process_single.h"
#include "dynamic_array.h"

typedef struct {
    HyLogSaveConfig_s   save_c;
    pthread_key_t       thread_key;
} _log_context_s;

static hy_s32_t _is_init = 0;
static _log_context_s _context;

HyLogLevel_e HyLogLevelGet(void)
{
    return HY_LOG_LEVEL_DEBUG;
}

void HyLogLevelSet(HyLogLevel_e level)
{
}

static void
_thread_private_data_reset(_thread_private_data_s *thread_private_data)
{
    thread_private_data->addi_info = NULL;
    DYNAMIC_ARRAY_RESET(thread_private_data->dynamic_array);
}

static _thread_private_data_s *_thread_private_data_get(void)
{
    _thread_private_data_s *thread_private_data = NULL;

    thread_private_data = pthread_getspecific(_context.thread_key);
    if (!thread_private_data) {
        printf("pthread_getspecific failed \n");
        return NULL;
    } else {
        return thread_private_data;
    }
}

static hy_s32_t _thread_private_data_set(_thread_private_data_s *thread_private_data)
{
    HY_ASSERT_RET_VAL(!thread_private_data, -1);

    if (0 != pthread_setspecific(_context.thread_key, thread_private_data)) {
        printf("pthread_setspecific fail \n");
        return -1;
    } else {
        return 0;
    }
}

static void _thread_private_data_destroy(void *args)
{
    HY_ASSERT_RET(!args);

    _thread_private_data_s *thread_private_data = args;
    dynamic_array_destroy(&thread_private_data->dynamic_array);

    thread_private_data->addi_info = NULL;

    HY_MEM_FREE_PP(&thread_private_data);
}

static _thread_private_data_s *
_thread_private_data_create(HyLogAddiInfo_s *addi_info)
{
    _thread_private_data_s *thread_private_data = NULL;

    thread_private_data =  HY_MEM_MALLOC_RET_VAL(_thread_private_data_s *,
            sizeof(thread_private_data), NULL);

    thread_private_data->dynamic_array = dynamic_array_create(128, 4 * 1024);
    if (!thread_private_data->dynamic_array) {
        printf("dynamic_array_create failed \n");
        return NULL;
    }

    thread_private_data->addi_info = addi_info;

    _thread_private_data_set(thread_private_data);

    return thread_private_data;
}

static _thread_private_data_s *
_thread_private_data_featch(HyLogAddiInfo_s *addi_info)
{
    _thread_private_data_s *thread_private_data = NULL;

    thread_private_data = _thread_private_data_get();
    if (!thread_private_data) {
        thread_private_data = _thread_private_data_create(addi_info);
        if (!thread_private_data) {
            printf("_thread_private_data_create failed \n");
        }
    } else {
        _thread_private_data_reset(thread_private_data);
    }

    return thread_private_data;
}

void HyLogWrite(HyLogAddiInfo_s *addi_info, char *fmt, ...)
{
    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &context->save_c;
    va_list args;
    _thread_private_data_s *thread_private_data;

    thread_private_data = _thread_private_data_featch(addi_info);
    if (!thread_private_data) {
        printf("_thread_private_data_featch failed \n");
        return;
    }

    va_start(args, fmt);
    if (HY_LOG_MODE_PROCESS_SINGLE == save_c->mode) {
        process_single_write(thread_private_data, fmt, args);
    }
    va_end(args);
}

static void _log_thread_private_data_destroy(void)
{
    _thread_private_data_destroy(_thread_private_data_get());
}

void HyLogDeInit(void)
{
    process_single_destroy();
}

hy_s32_t HyLogInit(HyLogConfig_s *log_c)
{
    HY_ASSERT_RET_VAL(!log_c, -1);

    if (_is_init) {
        printf("The logging system has been initialized \n");
        return -1;
    }

    _log_context_s *context = &_context;
    hy_s32_t ret = 0;
    do {
        HY_MEMSET(context, sizeof(*context));
        HY_MEMCPY(&context->save_c, &log_c->save_c, sizeof(context->save_c));

        if (0 != pthread_key_create(&context->thread_key,
                    _thread_private_data_destroy)) {
            printf("pthread_key_create failed \n");
            break;
        }

        if (0 != atexit(_log_thread_private_data_destroy)) {
            printf("atexit fail \n");
            break;
        }

        if (HY_LOG_MODE_PROCESS_SINGLE == log_c->save_c.mode) {
            ret = process_single_create(log_c->fifo_len);
        }
        if (0 != ret) {
            printf("process single create failed \n");
            break;
        }

        _is_init = 1;
        return 0;
    } while (0);

    HyLogDeInit();
    return -1;
}

