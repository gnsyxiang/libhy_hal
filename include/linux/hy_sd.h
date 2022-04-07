/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_sd.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/04 2022 18:49
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/04 2022      create the file
 * 
 *     last modified: 07/04 2022 18:49
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_SD_H_
#define __LIBHY_HAL_INCLUDE_HY_SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

typedef enum {
    HY_SD_STATE_UNINSERT,
    HY_SD_STATE_INSERT,

    HY_SD_STATE_MOUNT_OK,
    HY_SD_STATE_MOUNT_FAIL,
} HySdState_e;

hy_s32_t HySdGetPartitionsName(char *name, hy_u32_t name_len_max);

#ifdef __cplusplus
}
#endif

#endif

