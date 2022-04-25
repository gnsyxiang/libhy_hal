/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_ipc_socket.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 15:54
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 15:54
 */
#include <stdio.h>

#include "hy_hal/hy_assert.h"
#include "hy_hal/hy_file.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_string.h"
#include "hy_hal/hy_log.h"

#include "ipc_socket_client.h"
#include "ipc_socket_server.h"
#include "hy_ipc_socket.h"

hy_s32_t HyIpcSocketConnect(void *handle, hy_u32_t timeout_s)
{
    LOGT("handle: %p, timeout_s: %d \n", handle, timeout_s);
    HY_ASSERT_RET_VAL(!handle, -1);

    return ipc_socket_client_connect(handle, timeout_s);
}

hy_s32_t HyIpcSocketAccept(void *handle,
        HyIpcSocketAcceptCb_t accept_cb, void *args)
{
    LOGT("handle: %p, accept_cb: %p, args: %p \n", handle, accept_cb, args);
    HY_ASSERT_RET_VAL(!handle || !accept_cb, -1);

    return ipc_socket_server_accept(handle, accept_cb, args);
}

HyIpcSocketConnectState_e HyIpcSocketGetConnectState(void *handle)
{
    LOGT("handle: %p \n", handle);
    HY_ASSERT_RET_VAL(!handle, HY_IPC_SOCKET_CONNECT_STATE_MAX);

    return ((ipc_socket_s *)handle)->connect_state;
}

HyIpcSocketType_e HyIpcSocketGetType(void *handle)
{
    LOGT("handle: %p \n", handle);
    HY_ASSERT_RET_VAL(!handle, HY_IPC_SOCKET_TYPE_MAX);

    return ((ipc_socket_s *)handle)->type;
}

const char *HyIpcSocketGetName(void *handle)
{
    LOGT("handle: %p \n", handle);
    HY_ASSERT_RET_VAL(!handle, NULL);

    return ((ipc_socket_s *)handle)->ipc_name;
}

hy_s32_t HyIpcSocketGetFD(void *handle)
{
    LOGT("handle: %p \n", handle);
    HY_ASSERT_RET_VAL(!handle, -1);

    return ((ipc_socket_s *)handle)->fd;
}

hy_s32_t HyIpcSocketRead(void *handle, void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    hy_s32_t ret = 0;
    hy_s32_t fd = ((ipc_socket_s *)handle)->fd;

    ret = HyFileReadN(fd, buf, len);
    if (-1 == ret) {
        LOGE("hy file read n failed \n");

        close(fd);
        return -1;
    } else if (ret >= 0 && ret != (hy_s32_t)len) {
        LOGE("hy file read n error \n");

        return -1;
    } else {
        return len;
    }
}

hy_s32_t HyIpcSocketWrite(void *handle, const void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    return HyFileWriteN(((ipc_socket_s *)handle)->fd, buf, len);
}

void HyIpcSocketDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    ipc_socket_s *socket = *handle;

    void (*socket_destroy[HY_IPC_SOCKET_TYPE_MAX])(void **handle) = {
        ipc_socket_client_destroy,
        ipc_socket_server_destroy,
    };

    socket_destroy[socket->type](handle);
}

void *HyIpcSocketCreate(HyIpcSocketConfig_s *config)
{
    LOGT("config: %p \n", config);
    HY_ASSERT_RET_VAL(!config, NULL);

    void *(*socket_create[HY_IPC_SOCKET_TYPE_MAX])(const char *ipc_name,
            HyIpcSocketType_e type) = {
        ipc_socket_client_create,
        ipc_socket_server_create,
    };

    return socket_create[config->type](config->ipc_name, config->type);
}
