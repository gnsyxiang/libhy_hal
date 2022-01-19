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

#define HY_MEM_BYTE(x)                          (*((unsigned char *)(x)))                                   ///< 得到指定地址上的一个字节
#define HY_MEM_WORD(x)                          (*((unsigned short *)(x)))                                  ///< 得到指定地址上的一个字
#define HY_MEM_DOUBLE_WORD(x)                   (*((unsigned int *)(x)))                                    ///< 得到指定地址上的一个双字

#define HY_MEM_OFFSETOF(type, member)           ((size_t) &((type *)0)->member)                             ///< 得到一个member在结构体(struct)中的偏移量
#define HY_MEM_OFFSETOF_SIZE(type, member)      sizeof(((type *)0)->member)                                 ///< 得到一个结构体中member所占用的字节数

#define HY_MEM_LSB_BYTE_2_WORD(array)           ((((unsigned short) (array)[0]) << 8) + (array)[1])         ///< 按照LSB格式把两个字节转化为一个Word
#define HY_MEM_LSB_WORD_2_BYTE(array, word)     (array)[0] = ((word) / 256); (array)[1] = ((word) & 0xFF)   ///< 按照LSB格式把一个Word转化为两个字节

#define HY_MEM_ALIGN(len, align)                (((len) + (align) - 1) & ~((align) - 1))                    ///< 字节对齐
#define HY_MEM_ALIGN2(len)                      HY_MEM_ALIGN(len, 2)                                        ///< 2字节对齐
#define HY_MEM_ALIGN4(len)                      HY_MEM_ALIGN(len, 4)                                        ///< 4字节对齐
#define HY_MEM_ALIGN4_UP(len)                   (HY_MEM_ALIGN(len, 4) + HY_MEM_ALIGN4(1))                   ///< 4字节向上对齐(原来已经事4字节对齐，使用后再增加4个字节)

#define HY_MEMCMP(dst, src, size)               memcmp(dst, src, size)
#define HY_MEMSET(ptr, size)                    memset(ptr, '\0', size)
#define HY_MEMCPY(dst, src, size)               memcpy(dst, src, size)

#define HY_MEM_MALLOC_BREAK(type, size)         \
    ({                                          \
        void *ptr = malloc((size));             \
        if (!ptr) {                             \
            LOGES("malloc failed \n");          \
            break;                              \
        } else {                                \
            HY_MEMSET(ptr, (size));             \
        }                                       \
        (type)ptr;                              \
     })

#define HY_MEM_MALLOC_RET(type, size)           \
    ({                                          \
        void *ptr = malloc((size));             \
        if (!ptr) {                             \
            LOGES("malloc failed \n");          \
            return;                             \
        } else {                                \
            HY_MEMSET(ptr, (size));             \
        }                                       \
        (type)ptr;                              \
     })

#define HY_MEM_MALLOC_RET_VAL(type, size, ret)  \
    ({                                          \
        void *ptr = malloc((size));             \
        if (!ptr) {                             \
            LOGES("malloc failed \n");          \
            return ret;                         \
        } else {                                \
            HY_MEMSET(ptr, (size));             \
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
