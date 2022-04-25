/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    ipc_socket_client.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 16:38
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 16:38
 */
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stddef.h>

#include "hy_hal/hy_assert.h"
#include "hy_hal/hy_log.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_string.h"

#include "ipc_socket_client.h"

typedef struct {
    ipc_socket_s    ipc_socket;         // 放在前面，用于强制类型转换
} _ipc_socket_client_s;

hy_s32_t ipc_socket_client_connect(void *handle, hy_u32_t timeout_s)
{
    LOGT("handle: %p, timeout_s: %d \n", handle, timeout_s);
    HY_ASSERT_RET_VAL(!handle, -1);

    _ipc_socket_client_s *socket_client = handle;
    ipc_socket_s *ipc_socket = &socket_client->ipc_socket;

    hy_s32_t ret = 0;
    hy_u32_t time_cnt = 0;
    hy_u32_t addr_len = 0;
    struct sockaddr_un addr;

    HY_IPC_SOCKADDR_UN_INIT_(addr, addr_len, ipc_socket->ipc_name);

    do {
        ret = connect(ipc_socket->fd, (const struct sockaddr *)&addr, addr_len);
        if (ret < 0) {
            LOGES("connect failed, fd: %d, sleep and continue \n",
                    ipc_socket->fd);

            sleep(1);
        }
    } while ((ret < 0) && (0 == timeout_s || ++time_cnt < timeout_s));

    if (0 == ret && (0 == timeout_s || time_cnt < timeout_s)) {
        LOGI("connect ipc socket server, fd: %d \n", ipc_socket->fd);

        ipc_socket->connect_state = HY_IPC_SOCKET_CONNECT_STATE_CONNECT;
        return 0;
    } else {
        LOGE("connect ipc server failed \n");
        return -1;
    }
}

void ipc_socket_client_destroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _ipc_socket_client_s *socket_client = *handle;

    ipc_socket_destroy_2(&socket_client->ipc_socket);

    LOGI("ipc socket client destroy, socket_client: %p \n", socket_client);
    HY_MEM_FREE_PP(handle);
}

void *ipc_socket_client_create(const char *ipc_name, HyIpcSocketType_e type)
{
    LOGT("ipc_name: %s, type: %d \n", ipc_name, type);
    HY_ASSERT_RET_VAL(!ipc_name, NULL);

    _ipc_socket_client_s *socket_client = NULL;

    do {
        socket_client = HY_MEM_MALLOC_BREAK(_ipc_socket_client_s *,
                sizeof(*socket_client));

        if (0 != ipc_socket_create_2(&socket_client->ipc_socket, ipc_name, type)) {
            LOGE("ipc socket create 2 failed \n");
            break;
        }

        LOGI("ipc socket client create, socket_client: %p \n", socket_client);
        return socket_client;
    } while (0);

    LOGE("ipc socket client create failed \n");
    ipc_socket_client_destroy((void **)&socket_client);
    return NULL;
}
