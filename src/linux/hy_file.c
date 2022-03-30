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
#include <fcntl.h>

#include "hy_log.h"
#include "hy_type.h"

#include "hy_file.h"

ssize_t HyFileRead(hy_s32_t fd, void *buf, size_t len)
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
        LOGES("fd close, fd: %d \n", fd);
        ret = -1;
    }

    return ret;
}

ssize_t HyFileReadN(hy_s32_t fd, void *buf, size_t len)
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
                LOGES("read failed \n");
                return -1;
            }
        } else if (ret == 0) {
            LOGES("fd close, fd: %d \n", fd);
            break;
        }

        nleft  -= ret;
        offset += ret;
    }

    return offset;
}

ssize_t HyFileReadNTimeout(hy_s32_t fd, void *buf, size_t cnt, size_t ms)
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
            LOGES("select error");
            cnt = -1;
            break;
        case 0:
            LOGES("select timeout");
            cnt = -1;
            break;
        default:
            if ((len = HyFileReadN(fd, buf, cnt)) == -1) {
                LOGES("read error");
                cnt = -1;
            }
            break;
    }

    return cnt;
}

ssize_t HyFileWrite(hy_s32_t fd, const void *buf, size_t len)
{
    ssize_t ret;

    ret = write(fd, buf, len);
    if (ret < 0 && errno == EINTR) {
        return 0;
    } else if (ret == -1) {
        LOGES("fd close, fd: %d \n", fd);
        return -1;
    } else {
        return ret;
    }
}

ssize_t HyFileWriteN(hy_s32_t fd, const void *buf, size_t len)
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
                LOGES("fd close, fd: %d \n", fd);
                return -1;
            }
        }

        nleft -= ret;
        ptr   += ret;
    }

    return len;
}

static hy_s32_t _set_fcntl(hy_s32_t fd, hy_s32_t arg)
{
    hy_s32_t flags;

    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
        flags = 0;
    }

    return fcntl(fd, F_SETFL, flags | arg);
}

static hy_s32_t _reset_fcntl(hy_s32_t fd, hy_s32_t arg)
{
    hy_s32_t flags;

    if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
        flags = 0;
    }

    return fcntl(fd, F_SETFL, flags & arg);
}

hy_s32_t HyFileBlockStateSet(hy_s32_t fd, HyFileBlockState_e state)
{
    if (HY_FILE_BLOCK_STATE_BLOCK == state) {
        return _reset_fcntl(fd, ~O_NONBLOCK);
    } else {
        return _set_fcntl(fd, O_NONBLOCK);
    }
}

HyFileBlockState_e HyFileBlockStateGet(hy_s32_t fd)
{
    hy_s32_t flag;

    flag = fcntl(fd, F_GETFL, 0);
    if (flag < 0) {
        LOGES("fcntl failed \n");
    }

    if (flag & O_NONBLOCK) {
        return HY_FILE_BLOCK_STATE_NOBLOCK;
    } else {
        return HY_FILE_BLOCK_STATE_BLOCK;
    }
}

/*
 * 作用: 当fork子进程后，仍然可以使用fd。
 *       但执行exec后系统就会自动关闭进程中的fd
 */
hy_s32_t file_close_on_exec(hy_s32_t fd)
{
    return _set_fcntl(fd, FD_CLOEXEC);
}

long HyFileGetLen(const char *file)
{
    FILE *fp = NULL;
    long len = 0;

    fp = fopen(file, "r");
    if (!fp) {
        LOGES("fopen %s faild", file);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    if (len == -1) {
        LOGES("ftell failed \n");
    }

    fclose(fp);

    return len;
}

