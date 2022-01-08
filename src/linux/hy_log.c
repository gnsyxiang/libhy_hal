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
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>
#include <pthread.h>

#include "hy_assert.h"
#include "hy_compile.h"
#include "hy_string.h"
#include "hy_type.h"
#include "hy_mem.h"
#include "hy_printf.h"

#include "hy_log.h"

#define _SNPRINTF_FMT log_buf->buf + log_buf->len_cur, log_buf->len_real - log_buf->len_cur

typedef struct {
    char        *buf;
    hy_u32_t    len_real;
    hy_u32_t    len_cur;

    hy_u32_t    len_min;
    hy_u32_t    len_max;
} _log_buf_t;

typedef struct {
    HyLogSaveConfig_t   save_config;

    hy_s32_t            init_flag;
} _log_context_t;

static pthread_key_t thread_key;
static _log_context_t *context = NULL;

HY_WEAK void HyLogHex(const char *name, hy_u32_t line,
        const void *_buf, hy_u32_t len, hy_s32_t flag)
{
    if (len <= 0) {
        return;
    }
    const unsigned char *buf = (const unsigned char *)_buf;

    hy_u8_t cnt = 0;
    printf("[%s %d]len: %d \r\n", name, line, len);
    for (hy_u32_t i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a
                    || buf[i] < 32 || buf[i] >= 127) {
                printf("%02x[ ]  ", buf[i]);
            } else {
                printf("%02x[%c]  ", buf[i], buf[i]);
            }
        } else {
            printf("%02x ", buf[i]);
        }
        cnt++;
        if (cnt == 16) {
            cnt = 0;
            printf("\r\n");
        }
    }
    printf("\r\n");
}

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
    HyLogSaveConfig_t *save_config = &context->save_config;

    log_buf = _log_buf_create(save_config->buf_len_min, save_config->buf_len_max);
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

HY_WEAK void HyLogWrite(HyLogLevel_t level, const char *err_str,
        const char *file, const char *func,
        hy_u32_t line, char *fmt, ...)
{
    _log_buf_t *log_buf = NULL;
    hy_char_t *color[HY_LOG_LEVEL_MAX][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_YEL},
        {"I", ""},
        {"D", PRINT_FONT_GRE},
        {"T", ""},
    };

    if (!context || context->save_config.level < level) {
        return;
    }

    log_buf = _thread_key_featch();
    if (!log_buf) {
        printf("_thread_key_featch fail \n");
        goto _ERR_HY_LOG_WRITE_1;
    }

    if (context->save_config.color_enable) {
        log_buf->len_cur += snprintf(_SNPRINTF_FMT, "%s[%s]",
                color[level][1], color[level][0]);
    }

    log_buf->len_cur += snprintf(_SNPRINTF_FMT,
            "[%s:%"PRId32"][%s] ", file, line, func); 

    va_list args;
    va_start(args, fmt);
    log_buf->len_cur += vsnprintf(_SNPRINTF_FMT, fmt, args);
    va_end(args);

    if (err_str) {
        // -1 是为了去除'\n', 在最后加上'\n'
        log_buf->len_cur -= 1;
        log_buf->len_cur += snprintf(_SNPRINTF_FMT, ", error: %s \n", err_str);
    }

    if (context->save_config.color_enable) {
        log_buf->len_cur += snprintf(_SNPRINTF_FMT, "%s", PRINT_ATTR_RESET);
    }

    printf("%s", log_buf->buf);

    return ;

_ERR_HY_LOG_WRITE_1:
    printf("-----haha\n");
}

HY_WEAK void HyLogDestroy(void **handle)
{
    if (context) {
        HY_MEM_FREE_PP(&context);
    }
}

HY_WEAK void *HyLogCreate(HyLogConfig_t *config)
{
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    if (context && context->init_flag) {
        LOGW("hy_log already initialized \n");
        return NULL;
    }

    do {
        context = HY_MEM_MALLOC_BREAK(_log_context_t *, sizeof(*context));

        HyLogSaveConfig_t *save_config = &config->save_config;
        HY_MEMCPY(&context->save_config, save_config, sizeof(*save_config));

        if (0 != pthread_key_create(&thread_key, _log_buf_destroy)) {
            printf("pthread_key_create failed \n");
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
