/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    log_private.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/04 2022 11:58
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/04 2022      create the file
 * 
 *     last modified: 22/04 2022 11:58
 */
#ifndef __LIBHY_HAL_INCLUDE_LOG_PRIVATE_H_
#define __LIBHY_HAL_INCLUDE_LOG_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <sys/time.h>

#include "format_cb.h"

#define HY_LOG_DEBUG

#define SOCKET_IPC_SERVER_NAME_LEN_MAX      (64)
#define LOG_IPC_NAME                        "log_ipc_socket"
#define _ARRAY_CNT(array) (hy_u32_t)(sizeof((array)) / sizeof((array)[0]))

typedef struct {
    format_cb_t         *format_cb;
    hy_u32_t            format_cb_cnt;

    dynamic_array_s     *dynamic_array;
    HyLogAddiInfo_s     *addi_info;
} log_write_info_s;

#ifdef HY_LOG_DEBUG
#define log_time(_buf, _buf_len)                                                \
    do {                                                                        \
        time_t t = 0;                                                           \
        struct tm tm;                                                           \
        struct timeval tv;                                                      \
        t = time(NULL);                                                         \
        localtime_r(&t, &tm);                                                   \
        gettimeofday(&tv, NULL);                                                \
        snprintf(_buf, _buf_len, "[%04d-%02d-%02d_%02d:%02d:%02d.%03d]",        \
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,                   \
                tm.tm_hour, tm.tm_min, tm.tm_sec, (hy_u32_t)tv.tv_usec / 1000); \
    } while (0)

#define log_error(fmt, ...)                                             \
    do {                                                                \
        char buf[32] = {0};                                             \
        log_time(buf, sizeof(buf));                                     \
        printf("%s[%s:%d](errno: %d, errstr: %s)",                      \
                buf, __func__, __LINE__, errno, strerror(errno));       \
        printf(fmt, ##__VA_ARGS__);                                     \
    } while (0)

#define log_info(fmt, ...)                                              \
    do {                                                                \
        char buf[32] = {0};                                             \
        log_time(buf, sizeof(buf));                                     \
        printf("%s[%s:%d]", buf, __func__, __LINE__);                   \
        printf(fmt, ##__VA_ARGS__);                                     \
    } while (0)

#else
#define log_error(fmt, ...)
#define log_info(fmt, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif

