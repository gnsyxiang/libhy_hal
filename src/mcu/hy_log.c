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

#include "hy_assert.h"
#include "hy_string.h"
#include "hy_type.h"
#include "hy_mem.h"
#include "hy_printf.h"

#include "hy_log.h"

#define _LOG_BUF_LEN_MAX (512)

typedef struct {
    HyLogSaveConfig_s save_config;

    char buf[_LOG_BUF_LEN_MAX];
    hy_u32_t cur_len;
} _log_context_t;

static _log_context_t *context = NULL;

void HyLogHex(const char *name, hy_u32_t line,
        const void *_buf, hy_u32_t len, hy_s32_t flag)
{
    if (len <= 0) {
        return;
    }
    uint8_t *buf = (uint8_t *)_buf;

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

void HyLogWrite(HyLogLevel_e level, const char *err_str, 
        const char *file, hy_u32_t line, char *fmt, ...)
{
    printf("%s", (char *)context->buf);
}

void HyLogDestroy(void **handle)
{
    if (context) {
        HY_MEM_FREE_P(context);
    }
}

void *HyLogCreate(HyLogConfig_s *log_c)
{
    LOGT("log_c: %p \n", log_c);
    HY_ASSERT_RET_VAL(!log_c, NULL);

    do {
        context = HY_MEM_MALLOC_BREAK(_log_context_t *, sizeof(*context));

        HY_MEMCPY(&context->save_config,
                &log_c->save_config, sizeof(log_c->save_config));

        return context;
    } while (0);

    HyLogDestroy((void **)&context);
    return NULL;
}

