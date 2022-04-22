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
#include "hy_hal_utils.h"

#include "hy_log.h"
#include "log_private.h"
#include "process_single.h"
#include "dynamic_array.h"

typedef struct {
    HyLogSaveConfig_s   save_c;

    pthread_key_t       thread_key;
    format_log_cb_t    format_log_cb[5];
} _log_context_s;

static hy_s32_t _is_init = 0;
static _log_context_s _context;

HyLogLevel_e HyLogLevelGet(void)
{
    return _context.save_c.level;
}

void HyLogLevelSet(HyLogLevel_e level)
{
}

static hy_s32_t _format_log_color_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    HyLogLevel_e level = addi_info->level;
    hy_char_t *color[][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_YEL},
        {"I", ""},
        {"D", PRINT_FONT_GRE},
        {"T", ""},
    };
    char buf[16] = {0};
    hy_s32_t ret = 0;

    ret = snprintf(buf, sizeof(buf), "%s[%s]", color[level][1], color[level][0]);

    return dynamic_array_write(dynamic_array, buf, ret);
}

static hy_s32_t _format_log_time_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    return 0;
}

static hy_s32_t _format_log_pid_id_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    return 0;
}

static hy_s32_t _format_log_func_line_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    char buf[64] = {0};
    hy_s32_t ret = 0;

    ret = snprintf(buf, sizeof(buf),
            "[%s:%"PRId32"]", addi_info->func, addi_info->line);

    return dynamic_array_write(dynamic_array, buf, ret);
}

static hy_s32_t _format_log_color_reset_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    return dynamic_array_write(dynamic_array,
            PRINT_ATTR_RESET, HY_STRLEN(PRINT_ATTR_RESET));
}

static void _thread_private_data_reset(dynamic_array_s *dynamic_array)
{
    DYNAMIC_ARRAY_RESET(dynamic_array);
}

static dynamic_array_s *_thread_private_data_get(void)
{
    dynamic_array_s *dynamic_array = NULL;

    dynamic_array = pthread_getspecific(_context.thread_key);
    if (!dynamic_array) {
        printf("pthread_getspecific failed \n");
        return NULL;
    } else {
        return dynamic_array;
    }
}

static hy_s32_t _thread_private_data_set(dynamic_array_s *dynamic_array)
{
    HY_ASSERT_RET_VAL(!dynamic_array, -1);

    if (0 != pthread_setspecific(_context.thread_key, dynamic_array)) {
        printf("pthread_setspecific fail \n");
        return -1;
    } else {
        return 0;
    }
}

static void _thread_private_data_destroy(void *args)
{
    HY_ASSERT_RET(!args);
    dynamic_array_s *dynamic_array = args;

    dynamic_array_destroy(&dynamic_array);
}

static dynamic_array_s *_thread_private_data_create(void)
{
    dynamic_array_s *dynamic_array = NULL;

    dynamic_array = dynamic_array_create(128, 4 * 1024);
    if (!dynamic_array) {
        printf("dynamic_array_create failed \n");
        return NULL;
    }

    _thread_private_data_set(dynamic_array);

    return dynamic_array;
}

static dynamic_array_s* _thread_private_data_featch(void)
{
    dynamic_array_s *dynamic_array = NULL;

    dynamic_array = _thread_private_data_get();
    if (!dynamic_array) {
        dynamic_array = _thread_private_data_create();
        if (!dynamic_array) {
            printf("_thread_private_data_create failed \n");
        }
    } else {
        _thread_private_data_reset(dynamic_array);
    }

    return dynamic_array;
}

void HyLogWrite(HyLogAddiInfo_s *addi_info, char *fmt, ...)
{
    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &context->save_c;
    dynamic_array_s *dynamic_array = NULL;
    log_write_info_s log_write_info;
    va_list args;

    if (!_is_init) {
        return;
    }

    dynamic_array = _thread_private_data_featch();
    if (!dynamic_array) {
        printf("_thread_private_data_featch failed \n");
        return;
    }

    va_start(args, fmt);
    if (HY_LOG_MODE_PROCESS_SINGLE == save_c->mode) {
        log_write_info.format_log_cb        = context->format_log_cb;
        log_write_info.format_log_cb_cnt    = HyHalUtilsArrayCnt(context->format_log_cb);
        log_write_info.dynamic_array        = dynamic_array;
        log_write_info.addi_info            = addi_info;
        log_write_info.fmt                  = fmt;
        log_write_info.str_args             = &args;
        process_single_write(&log_write_info);
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

        context->format_log_cb[0] = _format_log_color_cb;
        context->format_log_cb[1] = _format_log_time_cb;
        context->format_log_cb[2] = _format_log_pid_id_cb;
        context->format_log_cb[3] = _format_log_func_line_cb;
        context->format_log_cb[4] = _format_log_color_reset_cb;

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

