/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_time.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2021 14:31
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2021      create the file
 * 
 *     last modified: 20/12 2021 14:31
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_TIME_H_
#define __LIBHY_HAL_INCLUDE_HY_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

#include "hy_hal/hy_type.h"

/**
 * @brief 获取utc时间
 *
 * @return 微妙
 */
hy_u64_t HyTimeGetUTCUs(void);

/**
 * @brief 获取utc时间
 *
 * @return 毫秒
 */
hy_u64_t HyTimeGetUTCMs(void);

/**
 * @brief 获取utc时间
 *
 * @return 秒数
 */
time_t HyTimeGetUTC(void);

/**
 * @brief 获取当前时间
 *
 * @param tm 当前时间
 */
void HyTimeGetLocalTime(struct tm *tm);

/**
 * @brief 获取并格式化当前时间
 *
 * @param buf 存储数组
 * @param len 数组的长度
 *
 * @note 2021-12-20_19-00-00，时分秒不用分号的原因是在fat32文件系统中无法识别
 */
void HyTimeFormatLocalTime(char *buf, size_t len);

/**
 * @brief 把格式化时间转成UTC时间
 *
 * @param data_time 被格式化的时间
 *
 * @return UTC时间
 *
 * @note 2021-12-20_19-00-00，时分秒不用分号的原因是在fat32文件系统中无法识别
 */
time_t HyTimeFormatTime2UTC(const char *data_time);

/**
 * @brief 获取当天的零点和24点的UTC值
 *
 * @param start 零点值
 * @param end 24点值
 */
void HyTimeGetCurDayRegion(const time_t cur_utc, time_t *start, time_t *end);

/**
 * @brief 延时us
 *
 * @param us 延时时间
 */
void HyTimeDelayUs(hy_u32_t us);

/**
 * @brief 延时ms
 *
 * @param ms 延时时间
 */
void HyTimeDelayMs(hy_u32_t ms);

/**
 * @brief 延时s
 *
 * @param s 延时时间
 */
void HyTimeDelayS(hy_u32_t s);

/**
 * @brief 转换时间
 *
 * @param ms ms
 *
 * @return 转换struct timespec
 */
struct timespec HyTimeGetTimespec(hy_u32_t ms);

#ifdef __cplusplus
}
#endif

#endif

