/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    socket_node_fd.h
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
#ifndef __LIBHY_HAL_INCLUDE_SOCKET_NODE_FD_H_
#define __LIBHY_HAL_INCLUDE_SOCKET_NODE_FD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_list.h"
#include "hy_type.h"

struct epoll_helper_cb_param_tag;

typedef struct {
    struct epoll_helper_cb_param_tag    *cb_param;
    struct hy_list_head                 entry;
} socket_node_fd_s;

socket_node_fd_s *socket_node_fd_create(hy_s32_t fd, void *args);
void socket_node_fd_destroy(socket_node_fd_s **context_pp);

void socket_node_fd_list_destroy(struct hy_list_head *list, hy_s32_t fd);

#ifdef __cplusplus
}
#endif

#endif

