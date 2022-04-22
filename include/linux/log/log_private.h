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

typedef struct {
    HyLogAddiInfo_s     *addi_info;
    void                *dynamic_array_h;
} _thread_private_data_s;

#ifdef __cplusplus
}
#endif

#endif

