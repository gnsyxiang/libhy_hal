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
#include <stdint.h>

#define HY_UART_DEV_PATH_NAME_LEN       (32)

// @FIXME: open串口为阻塞情况下，这两个参数适配应用层协议
#define UART_READ_VMIN_LEN      (0)
#define UART_READ_VTIME_100MS   (1)

typedef enum {
    HY_UART_SPEED_4800,
    HY_UART_SPEED_9600,
    HY_UART_SPEED_115200,

    HY_UART_SPEED_MAX,
} HyUartSpeed_t;

typedef enum {
    HY_UART_FLOW_CONTROL_NONE,
    HY_UART_FLOW_CONTROL_SOFT,
    HY_UART_FLOW_CONTROL_HARDWARE,

    HY_UART_FLOW_CONTROL_MAX,
} HyUartFlowControl_t;

typedef enum {
    HY_UART_DATA_BIT_5,
    HY_UART_DATA_BIT_6,
    HY_UART_DATA_BIT_7,
    HY_UART_DATA_BIT_8,

    HY_UART_DATA_BIT_MAX,
} HyUartDataBit_t;

typedef enum {
    HY_UART_PARITY_NONE,
    HY_UART_PARITY_ODD,
    HY_UART_PARITY_EVEN,

    HY_UART_PARITY_MAX,
} HyUartParityType_t;

typedef enum {
    HY_UART_STOP_BIT_1,
    HY_UART_STOP_BIT_2,

    HY_UART_STOP_BIT_MAX,
} HyUartStopBit_t;

typedef struct {
    HyUartSpeed_t           speed;
    HyUartFlowControl_t     flow_control;

    HyUartDataBit_t         data_bit;
    HyUartParityType_t      parity_type;
    HyUartStopBit_t         stop_bit;
} HyUartSaveConfig_t;

typedef struct {
    HyUartSaveConfig_t      save_config;

    char                    dev_path_name[HY_UART_DEV_PATH_NAME_LEN];
} HyUartConfig_t;

void *HyUartCreate(HyUartConfig_t *config);
void HyUartDestroy(void **handle);

ssize_t HyUartWrite(void *handle, const void *buf, size_t len);
ssize_t HyUartRead(void *handle, void *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif

