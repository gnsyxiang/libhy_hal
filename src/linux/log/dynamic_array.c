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

typedef struct {
    char *buf;
    hy_u32_t len;
    hy_u32_t cur_len;

    hy_u32_t min_len;
    hy_u32_t max_len;
} _dynamic_array_context_s;

hy_s32_t dynamic_array_read(void *handle, void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    _dynamic_array_context_s *context = handle;

    if (len > context->cur_len) {
        len = context->cur_len;
    }

    HY_MEMCPY(buf, context->buf, len);

    return len;
}

hy_s32_t dynamic_array_write(void *handle, const void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    _dynamic_array_context_s *context = handle;
    hy_u32_t extend_len = 0;
    void *ptr = NULL;

    if (context->len - context->cur_len > len) {
        HY_MEMCPY(context->buf + context->cur_len, buf, len);
        context->cur_len += len;
    } else {
        while (extend_len != context->max_len) {
            extend_len = 2 * context->len;
            if (extend_len > context->max_len) {
                extend_len = context->max_len;
            }

            ptr = realloc(context->buf, extend_len);
            if (!ptr) {
                LOGES("realloc failed \n");
                len = -1;
                break;
            }

            context->buf = ptr;
            context->len = extend_len;

            if (context->len - context->cur_len > len) {
                HY_MEMCPY(context->buf + context->cur_len, buf, len);
                context->cur_len += len;
                break;
            }
        }

        if (extend_len == context->max_len) {
            len = context->len - context->cur_len - 3 - 1; // 3 for "..." 1 for "\0"
            HY_MEMCPY(context->buf + context->cur_len, buf, len);
            context->cur_len += len;
        }
    }

    return len;
}

void dynamic_array_reset(void *handle)
{
    HY_ASSERT(handle);
    _dynamic_array_context_s *context = handle;

    HY_MEMSET(context->buf, context->len);
}

hy_s32_t dynamic_array_get_len(void *handle)
{
    HY_ASSERT(handle);
    _dynamic_array_context_s *context = handle;

    return context->cur_len;
}

void dynamic_array_destroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    _dynamic_array_context_s *context = *handle;

    HY_MEM_FREE_PP(&context->buf);

    LOGI("dynamic array destroy, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *dynamic_array_create(hy_u32_t min_len, hy_u32_t max_len)
{
    LOGT("min_len: %d, max_len: %d \n", min_len, max_len);
    HY_ASSERT_RET_VAL(min_len == 0 || max_len == 0 || min_len < max_len, NULL);

    _dynamic_array_context_s *context = NULL;
    do {
        context = HY_MEM_MALLOC_BREAK(_dynamic_array_context_s *, sizeof(*context));

        context->max_len    = max_len;
        context->min_len    = min_len;
        context->len        = context->min_len;

        context->buf = HY_MEM_MALLOC_BREAK(char *, context->len);

        LOGI("dynamic array create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("dynamic array create failed \n");
    dynamic_array_destroy((void **)&context);
    return NULL;
}

