/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_storage.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/01 2022 09:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/01 2022      create the file
 * 
 *     last modified: 06/01 2022 09:32
 */
#include <stdio.h>
#include <sys/vfs.h>    /* or <sys/statfs.h> */
#include <unistd.h>

#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_type.h"
#include "hy_log.h"

#include "hy_storage.h"

hy_s32_t HyStorageSdGetFree(const char *mount_path, hy_u32_t *free_size)
{
    LOGT("mount_path: %s, free_size: %p \n", mount_path, free_size);
    HY_ASSERT_RET_VAL(!mount_path || !free_size
            || HY_STRLEN(mount_path) <= 0, -1);

    struct statfs stat;
    hy_u64_t free_size_tmp;

    if (0 != access(mount_path, F_OK)){
        LOGES("access failed, mount_path: %s \n", mount_path);
        return -1;
    }

    HY_MEMSET(&stat, sizeof(stat));
    if (0 != statfs(mount_path, &stat)) {
        LOGES("statfs failed \n");
        return -1;
    }

    free_size_tmp = stat.f_bsize * stat.f_bavail;
    *free_size = (hy_u32_t)(free_size_tmp >> 20);

    return 0;
}

hy_s32_t HyStorageSdGetFreeRatio(const char *mount_path, float *free_ratio)
{
    LOGT("mount_path: %s, free_ratio: %p \n", mount_path, free_ratio);
    HY_ASSERT_RET_VAL(!mount_path || !free_ratio
            || HY_STRLEN(mount_path) <= 0, -1);

    struct statfs stat;
    long all_blocks;

    if (0 != access(mount_path, F_OK)){
        LOGES("access failed, mount_path: %s \n", mount_path);
        return -1;
    }

    HY_MEMSET(&stat, sizeof(stat));
    if (0 != statfs(mount_path, &stat)) {
        LOGES("statfs failed \n");
        return -1;
    }

    all_blocks = stat.f_blocks - stat.f_bfree + stat.f_bavail;
    *free_ratio = (float)stat.f_bavail / all_blocks;

    return 0;
}

hy_s32_t HyStorageSdGetInfo(const char *mount_path,
        hy_u32_t *total_size, hy_u32_t *free_size, float *free_ratio)
{
    LOGT("mount_path: %s, total_size: %p, free_size: %p, free_ratio: %p \n",
            mount_path, total_size, free_size, free_ratio);
    HY_ASSERT_RET_VAL(!mount_path || !total_size || !free_size
            || !free_ratio || HY_STRLEN(mount_path) <= 0, -1);

    struct statfs stat;
    hy_u64_t free_size_tmp;
    hy_u32_t all_blocks;

    if (0 != access(mount_path, F_OK)){
        LOGES("access failed, mount_path: %s \n", mount_path);
        return -1;
    }

    HY_MEMSET(&stat, sizeof(stat));
    if (0 != statfs(mount_path, &stat)) {
        LOGES("statfs failed \n");
        return -1;
    }

    free_size_tmp = stat.f_bsize * stat.f_bavail;
    all_blocks = stat.f_blocks - stat.f_bfree + stat.f_bavail;

    *free_ratio = (float)stat.f_bavail / all_blocks;
    *free_size = (hy_u32_t)(free_size_tmp >> 20);
    *total_size = *free_size / *free_ratio;

    return 0;
}

