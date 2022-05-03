/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    socket_fd_node.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 15:11
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 15:11
 */
#ifndef __LIBHY_HAL_INCLUDE_SOCKET_FD_NODE_H_
#define __LIBHY_HAL_INCLUDE_SOCKET_FD_NODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_list.h"
#include "log_epoll.h"

typedef struct {
    log_epoll_cb_param_s        cb_param;
    struct hy_list_head         entry;
} socket_fd_node_s;

socket_fd_node_s *socket_fd_node_create(hy_s32_t fd, hy_s32_t type, void *args);
void socket_fd_node_destroy(socket_fd_node_s **context_pp);

void socket_fd_node_list_destroy(struct hy_list_head *list, hy_s32_t fd);

#ifdef __cplusplus
}
#endif

#endif

