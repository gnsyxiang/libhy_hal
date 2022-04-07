/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_sd.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/04 2022 18:52
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/04 2022      create the file
 * 
 *     last modified: 07/04 2022 18:52
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_log.h"
#include "hy_hal_utils.h"

#include "hy_sd.h"

#define _SD_PARTITIIONS_PATH     "/proc/partitions"

static hy_s32_t _get_partitions_content(const char *name,
        char *content, hy_u32_t content_len)
{
    hy_s32_t fd = -1;

    fd = open(name, O_RDONLY);
    if (-1 == fd) {
        LOGES("open %s failed \n", name);
        return -1;
    }

    if (read(fd, content, content_len) <= 0) {
        LOGE("the %s length is 0 \n", name);

        close(0);
        return -1;
    }

    close(fd);

    return 0;
}

hy_s32_t HySdGetPartitionsName(char *name, hy_u32_t name_len_max)
{
    LOGT("name: %p, name_len_max: %d \n", name, name_len_max);
    HY_ASSERT_RET_VAL(!name, HY_SD_STATE_UNINSERT);

    HySdState_e ret = HY_SD_STATE_UNINSERT;
    char *delim = "\n";
    char *line = NULL;
    char *strtok_tmp_ptr = NULL;
    char buf[1024] = {0};

    char *mmc_name[][2] = {
        {"/dev/mmcblk1p1",  "mmcblk1p1"},
        {"/dev/mmcblk1",    "mmcblk1"},
        {"/dev/mmcblk0p1",  "mmcblk0p1"},
        {"/dev/mmcblk0",    "mmcblk0"},
    };

    if (0 != _get_partitions_content(_SD_PARTITIIONS_PATH, buf, sizeof(buf))) {
        LOGE("_get_partitions_content failed \n");
        return ret;
    }

    line = strtok_r(buf, delim, &strtok_tmp_ptr);
    while (line) {
        for (hy_u32_t i = 0; i < HyHalUtilsArrayCnt(mmc_name); ++i) {
            hy_s32_t fd = open(mmc_name[i][0], O_RDWR);
            if (fd > 0) {
                close(fd);

                char *seek_sd = strstr(line, mmc_name[i][1]);
                if (seek_sd) {
                    HY_MEMSET(name, name_len_max);
                    snprintf(name, name_len_max, "%s", seek_sd);
                    ret = HY_SD_STATE_INSERT;

                    LOGI("find mmc device [%s]\n", name);
                    goto _ERR;
                }
            }

            line = strtok_r(NULL, delim, &strtok_tmp_ptr);
        }
    }

_ERR:
    return ret;
}

