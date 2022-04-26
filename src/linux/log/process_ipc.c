/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_ipc.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/04 2022 17:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/04 2022      create the file
 * 
 *     last modified: 25/04 2022 17:27
 */
#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"
#include "log_private.h"
#include "socket_ipc_server.h"

#include "process_ipc.h"

typedef struct {
    hy_s32_t                is_exit;
    pthread_t               thread;

    socket_ipc_server_s     *socket_ipc_server;

    fifo_context_s          *fifo;
    pthread_mutex_t         mutex;
    pthread_cond_t          cond;
} _process_ipc_context_s;

static void _accept_cb(hy_s32_t fd)
{
    printf("---------fd: %d \n", fd);
}

void process_ipc_server_destroy(void **handle)
{

}

void *process_ipc_server_create(hy_u32_t fifo_len)
{
    if (fifo_len <= 0) {
        log_error("the param is error \n");
        return NULL;
    }

    _process_ipc_context_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->socket_ipc_server = socket_ipc_server_create("hy_ipc_server",
                _accept_cb);
        if (!context->socket_ipc_server) {
            log_error("socket_ipc_server_create failed \n");
            break;
        }

        return context;
    } while (0);

    return NULL;
}

