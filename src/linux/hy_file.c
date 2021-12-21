/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_file.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    04/11 2021 08:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        04/11 2021      create the file
 * 
 *     last modified: 04/11 2021 08:08
 */
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#include "hy_file.h"

#include "hy_log.h"

ssize_t HyFileWrite(int fd, const void *buf, size_t len)
{
    ssize_t ret;
    size_t nleft;
    const void *ptr;

    ptr   = buf;
    nleft = len;

    while (nleft > 0) {
        ret = write(fd, ptr, nleft);
        if (ret <= 0) {
            if (ret < 0 && errno == EINTR) {
                ret = 0;
            } else {
                return -1;
            }
        }

        nleft -= ret;
        ptr   += ret;
    }

    return len;
}

ssize_t HyFileRead(int fd, void *buf, size_t len)
{
    ssize_t ret;
    size_t nleft;
    size_t offset = 0;

    nleft = len;

    while (nleft > 0) {
        ret = read(fd, buf + offset, nleft);
        // printf("file_wrapper->read, len: %d \n", ret);
        if (ret < 0) {
            if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
                ret = 0;
            } else {
                return -1;
            }
        } else if (ret == 0) {
            LOGE("fd: %d, server/client closes \n", fd);
            break;
        }

        nleft  -= ret;
        offset += ret;
    }

    return offset;
}
