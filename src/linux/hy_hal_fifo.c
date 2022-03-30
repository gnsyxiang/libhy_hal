/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_hal_fifo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/03 2022 19:57
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/03 2022      create the file
 * 
 *     last modified: 07/03 2022 19:57
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hy_assert.h"
#include "hy_log.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_type.h"

#include "hy_hal_fifo.h"

#define _HAL_FIFO_PATH_LEN_MAX (64)

typedef struct {
    hy_s32_t fd;
} _hal_fifo_context_s;

hy_s32_t HyHalFifoRead(void *fifo_h, void *buf, hy_s32_t len)
{
    LOGT("fifo_h: %p, buf: %p, len: %d \n", fifo_h, buf, len);
    HY_ASSERT(fifo_h);
    HY_ASSERT(buf);

    _hal_fifo_context_s *context = fifo_h;

    return read(context->fd, buf, len);
}

hy_s32_t HyHalFifoWrite(void *fifo_h, const void *buf, hy_s32_t len)
{
    LOGT("fifo_h: %p, buf: %p, len: %d \n", fifo_h, buf, len);
    HY_ASSERT(fifo_h);
    HY_ASSERT(buf);

    _hal_fifo_context_s *context = fifo_h;

    return write(context->fd, buf, len);
}

void HyHalFifoDestroy(void **fifo_h)
{
    LOGT("&fifo_h: %p, fifo_h: %p \n", fifo_h, *fifo_h);
    HY_ASSERT_RET(!fifo_h || !*fifo_h);

    _hal_fifo_context_s *context = *fifo_h;

    close(context->fd);

    LOGI("hy hal fifo destroy, context: %p \n", context);
    HY_MEM_FREE_PP(fifo_h);
}

void *HyHalFifoCreate(HyHalFifoConfig_s *fifo_c)
{
    LOGT("fifo_c: %p \n", fifo_c);
    HY_ASSERT_RET_VAL(!fifo_c, NULL);

    _hal_fifo_context_s *context = NULL;
    char fifo_path[_HAL_FIFO_PATH_LEN_MAX] = {0};
    hy_s32_t flags = 0;

    do {
        context = HY_MEM_MALLOC_BREAK(_hal_fifo_context_s *, sizeof(*context));

        snprintf(fifo_path, _HAL_FIFO_PATH_LEN_MAX, "/tmp/%s", fifo_c->name);
        if (access(fifo_path, F_OK) == -1) {
            if (fifo_c->mode == 0) {
                fifo_c->mode = 0777;
            }

            if (-1 == mkfifo(fifo_path, fifo_c->mode)) {
                LOGES("mkfifo failed \n");
                break;
            }
        }

        switch (fifo_c->flag) {
            case HY_HAL_FIFO_FLAG_READ:
                flags = O_RDONLY;
                break;
            case HY_HAL_FIFO_FLAG_WRITE:
                flags = O_WRONLY;
                break;
            case HY_HAL_FIFO_FLAG_NOBLOCK_READ:
                flags = O_RDONLY | O_NONBLOCK;
                break;
            case HY_HAL_FIFO_FLAG_NOBLOCK_WRITE:
                flags = O_WRONLY | O_NONBLOCK;
                break;
            default:
                LOGE("flag error \n");
                break;
        }

        context->fd = open(fifo_path, flags);
        if (-1 == context->fd) {
            LOGES("open failed \n");
            break;
        }

        LOGI("hy hal fifo create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("hy hal fifo create failed \n");
    HyHalFifoDestroy((void **)&context);
    return NULL;
}

