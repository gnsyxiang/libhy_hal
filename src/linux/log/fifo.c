/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    fifo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    21/04 2022 15:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        21/04 2022      create the file
 * 
 *     last modified: 21/04 2022 15:43
 */
#include <stdio.h>

#include "hy_assert.h"
#include "hy_barrier.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"
#include "hy_thread_mutex.h"
#include "hy_hex.h"

#include "fifo.h"

#define _IS_POWER_OF_2(x)           ((x) != 0 && (((x) & ((x) - 1)) == 0))  ///< 判断x是否为2^n，是返回1，否返回0
#define HY_UTILS_MIN(x, y)          ((x) < (y) ? (x) : (y))                 ///< 求最小值
#define HY_UTILS_MAX(x, y)          ((x) > (y) ? (x) : (y))                 ///< 求最大值

/**
 * @brief 内存屏障
 *
 * 解决两个问题:
 * 1, 解决内存可见性问题
 * 2, 解决cpu重排的问题，cpu优化
 */
#define USE_MB

static void _dump_content(fifo_context_s *context)
{
    hy_u32_t len_tmp;

    len_tmp = context->len - FIFO_READ_POS(context);
    len_tmp = HY_UTILS_MIN(len_tmp, FIFO_USED_LEN(context));

    LOGD("used len: %u, write_pos: %u, read_pos: %u \n",
            FIFO_USED_LEN(context), context->write_pos, context->read_pos);

    HyHex(context->buf + FIFO_READ_POS(context), len_tmp, 1);
    HyHex(context->buf, FIFO_USED_LEN(context) - len_tmp, 1);
}

void fifo_dump(fifo_context_s *context, fifo_dump_type_e type)
{
    HY_ASSERT_RET(!context);

    switch (type) {
        case FIFO_DUMP_TYPE_ALL:
            HY_HEX_ASCII(context->buf, context->len);
            break;
        case FIFO_DUMP_TYPE_CONTENT:
            _dump_content(context);
            break;
        default:
            LOGE("error type: %d \n", type);
    }

}

static hy_s32_t _fifo_read_com(fifo_context_s *context, void *buf, hy_u32_t len)
{
    hy_u32_t len_tmp = 0;

    if (FIFO_IS_EMPTY(context)) {
        return 0;
    }

    len = HY_UTILS_MIN(len, FIFO_USED_LEN(context));

#ifdef USE_MB
    // 确保其他线程对read_pos的可见性
    HY_SMP_WMB();
#endif

    len_tmp = HY_UTILS_MIN(len, context->len - FIFO_READ_POS(context));

    memcpy(buf, context->buf + FIFO_READ_POS(context), len_tmp);
    memcpy(buf + len_tmp, context->buf, len - len_tmp);

#ifdef USE_MB
    // 确保read_pos不会优化到上面去
    HY_SMP_MB();
#endif

    return len;
}

hy_s32_t fifo_read(fifo_context_s *context, void *buf, hy_u32_t len)
{
    HY_ASSERT_RET_VAL(!context || !buf || len == 0, -1);

    len = _fifo_read_com(context, buf, len);
    context->read_pos += len;

    return len;
}

hy_s32_t fifo_read_peek(fifo_context_s *context, void *buf, hy_u32_t len)
{
    HY_ASSERT_RET_VAL(!context || !buf || len == 0, -1);

    return _fifo_read_com(context, buf, len);
}

hy_s32_t fifo_write(fifo_context_s *context, const void *buf, hy_u32_t len)
{
    HY_ASSERT_RET_VAL(!context || !buf || len == 0, -1);
    hy_u32_t len_tmp = 0;

    if (len > FIFO_FREE_LEN(context)) {
        LOGE("write failed, len: %u, free_len: %u \n",
                len, FIFO_FREE_LEN(context));
        return -1;
    }

#ifdef USE_MB
    // 确保其他线程对write_pos的可见性
    HY_SMP_MB();
#endif

    len_tmp = HY_UTILS_MIN(len, context->len - FIFO_WRITE_POS(context));

    memcpy(context->buf + FIFO_WRITE_POS(context), buf, len_tmp);
    memcpy(context->buf, buf + len_tmp, len - len_tmp);

#ifdef USE_MB
    // 确保write_pos不会优化到上面去
    HY_SMP_WMB();
#endif

    context->write_pos += len;

    return len;
}

static hy_u32_t _num_to_2n(hy_u32_t num)
{
    hy_u32_t i = 1;
    hy_u32_t num_tmp = num;

    while (num >>= 1) {
        i <<= 1;
    }

    return (i < num_tmp) ? i << 1U : i;
}

void fifo_destroy(fifo_context_s **context_pp)
{
    HY_ASSERT_RET(!context_pp || !*context_pp);
    fifo_context_s *context = *context_pp;

    HY_MEM_FREE_PP(&context->buf);

    LOGI("fifo destroy, context: %p \n", context);
    HY_MEM_FREE_PP(context_pp);
}

fifo_context_s *fifo_create(hy_u32_t len)
{
    HY_ASSERT_RET_VAL(len == 0, NULL);
    fifo_context_s *context = NULL;

    do {
        if (!_IS_POWER_OF_2(len)) {
            LOGW("old len: %d \n", len);
            len = _num_to_2n(len);
            LOGW("len must be power of 2, new len: %d \n", len);
        }

        context = HY_MEM_MALLOC_BREAK(fifo_context_s *, sizeof(*context));
        context->buf = HY_MEM_MALLOC_BREAK(char *, len);

        context->len        = len;
        context->read_pos   = context->write_pos = 0;

        LOGI("fifo create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("fifo create failed \n");
    return NULL;
}

