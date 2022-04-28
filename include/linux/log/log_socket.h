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

struct epoll_helper_tag;
struct epoll_helper_cb_param_tag;

typedef void (*log_socket_accept_cb_t)(hy_s32_t fd, void *args);

typedef struct log_socket_context_tag {
    hy_s32_t                            fd;
    struct epoll_helper_tag             *epoll_helper;
    struct epoll_helper_cb_param_tag    *cb_param;

    log_socket_accept_cb_t              accept_cb;
    void                                *args;
} log_socket_context_s;

log_socket_context_s *log_socket_create(hy_u16_t port,
        log_socket_accept_cb_t accept_cb, void *args);

void log_socket_destroy(log_socket_context_s **context_pp);

#ifdef __cplusplus
}
#endif

#endif

