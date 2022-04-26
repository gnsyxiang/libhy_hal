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
#include <stdlib.h>

#include "hy_barrier.h"
#include "log_private.h"

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

static void _dump_content(fifo_context_s *context)
{
    hy_u32_t len_tmp;

    len_tmp = context->len - FIFO_READ_POS(context);
    len_tmp = HY_UTILS_MIN(len_tmp, FIFO_USED_LEN(context));

    log_info("used len: %u, write_pos: %u, read_pos: %u \n",
            FIFO_USED_LEN(context), context->write_pos, context->read_pos);

    _hex(context->buf + FIFO_READ_POS(context), len_tmp, 1);
    _hex(context->buf, FIFO_USED_LEN(context) - len_tmp, 1);
}

void fifo_dump(fifo_context_s *context, fifo_dump_type_e type)
{
    if (!context) {
        log_error("the param is error \n");
        return;
    }

    switch (type) {
        case FIFO_DUMP_TYPE_ALL:
            printf("[%s:%d] len: %d \n", __func__, __LINE__, context->len);
            _hex(context->buf, context->len, 1);
            break;
        case FIFO_DUMP_TYPE_CONTENT:
            _dump_content(context);
            break;
        default:
            log_error("error type: %d \n", type);
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
    if (!context || !buf || len == 0) {
        log_error("the param is error \n");
        return -1;
    }

    len = _fifo_read_com(context, buf, len);
    context->read_pos += len;

    return len;
}

hy_s32_t fifo_read_peek(fifo_context_s *context, void *buf, hy_u32_t len)
{
    if (!context || !buf || len == 0) {
        log_error("the param is error \n");
        return -1;
    }

    return _fifo_read_com(context, buf, len);
}

hy_s32_t fifo_write(fifo_context_s *context, const void *buf, hy_u32_t len)
{
    hy_u32_t len_tmp = 0;

    if (!context || !buf || len == 0) {
        log_info("the param is error \n");
        return -1;
    }

    if (len > FIFO_FREE_LEN(context)) {
        log_error("write failed, len: %u, free_len: %u \n",
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
    fifo_context_s *context = *context_pp;

    if (!context_pp || !*context_pp) {
        log_error("the param is error \n");
        return ;
    }

    free(context->buf);

    log_info("fifo destroy, context: %p \n", context);
    free(context);
    *context_pp = NULL;
}

fifo_context_s *fifo_create(hy_u32_t len)
{
    fifo_context_s *context = NULL;

    if (len == 0) {
        log_error("the param is error \n");
        return NULL;
    }

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

        log_info("fifo create, context: %p \n", context);
        return context;
    } while (0);

    log_error("fifo create failed \n");
    return NULL;
}

