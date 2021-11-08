/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_gpio.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 16:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 16:41
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_GPIO_H_
#define __LIBHY_HAL_INCLUDE_HY_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HY_GPIO_GROUP_PA,
    HY_GPIO_GROUP_P0 = HY_GPIO_GROUP_PA,
    HY_GPIO_GROUP_PB,
    HY_GPIO_GROUP_P1 = HY_GPIO_GROUP_PB,
    HY_GPIO_GROUP_PC,
    HY_GPIO_GROUP_P2 = HY_GPIO_GROUP_PC,
    HY_GPIO_GROUP_PD,
    HY_GPIO_GROUP_P3 = HY_GPIO_GROUP_PD,
    HY_GPIO_GROUP_PE,
    HY_GPIO_GROUP_P4 = HY_GPIO_GROUP_PE,

    HY_GPIO_GROUP_MAX,
} HyGpioGroup_t;

typedef enum {
    HY_GPIO_PIN_0,
    HY_GPIO_PIN_1,
    HY_GPIO_PIN_2,
    HY_GPIO_PIN_3,
    HY_GPIO_PIN_4,
    HY_GPIO_PIN_5,
    HY_GPIO_PIN_6,
    HY_GPIO_PIN_7,
    HY_GPIO_PIN_8,
    HY_GPIO_PIN_9,
    HY_GPIO_PIN_10,
    HY_GPIO_PIN_11,
    HY_GPIO_PIN_12,
    HY_GPIO_PIN_13,
    HY_GPIO_PIN_14,
    HY_GPIO_PIN_15,

    HY_GPIO_PIN_MAX,
} HyGpioPin_t;

typedef enum {
    HY_GPIO_LEVEL_LOW,
    HY_GPIO_LEVEL_HIGH,

    HY_GPIO_LEVEL_MAX,
} HyGpioLevel_t;

typedef struct {
    HyGpioGroup_t   group;
    HyGpioPin_t     pin;
} HyGpio_t;

void HyGpioSetInput(HyGpio_t *gpio);
void HyGpioSetOutput(HyGpio_t *gpio, HyGpioLevel_t level);

HyGpioLevel_t HyGpioGetLevel(HyGpio_t *gpio);
void HyGpioSetLevel(HyGpio_t *gpio, HyGpioLevel_t level);
void HyGpioSetLevelToggle(HyGpio_t *gpio);

#ifdef __cplusplus
}
#endif

#endif

