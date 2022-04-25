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

#include "hy_printf.h"
#include "log_private.h"
#include "dynamic_array.h"
#include "process_single.h"
#include "process_ipc.h"

#include "hy_log.h"

/* @fixme: <22-04-23, uos>
 * 两者大小不能相隔太近，
 * 否则出现内存申请错误(malloc(): unsorted double linked list corrupted)，
 * 测试条件: min_len = 60, max_len = 87
 */
#define _DYNAMIC_ARRAY_MIN_LEN  (256)
#define _DYNAMIC_ARRAY_MAX_LEN  (4 * 1024)

#define _ARRAY_CNT(array) (hy_u32_t)(sizeof((array)) / sizeof((array)[0]))

typedef struct {
    HyLogSaveConfig_s   save_c;

    pthread_key_t       thread_key;
    format_log_cb_t     format_log_cb[FORMAT_LOG_CB_CNT];
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
    char buf[16] = {0};
    hy_s32_t ret = 0;
    hy_char_t *color[][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_YEL},
        {"I", ""},
        {"D", PRINT_FONT_GRE},
        {"T", ""},
    };

    ret = snprintf(buf, sizeof(buf), "%s", color[addi_info->level][1]);

    return dynamic_array_write(dynamic_array, buf, ret);
}

static hy_s32_t _format_log_level_info_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    char buf[4] = {0};
    hy_s32_t ret = 0;
    hy_char_t *color[][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_YEL},
        {"I", ""},
        {"D", PRINT_FONT_GRE},
        {"T", ""},
    };

    ret = snprintf(buf, sizeof(buf), "[%s]", color[addi_info->level][0]);

    return dynamic_array_write(dynamic_array, buf, ret);
}

static hy_s32_t _format_log_time_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    char buf[32] = {0};
    hy_s32_t ret = 0;
    time_t t = 0;
    struct tm tm;
    struct timeval tv;

    t = time(NULL);
    localtime_r(&t, &tm);
    gettimeofday(&tv, NULL);

    ret = snprintf(buf, sizeof(buf), "[%04d-%02d-%02d_%02d:%02d:%02d.%03d]",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec, (hy_u32_t)tv.tv_usec / 1000);

    return dynamic_array_write(dynamic_array, buf, ret);
}

static hy_s32_t _format_log_pid_id_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    char buf[32] = {0};
    hy_s32_t ret = 0;

    ret = snprintf(buf, sizeof(buf),
            "[%ld-0x%lx]", addi_info->pid, addi_info->tid);

    return dynamic_array_write(dynamic_array, buf, ret);
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

static hy_s32_t _format_log_usr_msg_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    return dynamic_array_write_vprintf(dynamic_array,
            addi_info->fmt, addi_info->str_args);
}

static hy_s32_t _format_log_color_reset_cb(dynamic_array_s *dynamic_array,
        HyLogAddiInfo_s *addi_info)
{
    return dynamic_array_write(dynamic_array,
            PRINT_ATTR_RESET, strlen(PRINT_ATTR_RESET));
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
        log_info("pthread_getspecific failed \n");
        return NULL;
    } else {
        return dynamic_array;
    }
}

static hy_s32_t _thread_private_data_set(dynamic_array_s *dynamic_array)
{
    if (!dynamic_array) {
        log_error("the param is error \n");
        return -1;
    }

    if (0 != pthread_setspecific(_context.thread_key, dynamic_array)) {
        log_error("pthread_setspecific fail \n");
        return -1;
    } else {
        return 0;
    }
}

static void _thread_private_data_destroy(void *args)
{
    if (!args) {
        log_error("the param is error \n");
        return ;
    }
    dynamic_array_s *dynamic_array = args;

    dynamic_array_destroy(&dynamic_array);
}

static dynamic_array_s *_thread_private_data_create(void)
{
    dynamic_array_s *dynamic_array = NULL;

    dynamic_array = dynamic_array_create(_DYNAMIC_ARRAY_MIN_LEN, _DYNAMIC_ARRAY_MAX_LEN);
    if (!dynamic_array) {
        log_error("dynamic_array_create failed \n");
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
            log_error("_thread_private_data_create failed \n");
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

    dynamic_array = _thread_private_data_featch();
    if (!dynamic_array) {
        log_info("_thread_private_data_featch failed \n");
        return;
    }

    va_start(args, fmt);
    addi_info->fmt = fmt;
    addi_info->str_args = &args;
    if (HY_LOG_MODE_PROCESS_SINGLE == save_c->mode) {
        log_write_info.format_log_cb        = context->format_log_cb;
        log_write_info.format_log_cb_cnt    = _ARRAY_CNT(context->format_log_cb);
        log_write_info.dynamic_array        = dynamic_array;
        log_write_info.addi_info            = addi_info;
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
    if (!log_c) {
        log_error("the param is error \n");
        return -1;
    }

    if (_is_init) {
        log_error("The logging system has been initialized \n");
        return -1;
    }

    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &log_c->save_c;
    hy_s32_t ret = 0;

    do {
        memset(context, '\0', sizeof(*context));
        memcpy(&context->save_c, &log_c->save_c, sizeof(context->save_c));

        struct {
            HyLogOutputFormat_e     format;
            format_log_cb_t         format_log_cb;
        } log_format_cb[] = {
            {HY_LOG_OUTPUT_FORMAT_COLOR,        {_format_log_color_cb,          NULL,                       }},
            {HY_LOG_OUTPUT_FORMAT_LEVEL_INFO,   {_format_log_level_info_cb,     _format_log_level_info_cb,  }},
            {HY_LOG_OUTPUT_FORMAT_TIME,         {_format_log_time_cb,           _format_log_time_cb,        }},
            {HY_LOG_OUTPUT_FORMAT_PID_ID,       {_format_log_pid_id_cb,         _format_log_pid_id_cb,      }},
            {HY_LOG_OUTPUT_FORMAT_FUNC_LINE,    {_format_log_func_line_cb,      _format_log_func_line_cb,   }},
            {HY_LOG_OUTPUT_FORMAT_USR_MSG,      {_format_log_usr_msg_cb,        _format_log_usr_msg_cb,     }},
            {HY_LOG_OUTPUT_FORMAT_COLOR_RESET,  {_format_log_color_reset_cb,    NULL,                       }},
        };

        for (hy_u32_t i = 0; i < _ARRAY_CNT(log_format_cb); ++i) {
            if (log_format_cb[i].format == (save_c->output_format & 0x1 << i)) {
                memcpy(context->format_log_cb[i],
                        log_format_cb[i].format_log_cb,
                        sizeof(log_format_cb[i].format_log_cb));
            }
        }

        if (0 != pthread_key_create(&context->thread_key,
                    _thread_private_data_destroy)) {
            log_error("pthread_key_create failed \n");
            break;
        }

        if (0 != atexit(_log_thread_private_data_destroy)) {
            log_error("atexit fail \n");
            break;
        }

        switch (log_c->save_c.mode) {
            case HY_LOG_MODE_PROCESS_SINGLE:
                ret = process_single_create(log_c->fifo_len);
                break;
            case HY_LOG_MODE_PROCESS_CLIENT:
                break;
            case HY_LOG_MODE_PROCESS_SERVER:
                ret = process_ipc_server_create(log_c->fifo_len);
                break;
            default:
                break;
        }
        if (0 != ret) {
            log_error("process single create failed \n");
            break;
        }

        _is_init = 1;
        return 0;
    } while (0);

    HyLogDeInit();
    return -1;
}

