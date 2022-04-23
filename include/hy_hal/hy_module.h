/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_module.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/10 2021 20:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/10 2021      create the file
 * 
 *     last modified: 29/10 2021 20:21
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_MODULE_H_
#define __LIBHY_HAL_INCLUDE_HY_MODULE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

#define _MODULE_ARRAY_CNT(array) (hy_u32_t)(sizeof((array)) / sizeof((array)[0]))

/**
 * @brief 创建句柄模块
 *
 * @param 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
typedef void *(*HyModuleCreateHandleCb_t)(void *config);

/**
 * @brief 销毁句柄模块
 *
 * @param 模块句柄的地址（二级指针）
 */
typedef void (*HyModuleDestroyHandleCb_t)(void **handle_pp);

/**
 * @brief 句柄模块创建结构体
 */
typedef struct {
    const char                  *name;                  ///< 模块名称
    void                        **handle;               ///< 模块句柄
    void                        *config;                ///< 模块配置参数
    HyModuleCreateHandleCb_t    create_handle_cb;       ///< 模块创建函数
    HyModuleDestroyHandleCb_t   destroy_handle_cb;      ///< 模块销毁函数
} HyModuleCreateHandle_s;

/**
 * @brief 句柄模块销毁结构体
 */
typedef struct {
    const char                  *name;                  ///< 模块名称
    void                        **handle;               ///< 模块句柄
    HyModuleDestroyHandleCb_t   destroy_handle_cb;      ///< 模块销毁函数
} HyModuleDestroyHandle_s;

#define HY_MODULE_RUN_CREATE_HANDLE(module)                                     \
    do {                                                                        \
        hy_u32_t i;                                                             \
        hy_u32_t len = _MODULE_ARRAY_CNT(module);                               \
        for (i = 0; i < len; ++i) {                                             \
            HyModuleCreateHandle_s *_create = &module[i];                       \
            if (_create->create_handle_cb) {                                    \
                *_create->handle = _create->create_handle_cb(_create->config);  \
                if (!*_create->handle) {                                        \
                    LOGE("%s create error \n", _create->name);                  \
                    break;                                                      \
                }                                                               \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (i < len) {                                                          \
            hy_s32_t j;                                                         \
            for (j = i - 1; j >= 0; j--) {                                      \
                HyModuleCreateHandle_s *_create = &module[j];                   \
                if (_create->destroy_handle_cb) {                               \
                    _create->destroy_handle_cb(_create->handle);                \
                }                                                               \
            }                                                                   \
            return NULL;                                                        \
        }                                                                       \
    } while(0)

#define HY_MODULE_RUN_DESTROY_HANDLE(module)                                    \
    do {                                                                        \
        hy_u32_t i;                                                             \
        for (i = 0; i < _MODULE_ARRAY_CNT(module); ++i) {                       \
            HyModuleDestroyHandle_s *_destroy = &module[i];                     \
            if (_destroy->destroy_handle_cb) {                                  \
                _destroy->destroy_handle_cb(_destroy->handle);                  \
            }                                                                   \
        }                                                                       \
    } while(0)

/**
 * @brief 创建bool模块
 *
 * @param 配置参数
 *
 * @return 成功返回0，失败返回-1
 */
typedef hy_s32_t *(*HyModuleCreateBoolCb_t)(void *config);

/**
 * @brief 销毁bool模块
 */
typedef void (*HyModuleDestroyBoolCb_t)(void);

/**
 * @brief bool模块创建结构体
 */
typedef struct {
    const char                  *name;                  ///< 模块名称
    void                        *config;                ///< 模块配置参数
    HyModuleCreateBoolCb_t      create_bool_cb;         ///< 模块创建函数
    HyModuleDestroyBoolCb_t     destroy_bool_cb;        ///< 模块销毁函数
} HyModuleCreateBool_s;

/**
 * @brief bool模块销毁结构体
 */
typedef struct {
    const char                  *name;                  ///< 模块名称
    HyModuleDestroyBoolCb_t     destroy_bool_cb;        ///< 模块销毁函数
} HyModuleDestroyBool_s;

#define HY_MODULE_RUN_CREATE_BOOL(module)                                       \
    do {                                                                        \
        hy_u32_t i;                                                             \
        hy_u32_t len = _MODULE_ARRAY_CNT(module);                               \
        for (i = 0; i < len; ++i) {                                             \
            HyModuleCreateBool_s *_create = &module[i];                         \
            if (_create->create_bool_cb) {                                      \
                if (0 != _create->create_bool_cb(_create->config)) {            \
                    LOGE("%s create error \n", _create->name);                  \
                    break;                                                      \
                }                                                               \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (i < len) {                                                          \
            hy_s32_t j;                                                         \
            for (j = i - 1; j >= 0; j--) {                                      \
                HyModuleCreateBool_s *_create = &module[j];                     \
                if (_create->destroy_bool_cb) {                                 \
                    _create->destroy_bool_cb();                                 \
                }                                                               \
            }                                                                   \
            return NULL;                                                        \
        }                                                                       \
    } while(0)

#define HY_MODULE_RUN_DESTROY_BOOL(module)                                      \
    do {                                                                        \
        hy_u32_t i;                                                             \
        for (i = 0; i < _MODULE_ARRAY_CNT(module); ++i) {                       \
            HyModuleDestroyBool_s *_destroy = &module[i];                       \
            if (_destroy->destroy_bool_cb) {                                    \
                _destroy->destroy_bool_cb();                                    \
            }                                                                   \
        }                                                                       \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif

