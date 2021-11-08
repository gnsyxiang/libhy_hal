/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    gpio_2_str.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 17:04
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 17:04
 */
#ifndef __LIBHY_HAL_INCLUDE_GPIO_2_STR_H_
#define __LIBHY_HAL_INCLUDE_GPIO_2_STR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _HY_GPIO_GROUP_2_STR(_buf, val)                                     \
    ({                                                                      \
     if      ((val) == HY_GPIO_GROUP_PA)    _buf = "HY_GPIO_GROUP_PA";      \
     else if ((val) == HY_GPIO_GROUP_PB)    _buf = "HY_GPIO_GROUP_PB";      \
     else if ((val) == HY_GPIO_GROUP_PC)    _buf = "HY_GPIO_GROUP_PC";      \
     else if ((val) == HY_GPIO_GROUP_PD)    _buf = "HY_GPIO_GROUP_PD";      \
     else if ((val) == HY_GPIO_GROUP_PE)    _buf = "HY_GPIO_GROUP_PE";      \
     else                                   _buf = "HY_GPIO_GROUP_MAX";     \
     _buf;                                                                  \
     })
#define HY_GPIO_GROUP_2_STR(val) ({char *_buf = NULL; _HY_GPIO_GROUP_2_STR(_buf, val);})

#define _HY_GPIO_PIN_2_STR(_buf, val)                                       \
    ({                                                                      \
     if      ((val) == HY_GPIO_PIN_0)       _buf = "HY_GPIO_PIN_0";         \
     else if ((val) == HY_GPIO_PIN_1)       _buf = "HY_GPIO_PIN_1";         \
     else if ((val) == HY_GPIO_PIN_2)       _buf = "HY_GPIO_PIN_2";         \
     else if ((val) == HY_GPIO_PIN_3)       _buf = "HY_GPIO_PIN_3";         \
     else if ((val) == HY_GPIO_PIN_4)       _buf = "HY_GPIO_PIN_4";         \
     else if ((val) == HY_GPIO_PIN_5)       _buf = "HY_GPIO_PIN_5";         \
     else if ((val) == HY_GPIO_PIN_6)       _buf = "HY_GPIO_PIN_6";         \
     else if ((val) == HY_GPIO_PIN_7)       _buf = "HY_GPIO_PIN_7";         \
     else if ((val) == HY_GPIO_PIN_8)       _buf = "HY_GPIO_PIN_8";         \
     else if ((val) == HY_GPIO_PIN_9)       _buf = "HY_GPIO_PIN_9";         \
     else if ((val) == HY_GPIO_PIN_10)      _buf = "HY_GPIO_PIN_10";        \
     else if ((val) == HY_GPIO_PIN_11)      _buf = "HY_GPIO_PIN_11";        \
     else if ((val) == HY_GPIO_PIN_12)      _buf = "HY_GPIO_PIN_12";        \
     else if ((val) == HY_GPIO_PIN_13)      _buf = "HY_GPIO_PIN_13";        \
     else if ((val) == HY_GPIO_PIN_14)      _buf = "HY_GPIO_PIN_14";        \
     else if ((val) == HY_GPIO_PIN_15)      _buf = "HY_GPIO_PIN_15";        \
     else                                   _buf = "HY_GPIO_PIN_MAX";       \
     _buf;                                                                  \
     })
#define HY_GPIO_PIN_2_STR(val) ({char *_buf = NULL; _HY_GPIO_PIN_2_STR(_buf, val);})

#define _HY_GPIO_LEVEL_2_STR(_buf, val)                                     \
    ({                                                                      \
     if      ((val) == HY_GPIO_LEVEL_LOW)   _buf = "HY_GPIO_LEVEL_LOW";     \
     else if ((val) == HY_GPIO_LEVEL_HIGH)  _buf = "HY_GPIO_LEVEL_HIGH";    \
     else                                   _buf = "HY_GPIO_LEVEL_MAX";     \
     _buf;                                                                  \
     })
#define HY_GPIO_LEVEL_2_STR(val) ({char *_buf = NULL; _HY_GPIO_LEVEL_2_STR(_buf, val);})

#ifdef __cplusplus
}
#endif

#endif

