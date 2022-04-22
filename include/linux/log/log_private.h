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

#include "hy_log.h"
#include "dynamic_array.h"

typedef struct {
    HyLogAddiInfo_s     *addi_info;
    dynamic_array_s     *dynamic_array;
} _thread_private_data_s;

typedef hy_s32_t (*format_log_cb_t)(_thread_private_data_s *thread_private_data);

#ifdef __cplusplus
}
#endif

#endif

