/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/03 2022 09:19
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/03 2022      create the file
 * 
 *     last modified: 30/03 2022 09:19
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_SOCKET_H_
#define __LIBHY_HAL_INCLUDE_HY_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

/**
 * @brief socket类型
 */
typedef enum {
    HY_SOCKET_TYPE_CLIENT,                  ///< 客户端
    HY_SOCKET_TYPE_SERVER,                  ///< 服务端

    HY_SOCKET_TYPE_MAX,
} HySocketType_e;

/**
 * @brief 连接状态
 */
typedef enum {
    HY_SOCKET_CONNECT_STATE_DISCONNECT,     ///< 断开
    HY_SOCKET_CONNECT_STATE_CONNECT,        ///< 连接

    HY_SOCKET_CONNECT_STATE_MAX,
} HySocketConnectState_e;

/**
 * @brief 新连接回调
 *
 * @param handle 句柄
 * @param ip ip地址
 * @param args 上层传递参数
 *
 * @return 无
 */
typedef void (*HySocketNewClientCb_t)(void *handle, const char *ip, void *args);

/**
 * @brief 配置结构
 */
typedef struct {
    HySocketType_e      type;               ///< 设备类型
} HySocketConfig_t;

/**
 * @brief 创建模块
 *
 * @param socket_c 配置结构
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HySocketCreate(HySocketConfig_t *socket_c);

/**
 * @brief 销毁模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HySocketDestroy(void **handle);

/**
 * @brief 读取数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 成功返回读到的字节数，失败返回-1
 */
hy_s32_t HYSocketRead(void *handle, void *buf, hy_u32_t len);

/**
 * @brief 写入数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 成功返回写入的字节数，失败返回-1
 */
hy_s32_t HYSocketWrite(void *handle, const char *buf, hy_u32_t len);

/**
 * @brief 连接服务器
 *
 * @param handle 句柄
 * @param ip 服务器ip地址
 * @param port 端口
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HySocketConnect(void *handle, const char *ip, hy_u16_t port);

/**
 * @brief 服务器等待连接
 *
 * @param handle 句柄
 * @param port 端口
 * @param new_client_cb 新连接回调
 * @param args 上层传递参数
 *
 * @return 成功返回0，失败返回-1
 */
hy_s32_t HySocketWaitConnect(void *handle, hy_u16_t port,
        HySocketNewClientCb_t new_client_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif

