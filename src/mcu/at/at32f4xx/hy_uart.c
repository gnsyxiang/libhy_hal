/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_uart.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/11 2021 19:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/11 2021      create the file
 * 
 *     last modified: 08/11 2021 19:16
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hy_uart.h"
#include "hy_gpio.h"
#include "uart_2_str.h"

#include "at32f4xx_gpio.h"
#include "at32f4xx_usart.h"
#include "at32f4xx_rcc.h"
#include "misc.h"
#include "at32f4xx.h"

#include "hy_mem.h"
#include "hy_type.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

#define _DEFINE_UART()                              \
    USART_Type *uart[HY_UART_NUM_MAX] = {           \
        NULL,                                       \
        USART1, USART2, USART3, UART4,              \
        UART5, USART6, UART7, UART8                 \
    }

typedef struct {
    HyUartConfigSave_t config_save;

    HyUartNum_t num;
} _uart_context_t;

static _uart_context_t *context_array[HY_UART_NUM_MAX] = {NULL};

static hy_s32_t _uart_send_byte(void *handle, char byte);

#ifdef DEBUG_UART
#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    /*
     * write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
     * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2) {
        return -1;
    }

    int i = 0;
    while (*ptr && (i < len)) {
        if (*ptr == '\n') {
            _uart_send_byte(context_array[DEBUG_UART_NUM], '\r');
        }
        _uart_send_byte(context_array[DEBUG_UART_NUM], *ptr);
        i++;
        ptr++;
    }
    return i;
}
#endif
#if __CC_ARM
int fputc(int ch, FILE *f)
{
    if ((hy_u8_t)ch == '\n') {
        _uart_send_byte(context_array[DEBUG_UART_NUM], '\r');
    }
    _uart_send_byte(context_array[DEBUG_UART_NUM], (hy_u8_t)ch);

    return 1;
}
#endif
#endif

static hy_s32_t _uart_send_byte(void *handle, char byte)
{
    HY_ASSERT_VAL_RET_VAL(!handle, -1);

    _DEFINE_UART();

    _uart_context_t *context = handle;
    FlagStatus status = RESET;
    hy_u32_t cnt = 0;

    USART_SendData(uart[context->num], byte);
    do {
        status = USART_GetFlagStatus(uart[context->num], USART_FLAG_TRAC);
        cnt++;
    } while((status == RESET) && (cnt != 0x1CE2));

    if (cnt == 0x1CE2)
        return (EOF);

    return (byte);
}

int32_t HyUartWrite(void *handle, void *buf, size_t len)
{
    HY_ASSERT_VAL_RET_VAL(!handle || !buf, -1);

    _uart_context_t *context = handle;
    _DEFINE_UART();

    FlagStatus status = RESET;
    hy_u32_t cnt = 0;
    uint16_t i = 0;
    char *str = buf;

    do {
        cnt = 0;

        USART_SendData(uart[context->num], str[i]);
        do {
            status = USART_GetFlagStatus(uart[context->num], USART_FLAG_TRAC);
            cnt++;
        } while((status == RESET) && (cnt != 0x1CE2));

        if (cnt == 0x1CE2) {
            break;
        }
        i++;
    } while (i < len);

    return (i);
}

int32_t HyUartProcess(void *handle)
{
    return 0;
}

static void _uart_irq_handler(HyUartNum_t num)
{
    if (context_array[num]) {
        _DEFINE_UART();

        if (USART_GetITStatus(uart[num], USART_INT_RDNE) != RESET) {

            HyUartConfigSave_t *config_save = &context_array[num]->config_save;
            if (config_save->read_cb) {
                hy_u8_t ch = USART_ReceiveData(uart[num]);
                config_save->read_cb(&ch, 1, config_save->args);
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    _uart_irq_handler(HY_UART_NUM_1);
}

void UART4_IRQHandler(void)
{
    _uart_irq_handler(HY_UART_NUM_4);
}

void UART5_IRQHandler(void)
{
    _uart_irq_handler(HY_UART_NUM_5);
}

static void _init_uart_gpio(HyUartNum_t num)
{
    HyGpio_t gpio[][2] = {
        {{0, 0}, {0, 0}},
        {{HY_GPIO_GROUP_PA, HY_GPIO_PIN_9}, {HY_GPIO_GROUP_PA, HY_GPIO_PIN_10}},
    };

    HyGpioSetOutput(&gpio[num][0], HY_GPIO_LEVEL_HIGH);
    HyGpioSetInput(&gpio[num][1]);
}

static void _init_uart_func(HyUartConfig_t *config)
{
    _DEFINE_UART();

    hy_u32_t rate[HY_UART_RATE_MAX] = {
        1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200,
    };

    hy_u16_t bits[] = {0, 0, 0, USART_WordLength_8b, USART_WordLength_9b};
    hy_u16_t parity[] = {USART_Parity_No, USART_Parity_Odd, USART_Parity_Even};
    hy_u16_t stop[] = {
        USART_StopBits_0_5, USART_StopBits_1,
        USART_StopBits_1_5, USART_StopBits_2
    };
    hy_u16_t flow_control[] = {
        USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS,
        USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS
    };

    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);  

    USART_InitType uart_init;
    USART_StructInit(&uart_init);

    uart_init.USART_BaudRate              = rate[config->rate];
    uart_init.USART_WordLength            = bits[config->bits];
    uart_init.USART_Parity                = parity[config->parity];
    uart_init.USART_StopBits              = stop[config->stop];
    uart_init.USART_HardwareFlowControl   = flow_control[config->flow_control];
    uart_init.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;	

    USART_Init(uart[config->config_save.num], &uart_init); 

    NVIC_InitType NVIC_InitStructure;
    hy_u16_t irq[HY_UART_NUM_MAX] = {
        0,
        USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn,
        UART5_IRQn, USART6_IRQn, UART7_IRQn, UART8_IRQn
    };

    NVIC_InitStructure.NVIC_IRQChannel = irq[config->config_save.num];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    USART_INTConfig(uart[config->config_save.num], USART_INT_RDNE, ENABLE);

    USART_Cmd(uart[config->config_save.num], ENABLE);
}

void HyUartDestroy(void **handle)
{
    HY_ASSERT_VAL_RET(!handle || !*handle);

    _uart_context_t *context = *handle;

    context_array[context->num] = NULL;

    LOGI("uart destroy successful, context: %p \n", context);
    HY_MEM_FREE_PP(handle);
}

void *HyUartCreate(HyUartConfig_t *config)
{
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    _uart_context_t *context = NULL;

    do {
        context = HY_MEM_MALLOC_BREAK(_uart_context_t *, sizeof(*context));
        HY_MEMCPY(&context->config_save,
                &config->config_save, sizeof(config->config_save));

        context_array[config->config_save.num] = context;

        _init_uart_gpio(config->config_save.num);
        _init_uart_func(config);

        LOGI("uart %s create successful, context: %p \n",
                HY_UART_NUM_2_STR(config->config_save.num), context);
        return context;
    } while (0);

    HyUartDestroy((void **)&context);
    return NULL;
}
