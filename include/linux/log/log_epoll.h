/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    log_epoll.h
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
#ifndef __LIBHY_HAL_INCLUDE_LOG_EPOLL_H_
#define __LIBHY_HAL_INCLUDE_LOG_EPOLL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/epoll.h>
#include <pthread.h>

#include "hy_type.h"

/**
 * @brief 回调函数的参数
 */
typedef struct {
    hy_s32_t            fd;                 ///< fd
    hy_s32_t            type;               ///< fd类型
    void                *args;              ///< fd所携带的用户参数
} log_epoll_cb_param_s;

/**
 * @brief 回调函数
 *
 * @param cb_param 参数
 *
 * @return 无
 */
typedef void (*log_epoll_cb_t)(log_epoll_cb_param_s *cb_param);

/**
 * @brief epoll上下文
 */
typedef struct log_epoll_tag {
    hy_u32_t            max_event;          ///< 内核监控的最大数
    hy_s32_t            fd;                 ///< epoll文件描述符

    log_epoll_cb_t      log_epoll_cb;       ///< 回调函数

    pthread_t           id;                 ///< 线程id
    hy_s32_t            is_exit;            ///< 控制线程退出
    hy_s32_t            pipe_fd[2];         ///< pipe文件描述符，控制线程退出
    hy_s32_t            wait_exit_flag;     ///< 控制线程退出标志位
} log_epoll_s;

/**
 * @brief 创建epoll模块
 *
 * @param max_event 内核监控的最大数
 * @param log_epoll_cb 回调函数
 *
 * @return 成功返回句柄，失败返回NULL
 */
log_epoll_s *log_epoll_create(const char *name, hy_u32_t max_event,
        log_epoll_cb_t log_epoll_cb);

/**
 * @brief 销毁epoll模块
 *
 * @param context_pp 句柄的地址（二级指针）
 */
void log_epoll_destroy(log_epoll_s **context_pp);

/**
 * @brief 增加监控对象
 *
 * @param context 句柄
 * @param event epoll事件
 * @param cb_param 回调函数的参数
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t log_epoll_add(log_epoll_s *context,
        hy_u32_t event, log_epoll_cb_param_s *cb_param);

/**
 * @brief 删除监控对象
 *
 * @param context 句柄
 * @param cb_param 回调函数的参数
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t log_epoll_del(log_epoll_s *context, log_epoll_cb_param_s *cb_param);

#ifdef __cplusplus
}
#endif

#endif

