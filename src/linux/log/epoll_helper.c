/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    epoll_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    27/04 2022 15:48
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        27/04 2022      create the file
 * 
 *     last modified: 27/04 2022 15:48
 */
#include <stdio.h>
#include <stdlib.h>

#include "log_private.h"

#include "epoll_helper.h"

hy_s32_t epoll_helper_context_set(epoll_helper_context_s *context,
        hy_s32_t fd, hy_u32_t event, void *args)
{
    struct epoll_event ev;

    ev.events   = event;
    ev.data.ptr = args;
    if(-1 == epoll_ctl(context->fd, EPOLL_CTL_ADD, fd, &ev)) {
        log_error("epoll_ctl failed \n");
        return -1;
    } else {
        return 0;
    }
}

static void *_thread_cb(void *args)
{
    #define MX_EVNTS 100
    hy_s32_t ret = 0;
    struct epoll_event events[MX_EVNTS];
    epoll_helper_context_s *context = args;

    while (!context->is_exit) {
        memset(events, '\0', sizeof(events));
        ret = epoll_wait(context->fd, events, MX_EVNTS, -1);
        if (-1 == ret) {
            log_error("epoll_wait failed \n");
            break;
        }

        for (hy_s32_t i = 0; i < ret; ++i) {
            ret = epoll_ctl(context->fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
            if (-1 == ret) {
                log_error("epoll_ctl failed \n");
                break;
            }

            if (events[i].data.fd == context->pipe_fd[0]) {
                log_error("exit epoll wait \n");
                goto _L_EPOLL_1;
            }

            if (context->epoll_helper_cb) {
                context->epoll_helper_cb(events[i].data.fd, events[i].data.ptr);
            }
        }
    }

_L_EPOLL_1:
    context->wait_exit_flag = 1;

    return NULL;
}

void epoll_helper_destroy(epoll_helper_context_s **context_pp)
{
    epoll_helper_context_s *context = *context_pp;

    context->is_exit = 1;
    write(context->pipe_fd[1], context, sizeof(*context));
    while (!context->wait_exit_flag) {
        usleep(10 * 1000);
    }
    pthread_join(context->id, NULL);

    close(context->fd);

    free(context);
    *context_pp = NULL;
}

epoll_helper_context_s *epoll_helper_create(epoll_helper_cb_t epoll_helper_cb)
{
    epoll_helper_context_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->fd = epoll_create1(0);
        if(-1 == context->fd){
            log_error("epoll_create1 failed \n");
            break;
        }

        if (0 != pipe(context->pipe_fd)) {
            log_error("pipe failed \n");
            break;
        }

        struct epoll_event ev;
        ev.events   = EPOLLIN | EPOLLET;
        ev.data.ptr = &context->pipe_fd[0];
        if (-1 == epoll_ctl(context->fd, EPOLL_CTL_ADD, context->pipe_fd[0], &ev)) {
            log_error("epoll_ctl failed \n");
            break;
        }

        if (0 != pthread_create(&context->id, NULL, _thread_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

        context->epoll_helper_cb = epoll_helper_cb;

        return context;
    } while (0);

    return NULL;
}

