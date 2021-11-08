/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_gpio.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 16:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 16:43
 */
#include <stdio.h>

#include "hy_gpio.h"
#include "gpio_2_str.h"

#include "at32f4xx_gpio.h"
#include "at32f4xx_rcc.h"

#include "hy_type.h"
#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

#define _DEFINE_GROUP()                                         \
    GPIO_Type *group[] = {                                      \
        GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,                      \
    }

#define _DEFINE_PIN()                                           \
    hy_u16_t pin[] = {                                          \
        GPIO_Pins_0, GPIO_Pins_1, GPIO_Pins_2, GPIO_Pins_3,     \
        GPIO_Pins_4, GPIO_Pins_5, GPIO_Pins_6, GPIO_Pins_7,     \
        GPIO_Pins_8, GPIO_Pins_9, GPIO_Pins_10, GPIO_Pins_11,   \
        GPIO_Pins_12, GPIO_Pins_13, GPIO_Pins_14, GPIO_Pins_15, \
        GPIO_Pins_All,                                          \
    }

HyGpioLevel_t HyGpioGetLevel(HyGpio_t *gpio)
{
    HY_ASSERT_VAL_RET_VAL(!gpio, HY_GPIO_LEVEL_MAX);

    _DEFINE_GROUP();
    _DEFINE_PIN();

    return GPIO_ReadInputDataBit(group[gpio->group], pin[gpio->pin]);
}

void HyGpioSetLevel(HyGpio_t *gpio, HyGpioLevel_t level)
{
    HY_ASSERT_VAL_RET(!gpio);
    LOGI("pin<%s, %s>, level: %s \n", HY_GPIO_GROUP_2_STR(gpio->group), HY_GPIO_PIN_2_STR(gpio->pin), HY_GPIO_LEVEL_2_STR(level));

    _DEFINE_GROUP();
    _DEFINE_PIN();

    if (level == HY_GPIO_LEVEL_HIGH) {
        group[gpio->group]->BSRE = pin[gpio->pin];
    } else {
        group[gpio->group]->BRE = pin[gpio->pin];
    }
}

void HyGpioSetLevelToggle(HyGpio_t *gpio)
{
    HY_ASSERT_VAL_RET(!gpio);
    LOGI("pin<%s, %s> toggle \n", HY_GPIO_GROUP_2_STR(gpio->group), HY_GPIO_PIN_2_STR(gpio->pin));

    _DEFINE_GROUP();
    _DEFINE_PIN();

    group[gpio->group]->OPTDT ^= pin[gpio->pin];
}

static void _init_PA_pin(HyGpio_t *gpio, GPIO_InitType *GPIO_InitStructure)
{
    switch (gpio->pin) {
        case HY_GPIO_PIN_0:
            GPIO_InitStructure->GPIO_Mode       = GPIO_Mode_IN_PD;
            break;
        case HY_GPIO_PIN_9:
            GPIO_InitStructure->GPIO_MaxSpeed   = GPIO_MaxSpeed_50MHz;
            GPIO_InitStructure->GPIO_Mode       = GPIO_Mode_AF_PP;
            break;
        case HY_GPIO_PIN_10:
            GPIO_InitStructure->GPIO_Mode       = GPIO_Mode_IN_FLOATING;
            break;
        default:
            break;
    }
}

static void _init_PB_pin(HyGpio_t *gpio, GPIO_InitType *GPIO_InitStructure)
{
}

static void _init_PC_pin(HyGpio_t *gpio, GPIO_InitType *GPIO_InitStructure)
{
    switch (gpio->pin) {
        case HY_GPIO_PIN_13:
            GPIO_InitStructure->GPIO_Mode = GPIO_Mode_IN_PD;
            break;
        default:
            break;
    }
}

static void _init_PD_pin(HyGpio_t *gpio, GPIO_InitType *GPIO_InitStructure)
{
    switch (gpio->pin) {
        case HY_GPIO_PIN_13:
        case HY_GPIO_PIN_14:
        case HY_GPIO_PIN_15:
            GPIO_InitStructure->GPIO_Mode       = GPIO_Mode_OUT_PP;
            GPIO_InitStructure->GPIO_MaxSpeed   = GPIO_MaxSpeed_50MHz;	
            break;
        default:
            break;
    }
}

static void _init_PE_pin(HyGpio_t *gpio, GPIO_InitType *GPIO_InitStructure)
{

}

static void _init_gpio_com(HyGpio_t *gpio)
{
    hy_u32_t rcc[] = {
        RCC_APB2PERIPH_GPIOA,
        RCC_APB2PERIPH_GPIOB,
        RCC_APB2PERIPH_GPIOC,
        RCC_APB2PERIPH_GPIOD,
    };

    _DEFINE_GROUP();
    _DEFINE_PIN();

    RCC_APB2PeriphClockCmd(rcc[gpio->group], ENABLE);

    GPIO_InitType GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pins = pin[gpio->pin];

    void (*_init_pin[HY_GPIO_GROUP_MAX])(HyGpio_t *, GPIO_InitType *) = {
        _init_PA_pin, _init_PB_pin, _init_PC_pin, _init_PD_pin, _init_PE_pin,
    };
    if (_init_pin[gpio->group]) {
        _init_pin[gpio->group](gpio, &GPIO_InitStructure);
    }

    GPIO_Init(group[gpio->group], &GPIO_InitStructure);
}

void HyGpioSetInput(HyGpio_t *gpio)
{
    HY_ASSERT_VAL_RET(!gpio);
    LOGI("pin<%s, %s> \n", HY_GPIO_GROUP_2_STR(gpio->group), HY_GPIO_PIN_2_STR(gpio->pin));

    _init_gpio_com(gpio);
}

void HyGpioSetOutput(HyGpio_t *gpio, HyGpioLevel_t level)
{
    HY_ASSERT_VAL_RET(!gpio);

    _init_gpio_com(gpio);

    HyGpioSetLevel(gpio, level);
}

