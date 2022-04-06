/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_gpio.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/04 2022 15:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/04 2022      create the file
 * 
 *     last modified: 02/04 2022 15:13
 */
#include <stdio.h>

#include "hy_hal/hy_assert.h"
#include "hy_hal/hy_string.h"
#include "hy_hal/hy_mem.h"
#include "hy_log.h"

#include "hy_gpio.h"

#define _GPIO_EXPORT_PATH       "/sys/class/gpio/export"
#define _GPIO_UNEXPORT_PATH     "/sys/class/gpio/unexport"
#define _GPIO_DIRECTION_PATH    "/sys/class/gpio/gpio%d/direction"
#define _GPIO_VALUE_PATH        "/sys/class/gpio/gpio%d/value"

#define _HY_GPIO_DIR_OUT        "out"
#define _HY_GPIO_DIR_IN         "in"

#define _EXPORT_GPIO(_gpio)                                             \
    do {                                                                \
        FILE *fp = fopen(_GPIO_EXPORT_PATH, "w");                       \
        if (!fp) {                                                      \
            LOGES("open %s failed", _GPIO_EXPORT_PATH);                 \
            return -1;                                                  \
        }                                                               \
        fprintf(fp, "%d", _gpio->gpio);                                 \
        fclose(fp);                                                     \
    } while (0)

#define _UNEXPORT_GPIO(_gpio)                                           \
    do {                                                                \
        FILE *fp = fopen(_GPIO_UNEXPORT_PATH, "w");                     \
        if (!fp) {                                                      \
            LOGES("open %s failed", _GPIO_UNEXPORT_PATH);               \
            return -1;                                                  \
        }                                                               \
        fprintf(fp, "%d", _gpio->gpio);                                 \
        fclose(fp);                                                     \
    } while (0)

#define _SET_DIRECTION(_gpio, _direction)                               \
    do {                                                                \
        char buf[64] = {0};                                             \
        FILE *fp = NULL;                                                \
        snprintf(buf, sizeof(buf), _GPIO_DIRECTION_PATH, _gpio->gpio);  \
        fp = fopen(buf, "w");                                           \
        if (!fp) {                                                      \
            LOGES("open %s failed", buf);                               \
            return -1;                                                  \
        }                                                               \
        if (_direction) {                                               \
            fprintf(fp, "%s", "out");                                   \
        } else {                                                        \
            fprintf(fp, "%s", "in");                                    \
        }                                                               \
        fclose(fp);                                                     \
    } while (0)

#define _SET_VAL(_gpio, _val)                                           \
    do {                                                                \
        FILE *fp = NULL;                                                \
        char buf[64] = {0};                                             \
        snprintf(buf, sizeof(buf), _GPIO_VALUE_PATH, _gpio->gpio);      \
        fp = fopen(buf, "w");                                           \
        if (!fp) {                                                      \
            LOGES("open %s failed", buf);                               \
            return -1;                                                  \
        }                                                               \
        if (HY_GPIO_VAL_ON == _val) {                                   \
            if (_gpio->active_val == HY_GPIO_ACTIVE_VAL_1) {            \
                fprintf(fp, "%d", 1);                                   \
            } else {                                                    \
                fprintf(fp, "%d", 0);                                   \
            }                                                           \
        } else {                                                        \
            if (_gpio->active_val == HY_GPIO_ACTIVE_VAL_1) {            \
                fprintf(fp, "%d", 0);                                   \
            } else {                                                    \
                fprintf(fp, "%d", 1);                                   \
            }                                                           \
        }                                                               \
        fclose(fp);                                                     \
    } while (0)

#define _GET_VAL(_gpio, _val)                                           \
    do {                                                                \
        char buf[64] = {0};                                             \
        char ch;                                                        \
        FILE *fp = NULL;                                                \
        snprintf(buf, sizeof(buf), _GPIO_VALUE_PATH, _gpio->gpio);      \
        fp = fopen(buf, "r");                                           \
        if (!fp) {                                                      \
            LOGES("open %s failed\n", buf);                             \
            return -1;                                                  \
        }                                                               \
        fread(&ch, 1, 1, fp);                                           \
        if (ch != '1') {                                                \
            if (_gpio->active_val == HY_GPIO_ACTIVE_VAL_1) {            \
                _val = 1;                                               \
            } else {                                                    \
                _val = 0;                                               \
            }                                                           \
        } else {                                                        \
            if (_gpio->active_val == HY_GPIO_ACTIVE_VAL_1) {            \
                _val = 0;                                               \
            } else {                                                    \
                _val = 1;                                               \
            }                                                           \
        }                                                               \
        fclose(fp);                                                     \
    } while (0)

hy_s32_t HyGpioSetOutputVal(HyGpio_s *gpio, HyGpioVal_e val)
{
    _EXPORT_GPIO(gpio);
    _SET_DIRECTION(gpio, HY_GPIO_DIRECTION_OUT);
    _SET_VAL(gpio, val);
    _UNEXPORT_GPIO(gpio);

    return 0;
}

hy_s32_t HyGpioSetDirection(HyGpio_s *gpio, HyGpioDirection_e direction)
{
    _EXPORT_GPIO(gpio);
    _SET_DIRECTION(gpio, direction);
    _UNEXPORT_GPIO(gpio);

    return 0;
}

hy_s32_t HyGpioSetVal(HyGpio_s *gpio, HyGpioVal_e val)
{
    _EXPORT_GPIO(gpio);
    _SET_VAL(gpio, val);
    _UNEXPORT_GPIO(gpio);

    return 0;
}

hy_s32_t HyGpioGetVal(HyGpio_s *gpio, HyGpioVal_e *val)
{
    _EXPORT_GPIO(gpio);
    _GET_VAL(gpio, *val);
    _UNEXPORT_GPIO(gpio);

    return 0;
}

