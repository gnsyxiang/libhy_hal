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
#include <unistd.h>

#include "log_private.h"

#include "epoll_helper.h"

hy_s32_t epoll_helper_del(epoll_helper_s *context,
        epoll_helper_cb_param_s *cb_param)
{
    assert(context);
    assert(cb_param);

    return epoll_ctl(context->fd, EPOLL_CTL_DEL, cb_param->fd, NULL);
}

hy_s32_t epoll_helper_add(epoll_helper_s *context,
        hy_u32_t event, epoll_helper_cb_param_s *cb_param)
{
    assert(context);
    assert(cb_param);
    struct epoll_event ev;

    ev.events   = event;
    ev.data.ptr = cb_param;
    return epoll_ctl(context->fd, EPOLL_CTL_ADD, cb_param->fd, &ev);
}

static void *_epoll_thread_cb(void *args)
{
    hy_s32_t ret = 0;
    hy_u32_t len = 0;
    epoll_helper_s *context = args;
    epoll_helper_cb_param_s *cb_param = NULL;
    struct epoll_event *events = NULL;

    len = context->max_event * sizeof(struct epoll_event);
    events = calloc(1, len);
    if (!events) {
        log_error("calloc failed \n");
        return NULL;
    }

    while (!context->is_exit) {
        memset(events, '\0', len);
        ret = epoll_wait(context->fd, events, context->max_event, -1);
        if (-1 == ret) {
            log_error("epoll_wait failed \n");
            break;
        }
        // log_info("epoll_wait ret: %d \n", ret);

        for (hy_s32_t i = 0; i < ret; ++i) {
            cb_param = events[i].data.ptr;
            ret = epoll_ctl(context->fd, EPOLL_CTL_DEL, cb_param->fd, NULL);
            if (-1 == ret) {
                log_error("epoll_ctl failed \n");
                break;
            }

            if (cb_param->fd == context->pipe_fd[0]) {
                log_error("exit epoll wait \n");
                goto _L_EPOLL_1;
            }

            if (context->epoll_helper_cb) {
                context->epoll_helper_cb(cb_param);
            }
        }
    }

_L_EPOLL_1:
    context->wait_exit_flag = 1;

    if (events) {
        free(events);
        events = NULL;
    }

    return NULL;
}

void epoll_helper_destroy(epoll_helper_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is NULL \n");
        return;
    }

    epoll_helper_s *context = *context_pp;
    log_info("epoll helper context: %p destroy, thread_id: 0x%lx, "
            "epoll_fd: %d, pipe_fd[0]: %d, pipe_fd[1]: %d \n",
            context, context->id, context->fd,
            context->pipe_fd[0], context->pipe_fd[1]);

    context->is_exit = 1;
    write(context->pipe_fd[1], context, sizeof(*context));
    while (!context->wait_exit_flag) {
        usleep(10 * 1000);
    }
    pthread_join(context->id, NULL);

    close(context->pipe_fd[0]);
    close(context->pipe_fd[1]);

    close(context->fd);

    free(context);
    *context_pp = NULL;
}

epoll_helper_s *epoll_helper_create(const char *name, hy_u32_t max_event,
        epoll_helper_cb_t epoll_helper_cb)
{
    if (!name || max_event <=0 || !epoll_helper_cb) {
        log_error("the param is NULL \n");
        return NULL;
    }

    epoll_helper_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->epoll_helper_cb    = epoll_helper_cb;
        context->max_event          = max_event;

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

        if (0 != pthread_create(&context->id, NULL, _epoll_thread_cb, context)) {
            log_error("pthread_create failed \n");
            break;
        }

#ifdef _GNU_SOURCE
        pthread_setname_np(context->id, name);
#endif

        log_info("epoll helper context: %p create, thread_id: 0x%lx, "
                "epoll_fd: %d, pipe_fd[0]: %d, pipe_fd[1]: %d \n",
                context, context->id, context->fd,
                context->pipe_fd[0], context->pipe_fd[1]);
        return context;
    } while (0);

    log_error("epoll helper context: %p create failed \n", context);
    epoll_helper_destroy(&context);
    return NULL;
}

