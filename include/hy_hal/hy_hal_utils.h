/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_hal_utils.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/10 2021 20:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/10 2021      create the file
 * 
 *     last modified: 29/10 2021 20:41
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_HAL_UTILS_H_
#define __LIBHY_HAL_INCLUDE_HY_HAL_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __compiler_offsetof
#define HY_OFFSETOF(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define HY_OFFSETOF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define HY_CONTAINER_OF(ptr, type, member) ({                       \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);        \
        (type *)( (char *)__mptr - HY_OFFSETOF(type,member) );})

#define HyHalUtilsArrayCnt(array) (int32_t)(sizeof((array)) / sizeof((array)[0]))

#ifdef __cplusplus
}
#endif

#endif

