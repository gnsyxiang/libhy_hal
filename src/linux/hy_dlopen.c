/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_dlopen.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/10 2021 09:15
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/10 2021      create the file
 * 
 *     last modified: 26/10 2021 09:15
 */
#include <stdio.h>
#include <dlfcn.h>

#include "hy_dlopen.h"

#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

void *HyDlLibLoadSymbol(void *handle, const char *symbol)
{
    HY_ASSERT_VAL_RET_VAL(!handle || !symbol, NULL);

    void *symbol_handle = dlsym(handle, symbol);
    if (!symbol_handle) {
        LOGE("dlsym failed, %s \n", dlerror());
    }

    return symbol_handle;
}

void HyDlLibClose(void **handle)
{
    HY_ASSERT_VAL_RET(!handle || !*handle);

    dlclose(*handle);

    *handle = NULL;
}

void *HyDlLibOpen(const char *so_name)
{
    HY_ASSERT_VAL_RET_VAL(!so_name, NULL);

    void *handle = dlopen(so_name, RTLD_LAZY);
    if (!handle) {
        LOGE("dlopen failed, %s \n", dlerror());
    }

    return handle;
}
