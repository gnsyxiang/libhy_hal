/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_log4cplus.cpp
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 14:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 14:39
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "hy_log.h"

#include "hy_hal_utils.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_type.h"
#include "hy_assert.h"

#include <log4cplus/config.hxx>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/loggingmacros.h>

#define ALONE_DEBUG 1

using namespace log4cplus;

typedef struct {
    HyLogSaveConfig_t   save_config;

    char                *buf;
    Logger              root;
} _log_context_t;

static _log_context_t *context = nullptr;

void HyLogWrite(LogLevel level, const char *file,
        const char *func, hy_u32_t line, char *fmt, ...)
{
    if (context && context->root.isEnabledFor(level)) {
        memset(context->buf, '\0', context->save_config.buf_len);

        // 加线程锁
        va_list args;
        va_start(args, fmt);
        vsnprintf(context->buf, context->save_config.buf_len, fmt, args);
        va_end(args);

        context->root.forcedLog(level, context->buf, file, line, func);
    }
}

void HyLogHex(const char *name, uint32_t line,
        void *_buf, size_t len, int8_t flag)
{
    if (len <= 0) {
        return;
    }
    uint8_t *buf = (uint8_t *)_buf;

    hy_u8_t cnt = 0;
    printf("[%s %d]len: %zu \r\n", name, line, len);
    for (size_t i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a || buf[i] < 32 || buf[i] >= 127) {
                printf("%02x[ ]  ", (hy_u8_t)buf[i]);
            } else {
                printf("%02x[%c]  ", (hy_u8_t)buf[i], (hy_u8_t)buf[i]);
            }
        } else {
            printf("%02x ", (hy_u8_t)buf[i]);
        }
        cnt++;
        if (cnt == 16) {
            cnt = 0;
            printf("\r\n");
        }
    }
    printf("\r\n");
}

void HyLogDestroy(void **handle)
{
    HY_ASSERT_VAL_RET(!handle || !*handle);

    delete []context->buf;

    delete context;
    *handle = NULL;
}

void *HyLogCreate(HyLogConfig_t *config)
{
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    do {
        context = new _log_context_t();
        if (!context) {
            printf("new _log_context_t faild \n");
            break;
        }

        HY_MEMCPY(&context->save_config, &config->save_config, sizeof(config->save_config));

        context->buf = new char[context->save_config.buf_len];
        if (!context->buf) {
            printf("new char faild \n");
            break;
        }

        helpers::LogLog::getLogLog()->setInternalDebugging(false);
        // @fixme
        // PropertyConfigurator::doConfigure(log_config->config_file);

        context->root = Logger::getRoot();

        return context;
    } while(0);

    HyLogDestroy((void **)&context);
    return NULL;
}
