/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    log_socket.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 15:51
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 15:51
 */
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>

#include "log_private.h"

#include "log_socket.h"

hy_s32_t log_socket_create(const char *ip,
        hy_u16_t port, log_socket_type_e type)
{
    hy_s32_t fd = -1;
    do {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            log_error("socket faild \n");
            break;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        if (1 != inet_pton(AF_INET, ip, &addr.sin_addr)) {
            log_error("inet_pton faild, ip: %s, port: %d \n", ip, port);
            break;
        }

        if (type == LOG_SOCKET_TYPE_CLIENT) {
            if (0 != connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
                log_error("connect faild \n");
                break;
            }
        } else {
            addr.sin_addr.s_addr  = htonl(INADDR_ANY);

            if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
                log_error("bind failed \n");
                break;
            }

            if (listen(fd, 10) == -1) {
                log_error("listen failed \n");
                break;
            }
        }

        log_info("log socket fd: %d create \n", fd);
        return fd;
    } while (0);

    log_error("log socket fd: %d create \n", fd);
    return -1;
}

hy_s32_t log_socket_ipc_create(const char *name, log_socket_type_e type)
{
    if (!name || strlen(name) <= 0) {
        log_error("the param is error \n");
        return -1;
    }
    hy_s32_t fd = -1;

    do {
        fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd < 0) {
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

        if (type == LOG_SOCKET_TYPE_IPC_CLIENT) {
            if (connect(fd, (const struct sockaddr *)&addr, addr_len) < 0) {
                log_error("connect failed fd: %d \n", fd);
                break;
            }
        } else {
            if (0 == access(ipc_path, F_OK)) {
                remove(ipc_path);
            }

            if (bind(fd, (const struct sockaddr *)&addr, addr_len) < 0) {
                log_error("bind failed, fd: %d \n", fd);
                break;
            }

            if (listen(fd, SOMAXCONN) < 0) {
                log_error("listen failed, fd: %d \n", fd);
                break;
            }
        }

        log_info("log socket ipc fd: %d create \n", fd);
        return fd;
    } while (0);

    if (fd > 0) {
        close(fd);
        // fd = -1;
    }

    log_error("log socket ipc fd: %d create failed \n", fd);
    return -1;
}

