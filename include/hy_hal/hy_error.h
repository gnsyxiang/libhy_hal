/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_error.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/10 2021 09:04
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/10 2021      create the file
 * 
 *     last modified: 26/10 2021 09:04
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_ERROR_H_
#define __LIBHY_HAL_INCLUDE_HY_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HY_ERR_FAILD = -1,
    HY_ERR_OK = 0,

    HY_ERR_MAX,
} HyErr_t;

#ifdef __cplusplus
}
#endif

#endif

