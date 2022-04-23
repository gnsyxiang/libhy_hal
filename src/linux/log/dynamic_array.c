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
#include <stdlib.h>
#include <assert.h>

#include "log_private.h"

#include "dynamic_array.h"

#define HY_MEM_ALIGN(len, align)    (((len) + (align) - 1) & ~((align) - 1))
#define HY_MEM_ALIGN4(len)          HY_MEM_ALIGN(len, 4)
#define HY_MEM_ALIGN4_UP(len)       (HY_MEM_ALIGN(len, 4) + HY_MEM_ALIGN4(1))

hy_s32_t dynamic_array_read(dynamic_array_s *dynamic_array,
        void *buf, hy_u32_t len)
{
    assert(dynamic_array);

    if (len > dynamic_array->cur_len) {
        len = dynamic_array->cur_len;
    }

    memcpy(buf, dynamic_array->buf, len);
    dynamic_array->read_pos += len;

    return len;
}

static hy_s32_t _dynamic_array_extend(dynamic_array_s *dynamic_array,
        hy_u32_t increment)
{
    hy_u32_t extend_len = 0;
    hy_s32_t ret = 0;
    void *ptr = NULL;

    if (dynamic_array->len >= dynamic_array->max_len) {
        log_info("can't extend size, len: %d, max_len: %d \n",
                dynamic_array->len, dynamic_array->max_len);
        return -1;
    }

    if (dynamic_array->len + increment <= dynamic_array->max_len) {
        extend_len = HY_MEM_ALIGN4_UP(dynamic_array->len + increment + 1);
    } else {
        extend_len = dynamic_array->max_len;
        ret = 1;
        log_info("extend to max len, len: %d, extend_len: %d \n",
                dynamic_array->len, extend_len);
    }

    ptr = realloc(dynamic_array->buf, extend_len);
    if (!ptr) {
        log_error("realloc failed \n");
        return -1;
    }

    dynamic_array->buf = ptr;
    dynamic_array->len = extend_len;

    return ret;
}

hy_s32_t dynamic_array_write_vprintf(dynamic_array_s *dynamic_array,
        const char *format, va_list *args)
{
    #define _get_dynamic_info()                                         \
        do {                                                            \
            va_copy(ap, *args);                                         \
            free_len = dynamic_array->len - dynamic_array->cur_len - 1; \
            ptr = dynamic_array->buf + dynamic_array->write_pos;        \
        } while (0)

    va_list ap;
    hy_s32_t free_len;
    hy_s32_t ret;
    char *ptr = NULL;

    _get_dynamic_info();
    ret = vsnprintf(ptr, free_len, format, ap);
    if (ret < 0) {
        log_error("vsnprintf failed \n");
        ret = -1;
    } else if (ret >= 0) {
        if (ret < free_len) {
            dynamic_array->cur_len += ret;
            dynamic_array->write_pos += ret;
        } else {
            ret = _dynamic_array_extend(dynamic_array,
                    ret - (dynamic_array->len - dynamic_array->cur_len));
            if (-1 == ret) {
                log_info("_dynamic_array_extend failed \n");
            } else if (ret >= 0) {
                _get_dynamic_info();
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
    #define _write_data_com(_buf, _len)                             \
        do {                                                        \
            char *ptr = NULL;                                       \
            ptr = dynamic_array->buf + dynamic_array->write_pos;    \
            memcpy(ptr, _buf, _len);                                \
            dynamic_array->cur_len      += _len;                    \
            dynamic_array->write_pos    += _len;                    \
        } while (0)

    assert(dynamic_array);
    hy_s32_t ret = 0;

    if (dynamic_array->len - dynamic_array->cur_len > len) {
        _write_data_com(buf, len);
    } else {
        ret = _dynamic_array_extend(dynamic_array,
                len - (dynamic_array->len - dynamic_array->cur_len));
        if (-1 == ret) {
            log_info("_dynamic_array_extend failed \n");
            len = -1;
        } else if (0 == ret) {
            _write_data_com(buf, len);
        } else {
            // 3 for "..." 1 for "\0"
            len = dynamic_array->len - dynamic_array->cur_len - 3 - 1;
            _write_data_com(buf, len);

            log_info("truncated data \n");
            _write_data_com("...", 3);
            len += 3;
        }
    }

    return len;
}

void dynamic_array_destroy(dynamic_array_s **dynamic_array_pp)
{
    dynamic_array_s *dynamic_array = *dynamic_array_pp;

    if (!dynamic_array_pp || !*dynamic_array_pp) {
        log_info("the param is error \n");
        return;
    }

    log_info("dynamic array destroy, dynamic_array: %p \n", dynamic_array);
    free(dynamic_array->buf);
    free(dynamic_array);
    *dynamic_array_pp = NULL;
}

dynamic_array_s *dynamic_array_create(hy_u32_t min_len, hy_u32_t max_len)
{
    dynamic_array_s *dynamic_array = NULL;

    if (min_len == 0 || max_len == 0 || min_len > max_len) {
        log_info("the param is error \n");
        return NULL;
    }

    do {
        dynamic_array = calloc(1, sizeof(*dynamic_array));
        if (!dynamic_array) {
            log_error("calloc failed \n");
            break;
        }

        dynamic_array->buf =calloc(1, min_len);
        if (!dynamic_array->buf) {
            log_error("calloc failed \n");
            break;
        }

        dynamic_array->max_len    = max_len;
        dynamic_array->min_len    = min_len;
        dynamic_array->len        = dynamic_array->min_len;
        dynamic_array->write_pos  = dynamic_array->read_pos = 0;

        log_info("dynamic array create, dynamic_array: %p \n", dynamic_array);
        return dynamic_array;
    } while (0);

    log_info("dynamic array create failed \n");
    dynamic_array_destroy(&dynamic_array);
    return NULL;
}

