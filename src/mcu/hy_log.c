/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 16:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 16:44
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>

#include "hy_log.h"

#include "hy_assert.h"
#include "hy_string.h"
#include "hy_type.h"
#include "hy_mem.h"

#define ALONE_DEBUG 1

/*设置输出前景色*/
#define PRINT_FONT_BLA      "\033[30m"      //黑色
#define PRINT_FONT_RED      "\033[31m"      //红色
#define PRINT_FONT_GRE      "\033[32m"      //绿色
#define PRINT_FONT_YEL      "\033[33m"      //黄色
#define PRINT_FONT_BLU      "\033[34m"      //蓝色
#define PRINT_FONT_PUR      "\033[35m"      //紫色
#define PRINT_FONT_CYA      "\033[36m"      //青色
#define PRINT_FONT_WHI      "\033[37m"      //白色

/*设置输出背景色*/ 
#define PRINT_BACK_BLA      "\033[40m"      //黑色
#define PRINT_BACK_RED      "\033[41m"      //红色
#define PRINT_BACK_GRE      "\033[42m"      //绿色
#define PRINT_BACK_YEL      "\033[43m"      //黄色
#define PRINT_BACK_BLU      "\033[44m"      //蓝色
#define PRINT_BACK_PUR      "\033[45m"      //紫色
#define PRINT_BACK_CYA      "\033[46m"      //青色
#define PRINT_BACK_WHI      "\033[47m"      //白色

/*输出属性设置*/
#define PRINT_ATTR_RESET    "\033[0m"       //重新设置属性到缺省设置 
#define PRINT_ATTR_BOL      "\033[1m"       //设置粗体 
#define PRINT_ATTR_LIG      "\033[2m"       //设置一半亮度(模拟彩色显示器的颜色) 
#define PRINT_ATTR_LIN      "\033[4m"       //设置下划线(模拟彩色显示器的颜色) 
#define PRINT_ATTR_GLI      "\033[5m"       //设置闪烁 
#define PRINT_ATTR_REV      "\033[7m"       //设置反向图象 
#define PRINT_ATTR_THI      "\033[22m"      //设置一般密度 
#define PRINT_ATTR_ULIN     "\033[24m"      //关闭下划线 
#define PRINT_ATTR_UGLI     "\033[25m"      //关闭闪烁 
#define PRINT_ATTR_UREV     "\033[27m"      //关闭反向图象

// printf("\033[字背景颜色;字体颜色m字符串\033[0m" );

typedef struct {
    HyLogSaveConfig_t save_config;

    char *buf;
    hy_u32_t cur_len;
} _log_context_t;

static _log_context_t *context = NULL;

void HyLogHex(const char *name, uint32_t line,
        const void *_buf, uint32_t len, int32_t flag)
{
    if (len <= 0) {
        return;
    }
    uint8_t *buf = (uint8_t *)_buf;

    hy_u8_t cnt = 0;
    printf("[%s %ld]len: %ld \r\n", name, line, len);
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

static inline void _output_set_color(HyLogLevel_t level, hy_u32_t *ret)
{
    hy_char_t *color[HY_LOG_LEVEL_MAX][2] = {
        {"F", PRINT_FONT_RED},
        {"E", PRINT_FONT_RED},
        {"W", PRINT_FONT_GRE},
        {"I", ""},
        {"D", ""},
        {"T", ""},
    };

    *ret += snprintf(context->buf + *ret,
            context->save_config.buf_len - *ret,
            "%s[%s]",
            color[level][1], color[level][0]);
}

static inline void _output_reset_color(HyLogLevel_t level, hy_u32_t *ret)
{
    *ret += snprintf(context->buf + *ret,
            context->save_config.buf_len - *ret,
            "%s",
            PRINT_ATTR_RESET);
}

void HyLogWrite(HyLogLevel_t level, const char *err_str, 
        const char *file, const char *func,
        uint32_t line, char *fmt, ...)
{
    if (context && context->save_config.level >= level) {
        #define _SHORT_FILE_LEN_MAX (32)
        char short_file[_SHORT_FILE_LEN_MAX] = {0};
        hy_u32_t ret = 0;
        hy_u32_t buf_len = context->save_config.buf_len;

        memset(context->buf, '\0', buf_len);

        if (context->save_config.color_enable) {
            _output_set_color(level, &ret);
        }

        // 0x5c == \, 去除windows分界符
        HyStrCopyRight2(file, short_file, _SHORT_FILE_LEN_MAX, '/', 0x5c);
        ret += snprintf(context->buf + ret,
                buf_len - ret, "[%s:%"PRId32"][%s] ",
                short_file, line, func); 

        va_list args;
        va_start(args, fmt);
        ret += vsnprintf(context->buf + ret, buf_len - ret, fmt, args);
        va_end(args);

        if (context->save_config.color_enable) {
            _output_reset_color(level, &ret);
        }

        printf("%s", (char *)context->buf);
    }
}

void HyLogDestroy(void **handle)
{
    if (context) {
        if (context->buf) {
            HY_MEM_FREE_PP(&context->buf);
        }

        HY_MEM_FREE_PP(&context);
    }
}

void *HyLogCreate(HyLogConfig_t *config)
{
    HY_ASSERT_RET_VAL(!config, NULL);

    do {
        context = HY_MEM_MALLOC_BREAK(_log_context_t *, sizeof(*context));
        context->buf = HY_MEM_MALLOC_BREAK(char *, config->save_config.buf_len);

        HY_MEMCPY(&context->save_config, &config->save_config, sizeof(config->save_config));

        return context;
    } while (0);

    HyLogDestroy((void **)&context);
    return NULL;
}

