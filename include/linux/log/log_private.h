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

#include "hy_log.h"
#include "dynamic_array.h"

#define FORMAT_LOG_CB_CNT       (6)
#define FORMAT_LOG_CB_TYPE      (2)

typedef hy_s32_t (*(format_log_cb_t[FORMAT_LOG_CB_TYPE]))
    (dynamic_array_s *dynamic_array, HyLogAddiInfo_s *addi_info);

typedef struct {
    format_log_cb_t     *format_log_cb;
    hy_u32_t            format_log_cb_cnt;

    dynamic_array_s     *dynamic_array;
    HyLogAddiInfo_s     *addi_info;

    char                *fmt;
    va_list             *str_args;
} log_write_info_s;

#ifdef __cplusplus
}
#endif

#endif

