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
#include "hy_printf.h"

#include "hy_log.h"

#define _SNPRINTF_FMT log_buf->buf + log_buf->len_cur, log_buf->len_real - log_buf->len_cur

/*
 * 1，去掉文件路径，只获取文件名
 *  1.1，使用strrchr函数，包含头文件#include <string.h>
 *      #define HY_STRRCHR_FILE (strrchr(__FILE__, '/'))
 *      #define HY_FILENAME     (HY_STRRCHR_FILE ? (HY_STRRCHR_FILE + 1) : __FILE__)
 *  1.2，使用basename函数，包含头文件#include <libgen.h>
 *      basename(__FILE__)
 */

typedef struct {
    char        *buf;
    hy_u32_t    len_real;
    hy_u32_t    len_cur;

    hy_u32_t    len_min;
    hy_u32_t    len_max;
} _log_buf_t;

typedef struct {
    HyLogSaveConfig_s   save_c;

    hy_s32_t            init_flag;
    pthread_mutex_t     printf_mutex;
} _log_context_t;

static pthread_key_t thread_key;
static _log_context_t *context = NULL;

static void _log_buf_reset(_log_buf_t *log_buf)
{
    if (!log_buf) {
        return;
    }

    memset(log_buf->buf, 0, log_buf->len_real);
    log_buf->len_cur = 0;
}

static void _log_buf_destroy(void *args)
{
    _log_buf_t *log_buf = args;

    if (log_buf) {
        if (log_buf->buf) {
            free(log_buf->buf);
        }

        free(log_buf);
    }
}

static _log_buf_t *_log_buf_create(hy_u32_t len_min, hy_u32_t len_max)
{
    _log_buf_t *log_buf = NULL;

    do {
        if (len_min > len_max || len_min == 0) {
            printf("the log buf len is error \n");
            break;
        }

        log_buf = calloc(1, sizeof(*log_buf));
        if (!log_buf) {
            printf("malloc fail \n");
            break;
        }

        log_buf->buf = calloc(1, len_min);
        if (!log_buf->buf) {
            printf("calloc fail \n");
            break;
        }

        log_buf->len_min = len_min;
        log_buf->len_max = len_max;

        log_buf->len_cur = 0;
        log_buf->len_real = len_min;

        return log_buf;
    } while (0);

    return NULL;
}

static void _thread_key_destroy(void)
{
    _log_buf_t *log_buf = pthread_getspecific(thread_key);
    if (!log_buf) {
        printf("pthread_getspecific failed \n");
        return ;
    }

    _log_buf_destroy(log_buf);
}

static _log_buf_t *_thread_key_create(void)
{
    _log_buf_t *log_buf = NULL;
    HyLogSaveConfig_s *save_c = &context->save_c;

    log_buf = _log_buf_create(save_c->buf_len_min, save_c->buf_len_max);
    if (!log_buf) {
        printf("_log_buf_create fail \n");
        return NULL;
    }

    if (0 != pthread_setspecific(thread_key, log_buf)) {
        printf("pthread_setspecific fail \n");
        _log_buf_destroy(log_buf);
    }

    return log_buf;
}

static _log_buf_t *_thread_key_featch(void)
{
    _log_buf_t *log_buf = NULL;

    log_buf = pthread_getspecific(thread_key);
    if (!log_buf) {
        log_buf = _thread_key_create();
    } else {
        _log_buf_reset(log_buf);
    }

    return log_buf;
}

static void _get_cur_time(char *buf, hy_u32_t len)
{
    time_t t = 0;
    struct tm tm;
    struct timeval tv;

    t = time(NULL);
    localtime_r(&t, &tm);

    gettimeofday(&tv, NULL);

    snprintf(buf, len, "%04d-%02d-%02d_%02d:%02d:%02d.%03d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec, (hy_u32_t)tv.tv_usec / 1000);
}

HY_WEAK void HyLogWrite(HyLogLevel_e level, const char *err_str,
        const char *file, hy_u32_t line, pthread_t tid, long pid,
        char *fmt, ...)
{
    _log_buf_t *log_buf = NULL;
    char format_time[32] = {0};
    hy_char_t *color[HY_LOG_LEVEL_MAX][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_YEL},
        {"I", ""},
        {"D", PRINT_FONT_GRE},
        {"T", ""},
    };

    if (!context || context->save_c.level < level) {
        return ;
    }

    log_buf = _thread_key_featch();
    if (!log_buf) {
        printf("_thread_key_featch fail \n");
        return ;
    }

    if (context->save_c.color_enable) {
        log_buf->len_cur += snprintf(_SNPRINTF_FMT, "%s[%s]",
                color[level][1], color[level][0]);
    }

    _get_cur_time(format_time, 32);

    log_buf->len_cur += snprintf(_SNPRINTF_FMT,
            "[%s][%ld-0x%lx][%s:%"PRId32"] ", format_time, pid, tid, file, line); 

    va_list args;
    va_start(args, fmt);
    log_buf->len_cur += vsnprintf(_SNPRINTF_FMT, fmt, args);
    va_end(args);

    if (err_str) {
        // -1 是为了去除'\n', 在最后加上'\n'
        log_buf->len_cur -= 1;
        log_buf->len_cur += snprintf(_SNPRINTF_FMT, ", error: %s \n", err_str);
    }

    if (context->save_c.color_enable) {
        log_buf->len_cur += snprintf(_SNPRINTF_FMT, "%s", PRINT_ATTR_RESET);
    }

    pthread_mutex_lock(&context->printf_mutex);
    printf("%s", log_buf->buf);
    pthread_mutex_unlock(&context->printf_mutex);
}

HY_WEAK void HyLogDestroy(void **handle)
{
    if (context) {
        pthread_mutex_destroy(&context->printf_mutex);

        HY_MEM_FREE_PP(&context);
    }
}

HY_WEAK void *HyLogCreate(HyLogConfig_s *log_c)
{
    HY_ASSERT_RET_VAL(!log_c, NULL);

    if (context && context->init_flag) {
        LOGW("hy_log already initialized \n");
        return NULL;
    }

    do {
        context = HY_MEM_MALLOC_BREAK(_log_context_t *, sizeof(*context));

        HY_MEMCPY(&context->save_c, &log_c->save_c, sizeof(context->save_c));

        if (0 != pthread_key_create(&thread_key, _log_buf_destroy)) {
            printf("pthread_key_create failed \n");
            break;
        }

        if (0 != pthread_mutex_init(&context->printf_mutex, NULL)) {
            printf("pthread_mutex_init fail \n");
            break;
        }

        if (0 != atexit(_thread_key_destroy)) {
            printf("atexit fail \n");
            break;
        }

        context->init_flag = 1;
        return context;
    } while (0);

    HyLogDestroy((void **)&context);
    return NULL;
}

