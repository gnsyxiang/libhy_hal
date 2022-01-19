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
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "hy_log.h"
#include "hy_type.h"

#include "hy_file.h"

ssize_t HyFileRead(int fd, void *buf, size_t len)
{
    ssize_t ret = 0;

    ret = read(fd, buf, len);
    if (ret < 0) {
        if (EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno) {
            ret = 0;
        } else {
            ret = -1;
            LOGES("read failed \n");
        }
    } else if (ret == 0) {
        LOGE("fd: %d, server/client closes \n", fd);
        ret = -1;
    }

    return ret;
}

ssize_t HyFileReadN(int fd, void *buf, size_t len)
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

ssize_t HyFileReadNTimeout(int fd, void *buf, size_t cnt, size_t ms)
{
    hy_s32_t len = 0;
    fd_set rfds;
    struct timeval time;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

	time.tv_sec = ms / 1000;
	time.tv_usec = (ms % 1000) * 1000;

    size_t ret = select(fd+1, &rfds, NULL, NULL, &time);
    switch (ret) {
        case -1:
            LOGE("select error");
            cnt = -1;
            break;
        case 0:
            LOGE("select timeout");
            cnt = -1;
            break;
        default:
            if ((len = HyFileReadN(fd, buf, cnt)) == -1) {
                LOGE("read error");
                cnt = -1;
            }
            break;
    }

    return cnt;
}

ssize_t HyFileWriteN(int fd, const void *buf, size_t len)
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
                LOGES("write failed \n");
                return -1;
            }
        }

        nleft -= ret;
        ptr   += ret;
    }

    return len;
}
