/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_time.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/12 2021 19:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/12 2021      create the file
 * 
 *     last modified: 20/12 2021 19:16
 */
#include <stdio.h>

#include "hy_time.h"

#include "hy_log.h"
#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"

time_t HyTimeGetUTC(void)
{
    return time(NULL);
}

void HyTimeGetLocalTime(struct tm *tm)
{
    time_t t = time(NULL);
    localtime_r(&t, tm);    // localtime_r可重入，localtime不可重入
}

void HyTimeFormatLocalTime(char *buf, size_t len)
{
    struct tm tm;

    HY_MEMSET(buf, len);

    HyTimeGetLocalTime(&tm);

    snprintf(buf, len, "%04d-%02d-%02d_%02d-%02d-%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    // 跟上面的效果一样，strftime可以生成许多的格式
    // strftime(buf, len, "%Y-%m-%d_%H-%M-%S", &tm);
}

time_t HyTimeFormatTime2UTC(const char *data_time)
{
    struct tm tm;

    sscanf(data_time, "%04d-%02d-%02d_%02d-%02d-%02d",
            &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
            &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

    tm.tm_year  -= 1900;
    tm.tm_mon   -= 1;

    return mktime(&tm);
}
