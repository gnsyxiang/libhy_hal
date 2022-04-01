/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 09:07
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 09:07
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_UART_H_
#define __LIBHY_HAL_INCLUDE_HY_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "hy_hal/hy_type.h"

#define HY_UART_DEV_PATH_LEN_MAX   (32)

// @FIXME: open串口为阻塞情况下，这两个参数适配应用层协议
#define UART_READ_VMIN_LEN      (0)
#define UART_READ_VTIME_100MS   (1)

/**
 * @brief 串口波特率
 */
typedef enum {
    HY_UART_SPEED_4800,                     ///< 4800
    HY_UART_SPEED_9600,                     ///< 9600
    HY_UART_SPEED_115200,                   ///< 115200

    HY_UART_SPEED_MAX,
} HyUartSpeed_t;

/**
 * @brief 流控类型
 */
typedef enum {
    HY_UART_FLOW_CONTROL_NONE,              ///< 无
    HY_UART_FLOW_CONTROL_SOFT,              ///< 软件流控
    HY_UART_FLOW_CONTROL_HARDWARE,          ///< 硬件流控

    HY_UART_FLOW_CONTROL_MAX,
} HyUartFlowControl_t;

/**
 * @brief 数据宽度
 */
typedef enum {
    HY_UART_DATA_BIT_5,                     ///< 5位
    HY_UART_DATA_BIT_6,                     ///< 6位
    HY_UART_DATA_BIT_7,                     ///< 7位
    HY_UART_DATA_BIT_8,                     ///< 8位

    HY_UART_DATA_BIT_MAX,
} HyUartDataBit_t;

/**
 * @brief 奇偶校验
 */
typedef enum {
    HY_UART_PARITY_NONE,                    ///< 无校验
    HY_UART_PARITY_ODD,                     ///< 奇校验
    HY_UART_PARITY_EVEN,                    ///< 偶校验

    HY_UART_PARITY_MAX,
} HyUartParityType_t;

/**
 * @brief 停止位个数
 */
typedef enum {
    HY_UART_STOP_BIT_1,                     ///< 一个停止位
    HY_UART_STOP_BIT_2,                     ///< 两个停止位

    HY_UART_STOP_BIT_MAX,
} HyUartStopBit_t;

/**
 * @brief 配置参数
 */
typedef struct {
    HyUartSpeed_t           speed;          ///< 波特率
    HyUartFlowControl_t     flow_control;   ///< 流控

    HyUartDataBit_t         data_bit;       ///< 数据宽度
    HyUartParityType_t      parity_type;    ///< 奇偶校验
    HyUartStopBit_t         stop_bit;       ///< 停止位个数
} HyUartSaveConfig_t;

/**
 * @brief 配置参数
 */
typedef struct {
    HyUartSaveConfig_t      save_c;    ///< 配置参数

    char                    dev_path[HY_UART_DEV_PATH_LEN_MAX];  ///< 设备节点地址
} HyUartConfig_t;

/**
 * @brief 创建模块
 *
 * @param uart_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyUartCreate(HyUartConfig_t *uart_c);

/**
 * @brief 销毁模块
 *
 * @param handle 句柄的地址（二级指针）
 */
void HyUartDestroy(void **handle);

/**
 * @brief 写入数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 成功返回写入的字节数，失败返回-1
 */
hy_s32_t HyUartWrite(void *handle, const void *buf, hy_u32_t len);

/**
 * @brief 读取数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 跟read返回值一样，需要根据返回值判断
 */
hy_s32_t HyUartRead(void *handle, void *buf, hy_u32_t len);

/**
 * @brief 读取数据
 *
 * @param handle 句柄
 * @param buf 数据地址
 * @param len 数据长度
 *
 * @return 成功返回读到的字节数，失败返回-1
 */
hy_s32_t HyUartReadN(void *handle, void *buf, hy_u32_t len);

#ifdef __cplusplus
}
#endif

#endif

