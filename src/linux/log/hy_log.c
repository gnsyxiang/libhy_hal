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

typedef struct {
    HyLogSaveConfig_s   save_c;

    char                *buf;
    hy_u32_t            read_pos;
    hy_u32_t            write_pos;

    hy_s32_t            is_exit;
    void                *async_thread_h;
    void                *async_mutex_h;
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

void HyLogWrite(HyLogAddiInfo_s *addi_info, char *fmt, ...)
{
    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &context->save_c;

    char *pos = NULL;
    hy_char_t *color[][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_YEL},
        {"I", ""},
        {"D", PRINT_FONT_GRE},
        {"T", ""},
    };
}

static hy_s32_t _async_thread_cb(void *args)
{
    _log_context_s *context = (_log_context_s *)args;

    while (!context->is_exit) {
    }

    return -1;
}

void HyLogDeInit(void)
{
    _log_context_s *context = &_context;

    HyThreadDestroy(&context->async_thread_h);

    HyThreadMutexDestroy(&context->async_mutex_h);

    HY_MEM_FREE_PP(&context->buf);

    printf("log destroy, context: %p \n", context);
}

hy_s32_t HyLogInit(HyLogConfig_s *log_c)
{
    HY_ASSERT_RET_VAL(!log_c, -1);

    if (_is_init) {
        printf("The logging system has been initialized \n");
        return -1;
    }

    _log_context_s *context = &_context;
    do {
        HY_MEMSET(context, sizeof(*context));
        HY_MEMCPY(&context->save_c, &log_c->save_c, sizeof(context->save_c));

        context->buf = HY_MEM_MALLOC_BREAK(char *, log_c->fifo_len);
        context->read_pos = context->write_pos = 0;

        context->async_mutex_h = HyThreadMutexCreate_m();
        if (!context->async_mutex_h) {
            printf("HyThreadMutexCreate_m failed \n");
            break;
        }

        context->async_thread_h = HyThreadCreate_m("HY_async_log",
                _async_thread_cb, context);
        if (!context->async_thread_h) {
            printf("HyThreadCreate_m failed \n");
            break;
        }

        _is_init = 1;
        printf("log init \n");
        return 0;
    } while (0);

    printf("log init failed \n");
    HyLogDeInit();
    return -1;
}

