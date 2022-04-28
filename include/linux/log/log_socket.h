/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    log_socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 15:46
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 15:46
 */
#ifndef __LIBHY_HAL_INCLUDE_LOG_SOCKET_H_
#define __LIBHY_HAL_INCLUDE_LOG_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_type.h"

/**
 * @brief 
 */
typedef enum {
    LOG_SOCKET_TYPE_CLIENT,   ///< 
    LOG_SOCKET_TYPE_SERVER,   ///< 
} log_socket_type_e;

hy_s32_t log_socket_ipc_create(const char *ip,
        hy_u16_t port, log_socket_type_e type);

#ifdef __cplusplus
}
#endif

#endif

