/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/03 2022 15:06
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/03 2022      create the file
 * 
 *     last modified: 30/03 2022 15:06
 */
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#include "hy_type.h"
#include "hy_assert.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_log.h"
#include "hy_file.h"

#include "hy_socket.h"

typedef struct {
    hy_s32_t    fd;

    hy_s32_t    is_exit;
} _socket_context_s;

hy_s32_t HYSocketRead(void *handle, void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    _socket_context_s *context = handle;

    return read(context->fd, buf, len);
}

hy_s32_t HYSocketReadN(void *handle, void *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    hy_s32_t ret = 0;
    _socket_context_s *context = handle;

    ret = HyFileReadN(context->fd, buf, len);
    if (-1 == ret) {
        LOGE("hy file read n failed \n");

        close(context->fd);
        context->fd = -1;
        return -1;
    } else if (ret >= 0 && ret != (hy_s32_t)len) {
        LOGE("hy file read n error \n");

        return -1;
    } else {
        return len;
    }
}

hy_s32_t HYSocketWrite(void *handle, const char *buf, hy_u32_t len)
{
    HY_ASSERT(handle);
    HY_ASSERT(buf);

    return HyFileWriteN(((_socket_context_s *)handle)->fd, buf, len);
}

hy_s32_t HySocketWaitConnect(void *handle, hy_u16_t port,
        HySocketNewClientCb_t new_client_cb, void *args)
{
    LOGT("handle: %p, port: %d, new_client_cb: %p, args: %p \n",
            handle, port, new_client_cb, args);
    HY_ASSERT_RET_VAL(!handle || !new_client_cb, -1);

    _socket_context_s *context = handle;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t addrlen;
    hy_s32_t new_fd = -1;
    _socket_context_s *new_context = NULL;

    int opt = SO_REUSEADDR;
    setsockopt(context->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    HY_MEMSET(&server, sizeof(server));
    server.sin_family       = AF_INET;
    server.sin_port         = htons(port);
    server.sin_addr.s_addr  = htonl(INADDR_ANY);

    if (bind(context->fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        LOGES("bind failed \n");
        return -1;
    }

    if (listen(context->fd, 10) == -1) {
        LOGES("listen failed \n");
        return -1;
    }

    addrlen = sizeof(client);
    while (!context->is_exit) {
        new_fd = accept(context->fd, (struct sockaddr *)&client, &addrlen);
        if (new_fd == -1) {
            LOGES("accept failed \n");
            break;
        }

        if (new_client_cb) {
            new_context = HY_MEM_MALLOC_BREAK(_socket_context_s *,
                    sizeof(new_context));
            new_context->fd = new_fd;

            new_client_cb(new_context, inet_ntoa(client.sin_addr), args);
        }
    }

    LOGI("exit accept \n");
    return -1;
}

hy_s32_t HySocketConnect(void *handle, const char *ip, hy_u16_t port)
{
    LOGT("handle: %p, ip: %s, port: %d \n", handle, ip, port);
    HY_ASSERT_RET_VAL(!handle || !ip, -1);

    _socket_context_s *context = handle;

    do {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        if (1 != inet_pton(AF_INET, ip, &addr.sin_addr)) {
            LOGES("inet_pton faild, ip: %s, port: %d \n", ip, port);
            break;
        }

        if (0 != connect(context->fd, (struct sockaddr*)&addr, sizeof(addr))) {
            LOGES("connect faild \n");
            break;
        }

        LOGI("socket connect to server successful \n");
        return 0;
    } while (0);

    LOGE("socket connect to server failed \n");
    return -1;
}

void HySocketDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _socket_context_s *context = *handle;
    LOGI("socket destroy, context: %p, fd: %d \n", context, context->fd);

    context->is_exit = 1;
    if (context->fd > 0) {
        close(context->fd);
    }

    HY_MEM_FREE_PP(handle);
}

void *HySocketCreate(HySocketConfig_t *socket_c)
{
    LOGT("socket_c: %p \n", socket_c);
    HY_ASSERT_RET_VAL(!socket_c, NULL);

    _socket_context_s *context = NULL;

    do {
        context = HY_MEM_MALLOC_BREAK(_socket_context_s *, sizeof(*context));

        context->fd = socket(AF_INET, SOCK_STREAM, 0);
        if (context->fd < 0) {
            LOGES("socket faild \n");
            break;
        }

        LOGI("socket create, context: %p, fd: %d \n", context, context->fd);
        return context;
    } while (0);

    LOGE("socket create failed \n");
    HySocketDestroy((void **)&context);
    return NULL;
}

