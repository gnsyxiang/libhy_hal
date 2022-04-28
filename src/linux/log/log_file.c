/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    log_file.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 13:55
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 13:55
 */
#include <stdio.h>
#include <unistd.h>

#include "log_private.h"

#include "log_file.h"

hy_s32_t log_file_write(hy_s32_t *fd, const void *buf, hy_u32_t len)
{
    hy_s32_t ret;
    hy_u32_t nleft;
    const void *ptr;

    if (*fd < 0) {
        return -1;
    }

    ptr   = buf;
    nleft = len;

    while (nleft > 0) {
        ret = write(*fd, ptr, nleft);
        if (ret <= 0) {
            if (ret < 0 && errno == EINTR) {
                ret = 0;
            } else {
                log_error("fd close, fd: %d \n", *fd);
                close(*fd);
                *fd = -1;
                return -1;
            }
        }

        nleft -= ret;
        ptr   += ret;
    }

    return len;
}

hy_s32_t log_file_read(hy_s32_t *fd, void *buf, hy_u32_t len)
{
    hy_s32_t ret = 0;

    if (*fd < 0) {
        return -1;
    }

    ret = read(*fd, buf, len);
    if (ret < 0) {
        if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
            ret = 0;
        } else {
            log_error("read failed, fd: %d \n", *fd);
            *fd = -1;
            ret = -1;
        }
    } else if (ret == 0) {
        log_error("fd close, fd: %d \n", *fd);
        *fd = -1;
        ret = -1;
    }

    return ret;
}

