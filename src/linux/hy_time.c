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
#include <sys/time.h>

#include "hy_log.h"
#include "hy_type.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"

#include "hy_time.h"

hy_u64_t HyTimeGetUTCUs(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return ((hy_u64_t)tv.tv_sec * 1000 * 1000 + tv.tv_usec);
}

hy_u64_t HyTimeGetUTCMs(void)
{
    return HyTimeGetUTCUs() / 1000;
}

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

void HyTimeGetCurDayRegion(const time_t cur_utc, time_t *start, time_t *end)
{
    struct tm tm;
    localtime_r(&cur_utc, &tm);
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    *start = mktime(&tm);

    *end = 0;
    *end += *start + 86400 - 1;
}

static void _delay_com(hy_u32_t s, hy_u32_t us)
{
    struct timeval tv;
    tv.tv_sec   = s;
    tv.tv_usec  = us;

    int err;
    do {
        err = select(0, NULL, NULL, NULL, &tv);
    } while(err < 0 && errno == EINTR);
}

void HyTimeDelayUs(hy_u32_t us)
{
    _delay_com(0, us);
}

void HyTimeDelayMs(hy_u32_t ms)
{
    _delay_com(0, ms * 1000);
}

void HyTimeDelayS(hy_u32_t s)
{
    _delay_com(s, 0);
}

struct timespec HyTimeGetTimespec(hy_u32_t ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000;

    return ts;
}

