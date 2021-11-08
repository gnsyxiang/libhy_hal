/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_mem.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 16:50
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 16:50
 */
#include <stdio.h>

#include "hy_mem.h"

#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

void *HyMemMalloc(size_t size)
{
    LOGE("HyMemMalloc failed \n");

    return NULL;
}

void HyMemFree(void **pptr)
{
    HY_ASSERT_VAL_RET(!pptr);

    LOGE("HyMemFree failed \n");
}

void *HyMemCalloc(size_t nmemb, size_t size)
{
    LOGE("HyMemCalloc failed \n");

    return NULL;
}

void *HyMemRealloc(void *ptr, size_t nmemb, size_t size)
{
    HY_ASSERT_VAL_RET_VAL(!ptr, NULL);

    LOGE("HyMemRealloc failed \n");

    return NULL;
}
