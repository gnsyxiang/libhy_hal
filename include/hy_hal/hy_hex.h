/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_hex.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/04 2022 10:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/04 2022      create the file
 * 
 *     last modified: 12/04 2022 10:20
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_HEX_H_
#define __LIBHY_HAL_INCLUDE_HY_HEX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"
#include "hy_hal/hy_hal_misc.h"

/**
 * @brief 打印二进制信息
 *
 * @param name 函数名
 * @param line 行号
 * @param buf 被打印数据
 * @param len 被打印数据的长度
 * @param flag 是否需要打印对应的ascii
 */
void HyHex(const char *name, hy_u32_t line,
       const void *buf, hy_u32_t len, hy_s32_t flag);

#define HY_HEX_ASCII(buf, len)  HyHex(HY_FILENAME, __LINE__, buf, len, 1)
#define HY_HEX(buf, len)        HyHex(HY_FILENAME, __LINE__, buf, len, 0)

#ifdef __cplusplus
}
#endif

#endif

