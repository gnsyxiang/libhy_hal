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
#include <stdarg.h>

#include "format_cb.h"
#include "log_private.h"
#include "dynamic_array.h"
#include "process_single.h"
#include "process_client.h"
#include "process_server.h"
#include "thread_specific_data.h"

#include "hy_log.h"

/* @fixme: <22-04-23, uos>
 * 两者大小不能相隔太近，
 * 否则出现内存申请错误(malloc(): unsorted double linked list corrupted)，
 * 测试条件: min_len = 60, max_len = 87
 */
#define _DYNAMIC_ARRAY_MIN_LEN  (256)
#define _DYNAMIC_ARRAY_MAX_LEN  (4 * 1024)

typedef struct {
    HyLogSaveConfig_s   save_c;

    format_cb_t         format_cb[FORMAT_LOG_CB_CNT];

    void                *write_h;
} _log_context_s;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static hy_s32_t _is_init = 0;
static _log_context_s _context;

HyLogLevel_e HyLogLevelGet(void)
{
    return _context.save_c.level;
}

void HyLogLevelSet(HyLogLevel_e level)
{
    _context.save_c.level = level;
}

void HyLogWrite(HyLogAddiInfo_s *addi_info, const char *fmt, ...)
{
    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &context->save_c;
    dynamic_array_s *dynamic_array = NULL;
    log_write_info_s log_write_info;
    va_list args;
    if (!context->write_h) {
        log_error("the write handle is NULL \n");
        return;
    }

    dynamic_array = thread_specific_data_fetch();
    if (!dynamic_array) {
        log_info("_thread_private_data_featch failed \n");
        return;
    }

    va_start(args, fmt);
    addi_info->fmt = fmt;
    addi_info->str_args = &args;
    log_write_info.format_cb        = context->format_cb;
    log_write_info.format_cb_cnt    = LOG_ARRAY_CNT(context->format_cb);
    log_write_info.dynamic_array    = dynamic_array;
    log_write_info.addi_info        = addi_info;
    switch (save_c->mode) {
        case HY_LOG_MODE_PROCESS_SINGLE:
            process_single_write(context->write_h, &log_write_info);
            break;
        case HY_LOG_MODE_PROCESS_CLIENT:
            process_client_write(context->write_h, &log_write_info);
            break;
        case HY_LOG_MODE_PROCESS_SERVER:
            process_server_write(context->write_h, &log_write_info);
            break;
        default:
            log_error("error mode \n");
            break;
    }
    va_end(args);
}

static void _thread_specific_data_reset_cb(void *handle)
{
    DYNAMIC_ARRAY_RESET((dynamic_array_s *)handle);
}

static void _thread_specific_data_destroy_cb(void *handle)
{
    if (!handle) {
        log_error("the param is error \n");
        return ;
    }
    dynamic_array_s *dynamic_array = handle;

    dynamic_array_destroy(&dynamic_array);
}

static void *_thread_specific_data_create_cb(void)
{
    return dynamic_array_create(_DYNAMIC_ARRAY_MIN_LEN, _DYNAMIC_ARRAY_MAX_LEN);
}

void HyLogDeInit(void)
{
    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &context->save_c;

    log_info("log context: %p destroy \n", context);

    switch (save_c->mode) {
        case HY_LOG_MODE_PROCESS_SINGLE:
            process_single_destroy(&context->write_h);
            break;
        case HY_LOG_MODE_PROCESS_CLIENT:
            process_client_destroy(&context->write_h);
            break;
        case HY_LOG_MODE_PROCESS_SERVER:
            process_server_destroy(&context->write_h);
            break;
        default:
            log_error("error mode \n");
            break;
    }

    thread_specific_data_destroy();
}

hy_s32_t HyLogInit(HyLogConfig_s *log_c)
{
    if (!log_c) {
        log_error("the param is error \n");
        return -1;
    }

    pthread_mutex_lock(&lock);
    if (_is_init) {
        log_error("The logging system has been initialized \n");
        pthread_mutex_unlock(&lock);
        return -1;
    }
    _is_init = 1;
    pthread_mutex_unlock(&lock);

    _log_context_s *context = &_context;
    HyLogSaveConfig_s *save_c = &log_c->save_c;

    do {
        memset(context, '\0', sizeof(*context));
        memcpy(&context->save_c, &log_c->save_c, sizeof(context->save_c));

        format_cb_register(context->format_cb, save_c->output_format);

        if (0 != thread_specific_data_create(
                    _thread_specific_data_create_cb,
                    _thread_specific_data_destroy_cb,
                    _thread_specific_data_reset_cb)) {
            log_error("thread_specific_data_create failed \n");
            break;
        }

        switch (log_c->save_c.mode) {
            case HY_LOG_MODE_PROCESS_SINGLE:
                context->write_h = process_single_create(log_c->fifo_len);
                break;
            case HY_LOG_MODE_PROCESS_CLIENT:
                context->write_h = process_client_create(log_c->fifo_len);
                break;
            case HY_LOG_MODE_PROCESS_SERVER:
                context->write_h = process_server_create(log_c->fifo_len);
                break;
            default:
                log_error("error mode \n");
                break;
        }
        if (!context->write_h) {
            log_error("create write handle failed \n");
            break;
        }

        log_info("log context: %p create \n", context);
        return 0;
    } while (0);

    log_error("log context: %p create failed \n", context);
    HyLogDeInit();
    return -1;
}

