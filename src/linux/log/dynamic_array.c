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
#include <stdarg.h>

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
    dynamic_array->read_pos += len;

    return len;
}

static hy_s32_t dynamic_array_extend(dynamic_array_s *dynamic_array,
        hy_u32_t increment)
{
    hy_u32_t extend_len = 0;
    hy_s32_t ret = 0;
    void *ptr = NULL;

    if (dynamic_array->len >= dynamic_array->max_len) {
        printf("can't extend size \n");
        return -1;
    }

    if (dynamic_array->len + increment <= dynamic_array->max_len) {
        extend_len = HY_MEM_ALIGN4_UP(dynamic_array->len + increment + 1);
    } else {
        extend_len = dynamic_array->max_len;
        ret = 1;
    }

    ptr = realloc(dynamic_array->buf, extend_len);
    if (!ptr) {
        LOGES("realloc failed \n");
        return -1;
    }

    dynamic_array->buf = ptr;
    dynamic_array->len = extend_len;

    return ret;
}

hy_s32_t dynamic_array_write_vprintf(dynamic_array_s *dynamic_array,
        const char *format, va_list *args)
{
    va_list ap;
    hy_s32_t free_len;
    hy_s32_t ret;
    char *ptr = dynamic_array->buf + dynamic_array->write_pos;

    va_copy(ap, *args);
    free_len = dynamic_array->len - dynamic_array->cur_len - 1;
    ret = vsnprintf(ptr, free_len, format, ap);
    if (ret < 0) {
        printf("vsnprintf failed \n");
        ret = -1;
    } else if (ret >= 0) {
        if (ret < free_len) {
            dynamic_array->cur_len += ret;
            dynamic_array->write_pos += ret;
        } else {
            ret = dynamic_array_extend(dynamic_array,
                    ret - (dynamic_array->len - dynamic_array->cur_len));
            if (-1 == ret) {
                printf("dynamic_array_extend failed \n");
            } else if (ret >= 0) {
                free_len = dynamic_array->len - dynamic_array->cur_len - 1;
                ptr = dynamic_array->buf + dynamic_array->write_pos;
                ret = vsnprintf(ptr, free_len, format, ap);
                dynamic_array->cur_len += ret;
                dynamic_array->write_pos += ret;

                /* @fixme: <22-04-23, uos> 做进一步判断 */
            }
        }
    }

    return ret;
}

hy_s32_t dynamic_array_write(dynamic_array_s *dynamic_array,
        const void *buf, hy_u32_t len)
{
    #define _write_data(_buf, _len)                                 \
        do {                                                        \
            char *ptr = NULL;                                       \
            ptr = dynamic_array->buf + dynamic_array->write_pos;    \
            HY_MEMCPY(ptr, _buf, _len);                             \
            dynamic_array->cur_len      += _len;                    \
            dynamic_array->write_pos    += _len;                    \
        } while (0)

    HY_ASSERT(dynamic_array);
    hy_s32_t ret = 0;

    if (dynamic_array->len - dynamic_array->cur_len > len) {
        _write_data(buf, len);
    } else {
        ret = dynamic_array_extend(dynamic_array,
                len - (dynamic_array->len - dynamic_array->cur_len));
        if (-1 == ret) {
            printf("dynamic_array_extend failed \n");
            len = -1;
        } else if (0 == ret) {
            _write_data(buf, len);
        } else {
            len = dynamic_array->len - dynamic_array->cur_len - 3 - 1; // 3 for "..." 1 for "\0"
            _write_data(buf, len);

            _write_data("...", 3);
            len += 3;
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
        dynamic_array->write_pos  = dynamic_array->read_pos = 0;

        dynamic_array->buf = HY_MEM_MALLOC_BREAK(char *, dynamic_array->len);

        return dynamic_array;
    } while (0);

    dynamic_array_destroy(&dynamic_array);
    return NULL;
}

