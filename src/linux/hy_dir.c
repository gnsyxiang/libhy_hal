/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_dir.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 09:00
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 09:00
 */
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "hy_dir.h"

#include "hy_type.h"
#include "hy_string.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

static void _filter_file(const char *path, const char *name,
        uint8_t type, void *args,
        HyDirReadCb_t read_cb, const char *filter)
{
    char buf[HY_STRING_BUF_MAX_LEN_64] = {0};
    size_t len;

    if (!read_cb) {
        return;
    }

    if (filter) {
        buf[0] = '.';
        len = strlen(filter);
        HyStrCopyRight(name, buf + 1, HY_STRING_BUF_MAX_LEN_64, '.');
        if (len > HY_STRING_BUF_MAX_LEN_64) {
            LOGE("the suffix is too long \n");
            return;
        } else {
            if (0 == strncmp(buf, filter, strlen(filter))) {
                read_cb(path, name, type, args);
            }
        }
    } else {
        read_cb(path, name, type, args);
    }
}

static void _handle_sub_dir(const char *path, char *name, const char *filter,
        HyDirReadCb_t handle_cb, void *args)
{
    size_t len = strlen(path) + strlen(name) + 1 + 1; // 1 for space('\0'), 1 for '/'
    char *sub_path = malloc(len);
    if (!sub_path) {
        LOGE("malloc failed \n");
        return ;
    }
    memset(sub_path, '\0', len);

    snprintf(sub_path, len, "%s/%s", path, name);
    HyDirReadRecurse(sub_path, filter, handle_cb, args);

    free(sub_path);
}

static int32_t _dir_read(int32_t type, const char *path, const char *filter,
        HyDirReadCb_t read_cb, void *args)
{
    DIR *dir;
    struct dirent *ptr;

    dir = opendir(path);
    if (!dir) {
        LOGE("opendir failed \n");
        return -1;
    }

    while ((ptr = readdir(dir)) != NULL) {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
            continue;
        }

        switch (ptr->d_type) {
            case DT_REG:
                _filter_file(path, ptr->d_name, HY_DIR_TYPE_FILE, args,
                        read_cb, filter);
                break;

            case DT_DIR:
                if (type) {
                    _handle_sub_dir(path, ptr->d_name, filter, read_cb, args);
                } else {
                    _filter_file(path, ptr->d_name, HY_DIR_TYPE_FILE, args,
                            read_cb, filter);
                }
                break;

            default:
                break;
        }
    }

    closedir(dir);

    return 0;
}

int32_t HyDirRead(const char *path, const char *filter,
        HyDirReadCb_t read_cb, void *args)
{
    return _dir_read(0, path, filter, read_cb, args);
}

int32_t HyDirReadRecurse(const char *path, const char *filter,
        HyDirReadCb_t read_cb, void *args)
{
    return _dir_read(1, path, filter, read_cb, args);
}

