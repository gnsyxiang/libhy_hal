/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    epoll_helper.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/04 2022 15:37
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/04 2022      create the file
 * 
 *     last modified: 27/04 2022 15:37
 */
#ifndef __LIBHY_HAL_INCLUDE_EPOLL_HELPER_H_
#define __LIBHY_HAL_INCLUDE_EPOLL_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/epoll.h>
#include <pthread.h>

#include "hy_type.h"

/**
 * @brief 
 */
typedef struct {
    hy_s32_t    fd;   ///< 
    void        *args;
} epoll_helper_cb_param_s;

/**
 * @brief 
 *
 * @param args 
 *
 * @return 
 */
typedef void (*epoll_helper_cb_t)(epoll_helper_cb_param_s *cb_param);

/**
 * @brief 
 */
typedef struct {
    hy_s32_t            fd;   ///< 
    pthread_t           id;
    hy_s32_t            is_exit;

    epoll_helper_cb_t   epoll_helper_cb;

    hy_s32_t            wait_exit_flag;
    hy_s32_t            pipe_fd[2];
} epoll_helper_context_s;

epoll_helper_context_s *epoll_helper_create(epoll_helper_cb_t epoll_helper_cb);
void epoll_helper_destroy(epoll_helper_context_s **context_pp);

hy_s32_t epoll_helper_context_set(epoll_helper_context_s *context,
        hy_u32_t event, epoll_helper_cb_param_s *cb_param);

#ifdef __cplusplus
}
#endif

#endif

