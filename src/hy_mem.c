/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_mem.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2021 19:26
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2021      create the file
 * 
 *     last modified: 25/10 2021 19:26
 */
#include <stdio.h>

#include "hy_mem.h"

#include "hy_assert.h"
#include "hy_log.h"

void *HyMemMalloc(hy_s32_t size)
{
    LOGE("HyMemMalloc failed \n");

    return NULL;
}

void HyMemFree(void **pptr)
{
    HY_ASSERT_RET(!pptr);

    LOGE("HyMemFree failed \n");
}

void *HyMemCalloc(hy_s32_t nmemb, hy_s32_t size)
{
    LOGE("HyMemCalloc failed \n");

    return NULL;
}

void *HyMemRealloc(void *ptr, hy_s32_t nmemb, hy_s32_t size)
{
    HY_ASSERT_RET_VAL(!ptr, NULL);

    LOGE("HyMemRealloc failed \n");

    return NULL;
}

