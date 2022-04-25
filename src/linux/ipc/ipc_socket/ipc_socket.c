/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    ipc_socket_private.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/01 2022 11:13
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/01 2022      create the file
 * 
 *     last modified: 22/01 2022 11:13
 */
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include "hy_hal/hy_assert.h"
#include "hy_hal/hy_log.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_string.h"

#include "ipc_socket.h"

void ipc_socket_destroy_2(ipc_socket_s *ipc_socket)
{
    LOGT("ipc_socket: %p \n", ipc_socket);
    HY_ASSERT_RET(!ipc_socket);

    LOGI("ipc socket destroy 2, fd: %d \n", ipc_socket->fd);

    close(ipc_socket->fd);
    ipc_socket->fd = -1;
}

hy_s32_t ipc_socket_create_2(ipc_socket_s *ipc_socket,
        const char *ipc_name, HyIpcSocketType_e type)
{
    LOGT("ipc_socket: %p, ipc_name: %s, type: %d \n",
            ipc_socket, ipc_name, type);
    HY_ASSERT_RET_VAL(!ipc_socket || !ipc_name, -1);

    hy_s32_t fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        LOGES("socket failed \n");
        return -1;
    }

    ipc_socket->fd      = fd;
    ipc_socket->type    = type;
    HY_MEMCPY(ipc_socket->ipc_name, ipc_name, HY_STRLEN(ipc_name));

    LOGI("ipc socket create 2, fd: %d \n", ipc_socket->fd);
    return 0;
}

void ipc_socket_destroy(ipc_socket_s **ipc_socket_pp)
{
    LOGT("&ipc_socket: %p, ipc_socket: %p \n", ipc_socket_pp, *ipc_socket_pp);
    HY_ASSERT_RET(!ipc_socket_pp|| !*ipc_socket_pp);

    ipc_socket_s *socket = *ipc_socket_pp;

    LOGI("ipc socket destroy, socket: %p, fd: %d \n", socket, socket->fd);

    close(socket->fd);
    socket->fd = -1;

    HY_MEM_FREE_PP(ipc_socket_pp);
}

ipc_socket_s *ipc_socket_create(hy_s32_t fd,
        const char *ipc_name, HyIpcSocketType_e type)
{
    LOGT("fd: %d, ipc_name: %s, type: %d \n", fd, ipc_name, type);
    HY_ASSERT_RET_VAL(!ipc_name, NULL);

    ipc_socket_s *socket = NULL;

    do {
        socket = HY_MEM_MALLOC_BREAK(ipc_socket_s *, sizeof(*socket));

        socket->fd      = fd;
        socket->type    = type;
        HY_MEMCPY(socket->ipc_name, ipc_name, HY_STRLEN(ipc_name));

        LOGI("ipc socket create, socket: %p, fd: %d \n", socket, socket->fd);
        return socket;
    } while (0);

    LOGE("ipc socket create failed \n");
    ipc_socket_destroy(&socket);
    return NULL;
}
