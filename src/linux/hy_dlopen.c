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

void *HyDlLibLoadSymbol(void *handle, const char *symbol)
{
    HY_ASSERT_RET_VAL(!handle || !symbol, NULL);

    void *symbol_handle = dlsym(handle, symbol);
    if (!symbol_handle) {
        LOGE("dlsym %s failed, %s \n", symbol, dlerror());
    }

    return symbol_handle;
}

void HyDlLibClose(void **handle)
{
    HY_ASSERT_RET(!handle || !*handle);

    dlclose(*handle);

    *handle = NULL;
}

void *HyDlLibOpen(const char *so_name)
{
    HY_ASSERT_RET_VAL(!so_name, NULL);

    void *handle = dlopen(so_name, RTLD_LAZY);
    if (!handle) {
        LOGE("dlopen %s failed, %s \n", so_name, dlerror());
    }

    return handle;
}

/* 
1, 获取LD_DEBUG帮助信息

$ LD_DEBUG=help ls
Valid options for the LD_DEBUG environment variable are:

  libs        display library search paths
  reloc       display relocation processing
  files       display progress for input file
  symbols     display symbol table processing
  bindings    display information about symbol binding
  versions    display version dependencies
  scopes      display scope information
  all         all previous options combined
  statistics  display relocation statistics
  unused      determined unused DSOs
  help        display this help message and exit

To direct the debugging output into a file instead of standard output
a filename can be specified using the LD_DEBUG_OUTPUT environment variable.

2，显示库搜索相关信息

    LD_DEBUG=libs ./hello-world

*/
