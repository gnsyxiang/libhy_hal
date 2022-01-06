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
#include <math.h>

#include "hy_string.h"
#include "hy_assert.h"
#include "hy_log.h"

#include "hy_storage.h"

int32_t HyStorageGetFree(const char *mount_path, uint32_t *free_size)
{
    LOGT("mount_path: %s, free_size: %p \n", mount_path, free_size);
    HY_ASSERT_VAL_RET_VAL(!mount_path || !free_size || HY_STRLEN(mount_path) <= 0, -1);

    int ret;
    float temp = 0.0;
    struct statfs stat;

    HY_MEMSET(&stat, sizeof(stat));
    ret = statfs(mount_path, &stat);
    if (0 != ret) {
        LOGES("statfs failed \n");
        return -1;
    }

    unsigned long long block_size = stat.f_bsize;
    unsigned long long free_size_tmp = block_size * stat.f_bavail;

    temp = (float)((free_size_tmp >> 20));
    *free_size = roundf(temp);

    return 0;
}

int32_t HyStorageGetFreeRatio(const char *mount_path, float *ratio)
{
    LOGT("mount_path: %s, ratio: %p \n", mount_path, ratio);
    HY_ASSERT_VAL_RET_VAL(!mount_path || !ratio || HY_STRLEN(mount_path) <= 0, -1);

    struct statfs stat;
    int ret;

    long all_blocks;
    float free_ratio;

    HY_MEMSET(&stat, sizeof(stat));
    ret = statfs(mount_path, &stat);
    if (0 != ret) {
        LOGES("statfs failed \n");
        return -1;
    }

    all_blocks = stat.f_blocks - stat.f_bfree + stat.f_bavail;
    free_ratio = (float)stat.f_bavail / all_blocks;

    *ratio = free_ratio;

    return 0;
}
