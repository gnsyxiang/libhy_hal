/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    format_cb.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 10:18
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 10:18
 */
#ifndef __LIBHY_HAL_INCLUDE_FORMAT_CB_H_
#define __LIBHY_HAL_INCLUDE_FORMAT_CB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_log.h"
#include "dynamic_array.h"

#define FORMAT_LOG_CB_CNT       (7)
#define FORMAT_LOG_CB_TYPE      (2)

typedef hy_s32_t (*(format_cb_t[FORMAT_LOG_CB_TYPE]))
    (dynamic_array_s *dynamic_array, HyLogAddiInfo_s *addi_info);

typedef struct log_write_info_tag {
    format_cb_t         *format_cb;
    hy_u32_t            format_cb_cnt;

    dynamic_array_s     *dynamic_array;
    HyLogAddiInfo_s     *addi_info;
} log_write_info_s;

void format_cb_register(format_cb_t *format_cb, hy_u32_t format);

#ifdef __cplusplus
}
#endif

#endif

