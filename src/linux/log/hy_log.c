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

#include "log_private.h"
#include "dynamic_array.h"
#include "process_single.h"
#include "process_ipc_client.h"
#include "process_ipc_server.h"
#include "socket_ipc_server.h"

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

    pthread_key_t       thread_key;
    format_cb_t     format_cb[FORMAT_LOG_CB_CNT];

    void                *write_h;
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
    log_write_info.format_cb        = context->format_cb;
    log_write_info.format_cb_cnt    = _ARRAY_CNT(context->format_cb);
    log_write_info.dynamic_array    = dynamic_array;
    log_write_info.addi_info        = addi_info;
    switch (save_c->mode) {
        case HY_LOG_MODE_PROCESS_SINGLE:
            process_single_write(context->write_h, &log_write_info);
            break;
        case HY_LOG_MODE_PROCESS_CLIENT:
            process_ipc_client_write(context->write_h, &log_write_info);
            break;
        case HY_LOG_MODE_PROCESS_SERVER:
            process_ipc_server_write(context->write_h, &log_write_info);
            break;
        default:
            break;
    }
    va_end(args);
}

static void _log_thread_private_data_destroy(void)
{
    _thread_private_data_destroy(_thread_private_data_get());
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
            process_ipc_client_destroy(&context->write_h);
            break;
        case HY_LOG_MODE_PROCESS_SERVER:
            process_ipc_server_destroy(&context->write_h);
            break;
        default:
            break;
    }
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

    do {
        memset(context, '\0', sizeof(*context));
        memcpy(&context->save_c, &log_c->save_c, sizeof(context->save_c));

        format_cb_register(context->format_cb, save_c->output_format);

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
                context->write_h = process_single_create(log_c->fifo_len);
                break;
            case HY_LOG_MODE_PROCESS_CLIENT:
                context->write_h = process_ipc_client_create(log_c->fifo_len);
                break;
            case HY_LOG_MODE_PROCESS_SERVER:
                context->write_h = process_ipc_server_create(log_c->fifo_len);
                break;
            default:
                break;
        }
        if (!context->write_h) {
            log_error("create write handle failed \n");
            break;
        }

        _is_init = 1;
        log_info("log context: %p create \n", context);
        return 0;
    } while (0);

    log_error("log context: %p create failed \n", context);
    HyLogDeInit();
    return -1;
}

