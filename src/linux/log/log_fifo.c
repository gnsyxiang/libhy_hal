/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    log_fifo.c
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

#include "hy_barrier.h"
#include "log_private.h"

#include "log_fifo.h"

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

static void _hex(const void *_buf, hy_u32_t len, hy_s32_t flag)
{
    hy_u8_t cnt = 0;
    const unsigned char *buf = (const unsigned char *)_buf;

    if (len <= 0) {
        return;
    }

    for (hy_u32_t i = 0; i < len; i++) {
        if (flag == 1) {
            if (buf[i] == 0x0d || buf[i] == 0x0a || buf[i] < 32 || buf[i] >= 127) {
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

static void _dump_content(log_fifo_context_s *context)
{
    assert(context);
    hy_u32_t len_tmp;

    len_tmp = context->len - LOG_FIFO_READ_POS(context);
    len_tmp = HY_UTILS_MIN(len_tmp, LOG_FIFO_USED_LEN(context));

    log_info("used len: %u, write_pos: %u, read_pos: %u \n",
            LOG_FIFO_USED_LEN(context), context->write_pos, context->read_pos);

    _hex(context->buf + LOG_FIFO_READ_POS(context), len_tmp, 1);
    _hex(context->buf, LOG_FIFO_USED_LEN(context) - len_tmp, 1);
}

void fifo_dump(log_fifo_context_s *context, log_fifo_dump_type_e type)
{
    assert(context);

    switch (type) {
        case LOG_FIFO_DUMP_TYPE_ALL:
            printf("[%s:%d] len: %d \n", __func__, __LINE__, context->len);
            _hex(context->buf, context->len, 1);
            break;
        case LOG_FIFO_DUMP_TYPE_CONTENT:
            _dump_content(context);
            break;
        default:
            log_error("error type: %d \n", type);
    }

}

static hy_s32_t _fifo_read_com(log_fifo_context_s *context, void *buf, hy_u32_t len)
{
    hy_u32_t len_tmp = 0;

    if (LOG_FIFO_IS_EMPTY(context)) {
        return 0;
    }

    len = HY_UTILS_MIN(len, LOG_FIFO_USED_LEN(context));

#ifdef USE_MB
    // 确保其他线程对read_pos的可见性
    HY_SMP_WMB();
#endif

    len_tmp = HY_UTILS_MIN(len, context->len - LOG_FIFO_READ_POS(context));

    memcpy(buf, context->buf + LOG_FIFO_READ_POS(context), len_tmp);
    memcpy(buf + len_tmp, context->buf, len - len_tmp);

#ifdef USE_MB
    // 确保read_pos不会优化到上面去
    HY_SMP_MB();
#endif

    return len;
}

hy_s32_t log_fifo_read(log_fifo_context_s *context, void *buf, hy_u32_t len)
{
    assert(context);
    assert(buf);

    len = _fifo_read_com(context, buf, len);
    context->read_pos += len;

    return len;
}

hy_s32_t log_fifo_read_peek(log_fifo_context_s *context, void *buf, hy_u32_t len)
{
    assert(context);
    assert(buf);

    return _fifo_read_com(context, buf, len);
}

hy_s32_t log_fifo_write(log_fifo_context_s *context, const void *buf, hy_u32_t len)
{
    assert(context);
    assert(buf);

    hy_u32_t len_tmp = 0;

    if (len > LOG_FIFO_FREE_LEN(context)) {
        log_error("write failed, len: %u, free_len: %u \n",
                len, LOG_FIFO_FREE_LEN(context));
        return -1;
    }

#ifdef USE_MB
    // 确保其他线程对write_pos的可见性
    HY_SMP_MB();
#endif

    len_tmp = HY_UTILS_MIN(len, context->len - LOG_FIFO_WRITE_POS(context));

    memcpy(context->buf + LOG_FIFO_WRITE_POS(context), buf, len_tmp);
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

void log_fifo_destroy(log_fifo_context_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is error \n");
        return ;
    }
    log_fifo_context_s *context = *context_pp;
    log_info("fifo create context: %p destroy, buf: %p \n",
            context, context->buf);

    free(context->buf);

    free(context);
    *context_pp = NULL;
}

log_fifo_context_s *log_fifo_create(hy_u32_t len)
{
    if (len == 0) {
        log_error("the param is error \n");
        return NULL;
    }

    log_fifo_context_s *context = NULL;
    do {
        if (!_IS_POWER_OF_2(len)) {
            log_error("old len: %d \n", len);
            len = _num_to_2n(len);
            log_error("len must be power of 2, new len: %d \n", len);
        }

        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->buf = calloc(1, len);
        if (!context->buf) {
            log_error("calloc failed \n");
            break;
        }

        context->len        = len;
        context->read_pos   = context->write_pos = 0;

        log_info("fifo create context: %p create, buf: %p \n",
                context, context->buf);
        return context;
    } while (0);

    log_error("fifo create context: %p failed \n", context);
    return NULL;
}

