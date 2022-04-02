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

/**
 * @brief gpio方向
 */
typedef enum {
    HY_GPIO_DIRECTION_IN,               ///< 输入
    HY_GPIO_DIRECTION_OUT,              ///< 输出
} HyGpioDirection_e;

/**
 * @brief 工作状态电平有效值
 */
typedef enum {
    HY_GPIO_ACTIVE_VAL_0,               ///< 工作状态低电平有效
    HY_GPIO_ACTIVE_VAL_1,               ///< 工作状态高电平有效
} HyGpioActiveVal_e;

/**
 * @brief gpio值状态
 */
typedef enum {
    HY_GPIO_VAL_OFF,                    ///< 非工作状态
    HY_GPIO_VAL_ON,                     ///< 工作状态
} HyGpioVal_e;

/**
 * @brief gpio结构体
 */
typedef struct {
    hy_s32_t            gpio;           ///< gpio
    HyGpioActiveVal_e   active_val;     ///< 工作状态的电平
} HyGpio_s;

/**
 * @brief 设置gpio为输出，同时设置val值
 *
 * @param gpio gpio
 * @param val 值
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioSetOutputVal(HyGpio_s *gpio, HyGpioVal_e val);

/**
 * @brief 设置gpio的方向
 *
 * @param gpio gpio
 * @param direction 方向
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioSetDirection(HyGpio_s *gpio, HyGpioDirection_e direction);

/**
 * @brief 设置gpio的值
 *
 * @param gpio gpio
 * @param val 值
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioSetVal(HyGpio_s *gpio, HyGpioVal_e val);

/**
 * @brief 获取gpio的值
 *
 * @param gpio gpio
 * @param val 值
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HyGpioGetVal(HyGpio_s *gpio, HyGpioVal_e *val);

#ifdef __cplusplus
}
#endif

#endif

