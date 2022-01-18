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

#include <stdio.h>
#include <time.h>
#include <stdint.h>

/**
 * @brief 获取utc时间
 *
 * @return 微妙
 */
unsigned long long HyTimeGetUTCUs(void);

/**
 * @brief 获取utc时间
 *
 * @return 毫秒
 */
unsigned long long HyTimeGetUTCMs(void);

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

#ifdef __cplusplus
}
#endif

#endif
