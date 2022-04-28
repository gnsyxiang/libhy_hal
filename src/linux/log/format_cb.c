/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    format_cb.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 10:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 10:21
 */
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>

#include "hy_printf.h"
#include "log_private.h"

#include "format_cb.h"

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

void format_cb_register(format_cb_t *format_cb, hy_u32_t format)
{
    if (!format_cb) {
        log_error("the param is NULL \n");
        return;
    }

    struct {
        HyLogOutputFormat_e     format;
        format_cb_t         format_log_cb;
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
        if (log_format_cb[i].format == (format & 0x1 << i)) {
            memcpy(format_cb[i], log_format_cb[i].format_log_cb,
                    sizeof(log_format_cb[i].format_log_cb));
        }
    }
}

