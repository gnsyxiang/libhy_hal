/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    ipc_socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 09:17
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 09:17
 */
#ifndef __LIBHY_HAL_INCLUDE_IPC_SOCKET_H_
#define __LIBHY_HAL_INCLUDE_IPC_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#include "hy_ipc_socket.h"

#define HY_IPC_SOCKET_PATH_     "/tmp"

#define HY_IPC_SOCKADDR_UN_INIT_(addr, addr_len, ipc_name)                      \
    do {                                                                        \
        char ipc_path[HY_IPC_SOCKET_NAME_LEN_MAX] = {0};                        \
        addr.sun_family = AF_UNIX;                                              \
        snprintf(ipc_path, HY_IPC_SOCKET_NAME_LEN_MAX,                          \
                "%s/%s", HY_IPC_SOCKET_PATH_, ipc_name);                        \
        strcpy(addr.sun_path, ipc_path);                                        \
        \
        addr_len = strlen(ipc_path) + offsetof(struct sockaddr_un, sun_path);   \
    } while (0)

typedef struct {
    hy_s32_t                    fd;

    char                        ipc_name[HY_IPC_SOCKET_NAME_LEN_MAX / 2];
    HyIpcSocketType_e           type:2;
    HyIpcSocketConnectState_e   connect_state:2;
    hy_s32_t                    reserved;
} ipc_socket_s;

ipc_socket_s *ipc_socket_create(hy_s32_t fd,
        const char *ipc_name, HyIpcSocketType_e type);
void ipc_socket_destroy(ipc_socket_s **ipc_socket_pp);

hy_s32_t ipc_socket_create_2(ipc_socket_s *ipc_socket,
        const char *ipc_name, HyIpcSocketType_e type);
void ipc_socket_destroy_2(ipc_socket_s *ipc_socket);

#ifdef __cplusplus
}
#endif

#endif
