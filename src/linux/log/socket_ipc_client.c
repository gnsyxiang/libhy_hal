/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    socket_ipc_client.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/04 2022 11:56
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/04 2022      create the file
 * 
 *     last modified: 26/04 2022 11:56
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>
#include <assert.h>

#include "log_file.h"

#include "socket_ipc_client.h"

hy_s32_t socket_ipc_client_write(socket_ipc_client_s *context,
        const void *buf, hy_u32_t len)
{
    assert(context);
    assert(buf);

    return log_file_write(context->fd, buf, len);
}

void socket_ipc_client_destroy(socket_ipc_client_s **context_pp)
{
    if (!context_pp || !*context_pp) {
        log_error("the param is error \n");
        return;
    }
    socket_ipc_client_s *context = *context_pp;
    log_info("socket ipc client context: %p create, fd: %d \n",
            context, context->fd);

    close(context->fd);

    free(context);
    *context_pp = NULL;
}

socket_ipc_client_s *socket_ipc_client_create(const char *name)
{
    if (!name || strlen(name) <= 0) {
        log_error("the param is error \n");
        return NULL;
    }

    socket_ipc_client_s *context = NULL;
    do {
        context = calloc(1, sizeof(*context));
        if (!context) {
            log_error("calloc failed \n");
            break;
        }

        context->fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (context->fd < 0) {
            log_error("socket failed \n");
            break;
        }

        char ipc_path[SOCKET_IPC_SERVER_NAME_LEN_MAX] = {0};
        snprintf(ipc_path, sizeof(ipc_path), "%s/%s", "/tmp", name);

        hy_u32_t addr_len;
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, ipc_path);
        addr_len = strlen(ipc_path) + offsetof(struct sockaddr_un, sun_path);

        if (connect(context->fd, (const struct sockaddr *)&addr, addr_len) < 0) {
            close(context->fd);
            context->fd = -1;
        }

        log_info("socket ipc client context: %p create, fd: %d \n",
                context, context->fd);
        return context;
    } while (0);

    log_error("socket ipc client context: %p create failed \n", context);
    socket_ipc_client_destroy(&context);
    return NULL;
}

