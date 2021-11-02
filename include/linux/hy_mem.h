/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_mem.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2021 19:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2021      create the file
 * 
 *     last modified: 25/10 2021 19:11
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_MEM_H_
#define __LIBHY_HAL_INCLUDE_HY_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define HY_MEM_ALIGN(len, align)    (((len) + (align) - 1) & ~((align) - 1))    ///< 字节对齐
#define HY_MEM_ALIGN2(len)          HY_MEM_ALIGN(len, 2)                        ///< 2字节对齐
#define HY_MEM_ALIGN4(len)          HY_MEM_ALIGN(len, 4)                        ///< 4字节对齐
#define HY_MEM_ALIGN4_UP(len)       (HY_MEM_ALIGN(len, 4) + HY_MEM_ALIGN4(1))   ///< 4字节向上对齐(原来已经事4字节对齐，使用后再增加4个字节)

#define HY_MEM_MALLOC_BREAK(type, size)         \
    ({                                          \
        void *ptr = malloc((size));             \
        if (!ptr) {                             \
            LOGE("malloc faild \n");            \
            break;                              \
        } else {                                \
            memset(ptr, '\0', (size));          \
        }                                       \
        (type)ptr;                              \
     })

#define HY_MEM_MALLOC_RET(type, size)           \
    ({                                          \
        void *ptr = malloc((size));             \
        if (!ptr) {                             \
            LOGE("malloc faild \n");            \
            return;                             \
        } else {                                \
            memset(ptr, '\0', (size));          \
        }                                       \
        (type)ptr;                              \
     })

#define HY_MEM_MALLOC_RET_VAL(type, size, ret)  \
    ({                                          \
        void *ptr = malloc((size));             \
        if (!ptr) {                             \
            LOGE("malloc faild \n");            \
            return ret;                         \
        } else {                                \
            memset(ptr, '\0', (size));          \
        }                                       \
        (type)ptr;                              \
     })

#define HY_MEM_FREE_P(ptr)                      \
    do {                                        \
        if (ptr) {                              \
            free(ptr);                          \
            ptr = NULL;                         \
        }                                       \
    } while (0)

#define HY_MEM_FREE_PP(pptr)                    \
    do {                                        \
        if (pptr && *pptr) {                    \
            free(*pptr);                        \
            *pptr = NULL;                       \
        }                                       \
    } while (0)

/**
 * @brief 申请内存
 *
 * @param size 需要分配的长度
 *
 * @return 成功返回内存的地址，失败返回NULL
 */
void *HyMemMalloc(size_t size);

/**
 * @brief 释放内存
 *
 * @param pptr 需要释放内存的二级指针
 */
void HyMemFree(void **pptr);

/**
 * @brief 申请内存
 *
 * @param nmemb 申请内存的个数
 * @param size 每个内存对应的长度
 *
 * @return 成功返回内存的地址，失败返回NULL
 */
void *HyMemCalloc(size_t nmemb, size_t size);

/**
 * @brief 申请内存
 *
 * @param ptr 原来内存的地址
 * @param nmemb 申请内存的个数
 * @param size 每个内存对应的长度
 *
 * @return 成功返回内存的地址，失败返回NULL
 */
void *HyMemRealloc(void *ptr, size_t nmemb, size_t size);

#ifdef __cplusplus
}
#endif

#endif
