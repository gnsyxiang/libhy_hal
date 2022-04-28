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

#include "log_private.h"

#include "dynamic_array.h"

#define HY_MEM_ALIGN(len, align)    (((len) + (align) - 1) & ~((align) - 1))
#define HY_MEM_ALIGN4(len)          HY_MEM_ALIGN(len, 4)
#define HY_MEM_ALIGN4_UP(len)       (HY_MEM_ALIGN(len, 4) + HY_MEM_ALIGN4(1))

hy_s32_t dynamic_array_read(dynamic_array_s *context,
        void *buf, hy_u32_t len)
{
    assert(context);
    assert(buf);

    if (len > context->cur_len) {
        len = context->cur_len;
    }

    memcpy(buf, context->buf, len);
    context->read_pos += len;

    return len;
}

static hy_s32_t _dynamic_array_extend(dynamic_array_s *context,
        hy_u32_t increment)
{
    assert(context);
    hy_u32_t extend_len = 0;
    hy_s32_t ret = 0;
    void *ptr = NULL;

    if (context->len >= context->max_len) {
        log_error("can't extend size, len: %d, max_len: %d \n",
                context->len, context->max_len);
        return -1;
    }

    if (context->len + increment <= context->max_len) {
        extend_len = HY_MEM_ALIGN4_UP(context->len + increment + 1);
    } else {
        extend_len = context->max_len;
        ret = 1;
        log_error("extend to max len, len: %d, extend_len: %d \n",
                context->len, extend_len);
    }

    ptr = realloc(context->buf, extend_len);
    if (!ptr) {
        log_error("realloc failed \n");
        return -1;
    }

    context->buf = ptr;
    context->len = extend_len;

    return ret;
}

hy_s32_t dynamic_array_write_vprintf(dynamic_array_s *context,
        const char *format, va_list *args)
{
    assert(context);
    assert(format);
    assert(args);
    #define _get_dynamic_info()                                 \
        do {                                                    \
            va_copy(ap, *args);                                 \
            free_len = context->len - context->cur_len - 1;     \
            ptr = context->buf + context->write_pos;            \
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
            context->cur_len += ret;
            context->write_pos += ret;
        } else {
            ret = _dynamic_array_extend(context,
                    ret - (context->len - context->cur_len));
            if (-1 == ret) {
                log_info("_dynamic_array_extend failed \n");
            } else if (ret >= 0) {
                _get_dynamic_info();
                ret = vsnprintf(ptr, free_len, format, ap);
                context->cur_len += ret;
                context->write_pos += ret;

                /* @fixme: <22-04-23, uos> 做进一步判断 */
            }
        }
    }

    return ret;
}

hy_s32_t dynamic_array_write(dynamic_array_s *context,
        const void *buf, hy_u32_t len)
{
    #define _write_data_com(_buf, _len)                 \
        do {                                            \
            char *ptr = NULL;                           \
            ptr = context->buf + context->write_pos;    \
            memcpy(ptr, _buf, _len);                    \
            context->cur_len      += _len;              \
            context->write_pos    += _len;              \
        } while (0)

    assert(context);
    assert(buf);
    hy_s32_t ret = 0;

    if (context->len - context->cur_len > len) {
        _write_data_com(buf, len);
    } else {
        ret = _dynamic_array_extend(context,
                len - (context->len - context->cur_len));
        if (-1 == ret) {
            log_info("_dynamic_array_extend failed \n");
            len = -1;
        } else if (0 == ret) {
            _write_data_com(buf, len);
        } else {
            // 3 for "..." 1 for "\0"
            len = context->len - context->cur_len - 3 - 1;
            _write_data_com(buf, len);

            log_info("truncated data \n");
            _write_data_com("...", 3);
            len += 3;
        }
    }

    return len;
}

void dynamic_array_destroy(dynamic_array_s **context_pp)
{
    dynamic_array_s *context = *context_pp;

    if (!context_pp || !*context_pp) {
        log_error("the param is error \n");
        return;
    }
    log_info("dynamic array context: %p destroy, buf: %p \n",
            context, context->buf);

    free(context->buf);
    free(context);
    *context_pp = NULL;
}

dynamic_array_s *dynamic_array_create(hy_u32_t min_len, hy_u32_t max_len)
{
    if (min_len == 0 || max_len == 0 || min_len > max_len) {
        log_error("the param is error \n");
        return NULL;
    }

    dynamic_array_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->buf =calloc(1, min_len);
        if (!context->buf) {
            log_error("calloc failed \n");
            break;
        }

        context->max_len    = max_len;
        context->min_len    = min_len;
        context->len        = context->min_len;
        context->write_pos  = context->read_pos = 0;

        log_info("dynamic array context: %p create, buf: %p \n",
                context, context->buf);
        return context;
    } while (0);

    log_error("dynamic array context: %p create failed \n", context);
    dynamic_array_destroy(&context);
    return NULL;
}

