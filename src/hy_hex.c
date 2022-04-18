/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_hex.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    12/04 2022 10:22
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        12/04 2022      create the file
 * 
 *     last modified: 12/04 2022 10:22
 */
#include <stdio.h>

#include "hy_hal/hy_type.h"
#include "hy_hal/hy_string.h"
#include "hy_hal/hy_assert.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_log.h"

#include "hy_hex.h"

void HyHex(const char *name, hy_u32_t line,
        const void *_buf, hy_u32_t len, hy_s32_t flag)
{
    if (len <= 0) {
        return;
    }
    const unsigned char *buf = (const unsigned char *)_buf;

    hy_u8_t cnt = 0;
    printf("[%s:%d] len: %d \r\n", name, line, len);
    for (hy_u32_t i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a
                    || buf[i] < 32 || buf[i] >= 127) {
                printf("%02x[ ]  ", buf[i]);
            } else {
                printf("%02x[%c]  ", buf[i], buf[i]);
            }
        } else {
            printf("%02x ", buf[i]);
        }
        cnt++;
        if (cnt == 16) {
            cnt = 0;
            printf("\r\n");
        }
    }
    printf("\r\n");
}

