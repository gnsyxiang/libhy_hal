/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_gpio.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/04 2022 15:12
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/04 2022      create the file
 * 
 *     last modified: 02/04 2022 15:12
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_GPIO_H_
#define __LIBHY_HAL_INCLUDE_HY_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

typedef enum {
    HY_GPIO_DIRECTION_IN,
    HY_GPIO_DIRECTION_OUT,
} HyGpioDirection_e;

/**
 * @brief 设置gpio为输出，同时设置val值
 *
 * @param gpio gpio
 * @param val 值
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioSetOutputVal(hy_u32_t gpio, hy_s32_t val);

/**
 * @brief 设置gpio的方向
 *
 * @param gpio gpio
 * @param direction 方向
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioSetDirection(hy_u32_t gpio, HyGpioDirection_e direction);

/**
 * @brief 设置gpio的值
 *
 * @param gpio gpio
 * @param val 值
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioSetVal(hy_u32_t gpio, hy_s32_t val);

/**
 * @brief 获取gpio的值
 *
 * @param gpio gpio
 * @param val 值
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioGetVal(hy_u32_t gpio, hy_s32_t *val);

#ifdef __cplusplus
}
#endif

#endif

