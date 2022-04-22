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
#include "process_single.h"

typedef struct {
    HyLogAddiInfo_s     *addi_info;
} _thread_private_s;

typedef struct {
    HyLogSaveConfig_s   save_c;
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
    va_list args;

    va_start(args, fmt);

    if (HY_LOG_MODE_PROCESS_SINGLE == save_c->mode) {
        process_single_write(addi_info, fmt, args);
    }

    va_end(args);
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

