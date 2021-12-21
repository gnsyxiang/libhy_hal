/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_compile.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/10 2021 08:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/10 2021      create the file
 * 
 *     last modified: 26/10 2021 08:48
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_COMPILE_H_
#define __LIBHY_HAL_INCLUDE_HY_COMPILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
    #ifdef BUILDING_DLL
        #ifdef __GNUC__
            #define EXPORT_FUNC __attribute__((dllexport))
        #else
            #define EXPORT_FUNC __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define EXPORT_FUNC __attribute__((dllimport))
        #else
            #define EXPORT_FUNC __declspec(dllimport)
        #endif
    #endif
#else
    #ifdef __GNUC__
        #if __GNUC__ >= 4 || defined(__arm__)
            #define EXPORT_FUNC __attribute__((visibility("default")))
            // #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
        #else
            #error "gcc version too low"
        #endif
    #endif
#endif

#define HY_WEAK __attribute__((weak))

#if 1
#   define HY_CHECK_FMT_WITH_PRINTF(a, b) __attribute__((format(printf, a, b)))
#else
#   define HY_CHECK_FMT_WITH_PRINTF(a, b)
#endif

#ifdef __GNUC__
#   define UNPACKED         __attribute__ ((packed))
#   define PACKED_4         __attribute__((aligned (4)))
#   define UNUSED           __attribute__((__unused__))
#else
#   define STRUCT_PACKED
#   define UNUSED
#endif

#if (__linux__ && __GLIBC__ && !__UCLIBC__) || __APPLE__
#endif

#ifdef __cplusplus
}
#endif

#endif

