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

#include "hy_assert.h"
#include "hy_log.h"

#include "hy_file.h"

#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);

成功返回文件描述符，失败返回-1

O_RDONLY　　只读打开
O_WRONLY　  只写打开
O_RDWR      读写打开

O_APPEND    将写入追加到文件的尾端
O_CREAT     若文件不存在，则创建它。使用该选项时，需要第三个参数mode，用来指定新文件的访问权限位
O_EXCL      如果同时指定了O_CREAT，而文件已经存在，则会出错
O_TRUNC     如果此文件存在，而且为只写或读写模式成功打开，则将其长度截短为0
O_NOCTTY    如果pathname指的是终端设备，则不将该设备分配作为此进程的控制终端
O_NONBLOCK  如果pathname指的是一个FIFO文件、块设备文件或字符设备文件，则此选项将文件的本次打开操作和后续的I/O操作设置为非阻塞模式
#endif

hy_s32_t HyFileIsExist(const char *file_path)
{
    HY_ASSERT_RET_VAL(!file_path, -1);

    return access(file_path, F_OK);
}

void HyFileRemove(const char *file_path)
{
    HY_ASSERT_RET(!file_path);

    remove(file_path);
}

hy_s64_t HyFileGetLen(const char *file)
{
    FILE *fp = NULL;
    hy_s64_t len = 0;

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

hy_s32_t HyFileRead(hy_s32_t fd, void *buf, hy_u32_t len)
{
    hy_s32_t ret = 0;

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

hy_s32_t HyFileReadN(hy_s32_t fd, void *buf, hy_u32_t len)
{
    hy_s32_t ret;
    hy_u32_t nleft;
    hy_u32_t offset = 0;

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

hy_s32_t HyFileReadNTimeout(hy_s32_t fd, void *buf, hy_u32_t cnt, hy_u32_t ms)
{
    hy_s32_t len = 0;
    fd_set rfds;
    struct timeval time;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

	time.tv_sec = ms / 1000;
	time.tv_usec = (ms % 1000) * 1000;

    hy_u32_t ret = select(fd+1, &rfds, NULL, NULL, &time);
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

hy_s32_t HyFileWrite(hy_s32_t fd, const void *buf, hy_u32_t len)
{
    hy_s32_t ret;

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

hy_s32_t HyFileWriteN(hy_s32_t fd, const void *buf, hy_u32_t len)
{
    hy_s32_t ret;
    hy_u32_t nleft;
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

