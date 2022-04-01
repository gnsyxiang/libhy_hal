/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_string.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/10 2021 08:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/10 2021      create the file
 * 
 *     last modified: 26/10 2021 08:16
 */
#include <stdio.h>

#include "hy_string.h"

#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_log.h"

void HyStrCopyRight(const char *src, char *dst, hy_u32_t len, char ch)
{
    HY_ASSERT_RET(!src || !dst);

    HY_MEMSET(dst, len);

    hy_u32_t src_len = HY_STRLEN(src);
    hy_u32_t cnt = 0;
    const char *str = src + src_len - 1;
    while (*str && cnt < src_len) {
        if (*str == ch) {
            break;
        }
        str--;
        cnt++;
    }

    if (cnt >= len) {
        cnt = len - 1;
    }

    HY_MEMCPY(dst, str + 1, cnt);
}

void HyStrCopyRight2(const char *src, char *dst, hy_u32_t len, char ch, char ch2)
{
    HY_ASSERT_RET(!src || !dst);

    HY_MEMSET(dst, len);

    hy_u32_t src_len = HY_STRLEN(src);
    hy_u32_t cnt = 0;
    const char *str = src + src_len - 1;
    while (*str && cnt < src_len) {
        if (*str == ch || *str == ch2) {
            break;
        }
        str--;
        cnt++;
    }

    if (cnt >= len) {
        cnt = len - 1;
    }

    HY_MEMCPY(dst, str + 1, cnt);
}

