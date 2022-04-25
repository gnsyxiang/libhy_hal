/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_ipc_socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    17/01 2022 08:21
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        17/01 2022      create the file
 * 
 *     last modified: 17/01 2022 08:21
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_IPC_SOCKET_H_
#define __LIBHY_HAL_INCLUDE_HY_IPC_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

#define HY_IPC_SOCKET_NAME_LEN_MAX              (64)

/**
 * @brief ipc socket类型
 */
typedef enum {
    HY_IPC_SOCKET_TYPE_CLIENT,                  ///< 客户端
    HY_IPC_SOCKET_TYPE_SERVER,                  ///< 服务端

    HY_IPC_SOCKET_TYPE_MAX,
} HyIpcSocketType_e;

/**
 * @brief ipc socket连接状态
 */
typedef enum {
    HY_IPC_SOCKET_CONNECT_STATE_DISCONNECT,     ///< 断开
    HY_IPC_SOCKET_CONNECT_STATE_CONNECT,        ///< 连接

    HY_IPC_SOCKET_CONNECT_STATE_MAX,
} HyIpcSocketConnectState_e;

/**
 * @brief 接收客户端连接回调
 *
 * @param handle 句柄
 * @param args 上层传递参数
 *
 * @note handle是内部申请的空间，需要上层释放，否者造成内存泄露
 */
typedef void (*HyIpcSocketAcceptCb_t)(void *handle, void *args);

/**
 * @brief 配置参数
 */
typedef struct {
    const char                  *ipc_name;      ///< 服务器名字
    HyIpcSocketType_e           type;           ///< ipc socket类型
} HyIpcSocketConfig_s;

/**
 * @brief 创建ipc socket
 *
 * @param config 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyIpcSocketCreate(HyIpcSocketConfig_s *config);

/**
 * @brief 创建ipc socket
 *
 * @param _ipc_name 服务器名字
 * @param _type socket类型
 *
 * @return 成功返回句柄，失败返回NULL
 */
#define HyIpcSocketCreate_m(_ipc_name, _type)                   \
    ({                                                          \
        HyIpcSocketConfig_s socket_c;                           \
        HY_MEMSET(&socket_c, sizeof(socket_c));                 \
        socket_c.type       = _type;                            \
        socket_c.ipc_name   = _ipc_name;                        \
        HyIpcSocketCreate(&socket_c);                           \
     })

/**
 * @brief 销毁ipc socket
 *
 * @param handle 句柄的地址(二级指针)
 */
void HyIpcSocketDestroy(void **handle);

/**
 * @brief 读取数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 成功返回读到的字节数，失败返回-1
 */
hy_s32_t HyIpcSocketRead(void *handle, void *buf, hy_u32_t len);

/**
 * @brief 写入数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 成功返回写入的字节数，失败返回-1
 */
hy_s32_t HyIpcSocketWrite(void *handle, const void *buf, hy_u32_t len);

/**
 * @brief 获取ipc_socket通信fd
 *
 * @param handle 句柄
 *
 * @return 成功返回fd，失败返回-1
 */
hy_s32_t HyIpcSocketGetFD(void *handle);

/**
 * @brief 获取ipc_socket名字
 *
 * @param handle 句柄
 *
 * @return 成功返回ipc_name，失败返回NULL
 */
const char *HyIpcSocketGetName(void *handle);

/**
 * @brief 获取ipc_socket类型
 *
 * @param handle 句柄
 *
 * @return 成功返回状态，失败返回HY_IPC_SOCKET_TYPE_MAX
 */
HyIpcSocketType_e HyIpcSocketGetType(void *handle);

/**
 * @brief 获取ipc_socket连接状态
 *
 * @param handle 句柄
 *
 * @return 成功返回状态，失败返回HY_IPC_SOCKET_CONNECT_STATE_MAX
 */
HyIpcSocketConnectState_e HyIpcSocketGetConnectState(void *handle);

/**
 * @brief 服务端等待客户端连接
 *
 * @param handle 句柄
 * @param accept_cb 接收客户端回调
 * @param args 上层传递的参数
 *
 * @return 错误返回-1，退出返回0
 *
 * @note accept_cb中的handle是内部申请的空间，需要上层释放
 */
hy_s32_t HyIpcSocketAccept(void *handle,
        HyIpcSocketAcceptCb_t accept_cb, void *args);

/**
 * @brief 客户端连接服务器
 *
 * @param handle 句柄
 * @param timeout_s 超时时间，0表示阻塞执行
 *
 * @return 成功返回0，失败返回-1
 *
 * @note 如果是阻塞执行，ctrl+c将不能完全释放资源
 */
hy_s32_t HyIpcSocketConnect(void *handle, hy_u32_t timeout_s);

#ifdef __cplusplus
}
#endif

#endif
