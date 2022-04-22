/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    dynamic_array.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/04 2022 09:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/04 2022      create the file
 * 
 *     last modified: 22/04 2022 09:06
 */
#include <stdio.h>

#include "hy_type.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"

#include "dynamic_array.h"

hy_s32_t dynamic_array_read(dynamic_array_s *dynamic_array,
        void *buf, hy_u32_t len)
{
    HY_ASSERT(dynamic_array);

    if (len > dynamic_array->cur_len) {
        len = dynamic_array->cur_len;
    }

    HY_MEMCPY(buf, dynamic_array->buf, len);

    return len;
}

hy_s32_t dynamic_array_write(dynamic_array_s *dynamic_array,
        const void *buf, hy_u32_t len)
{
    HY_ASSERT(dynamic_array);
    hy_u32_t extend_len = 0;
    void *ptr = NULL;

    if (dynamic_array->len - dynamic_array->cur_len > len) {
        HY_MEMCPY(dynamic_array->buf + dynamic_array->cur_len, buf, len);
        dynamic_array->cur_len += len;
    } else {
        while (extend_len != dynamic_array->max_len) {
            extend_len = 2 * dynamic_array->len;
            if (extend_len > dynamic_array->max_len) {
                extend_len = dynamic_array->max_len;
            }

            ptr = realloc(dynamic_array->buf, extend_len);
            if (!ptr) {
                LOGES("realloc failed \n");
                len = -1;
                break;
            }

            dynamic_array->buf = ptr;
            dynamic_array->len = extend_len;

            if (dynamic_array->len - dynamic_array->cur_len > len) {
                HY_MEMCPY(dynamic_array->buf + dynamic_array->cur_len, buf, len);
                dynamic_array->cur_len += len;
                break;
            }
        }

        if (extend_len == dynamic_array->max_len) {
            len = dynamic_array->len - dynamic_array->cur_len - 3 - 1; // 3 for "..." 1 for "\0"
            HY_MEMCPY(dynamic_array->buf + dynamic_array->cur_len, buf, len);
            dynamic_array->cur_len += len;

            HY_MEMCPY(dynamic_array->buf + dynamic_array->cur_len, "...", 3);
        }
    }

    return len;
}

void dynamic_array_destroy(dynamic_array_s **dynamic_array_pp)
{
    dynamic_array_s *dynamic_array = *dynamic_array_pp;

    HY_MEM_FREE_PP(&dynamic_array->buf);

    LOGI("dynamic array destroy, dynamic_array: %p \n", dynamic_array);
    HY_MEM_FREE_PP(dynamic_array_pp);
}

dynamic_array_s *dynamic_array_create(hy_u32_t min_len, hy_u32_t max_len)
{
    HY_ASSERT_RET_VAL(min_len == 0 || max_len == 0 || min_len > max_len, NULL);
    dynamic_array_s *dynamic_array = NULL;

    do {
        dynamic_array = HY_MEM_MALLOC_BREAK(dynamic_array_s *, sizeof(*dynamic_array));

        dynamic_array->max_len    = max_len;
        dynamic_array->min_len    = min_len;
        dynamic_array->len        = dynamic_array->min_len;

        dynamic_array->buf = HY_MEM_MALLOC_BREAK(char *, dynamic_array->len);

        return dynamic_array;
    } while (0);

    dynamic_array_destroy(&dynamic_array);
    return NULL;
}

