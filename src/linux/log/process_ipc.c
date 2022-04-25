/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    process_ipc.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/04 2022 17:27
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/04 2022      create the file
 * 
 *     last modified: 25/04 2022 17:27
 */
#include <stdio.h>

#include "fifo.h"

#include "process_ipc.h"

typedef struct {
    hy_s32_t            is_exit;
    pthread_t           thread;

    fifo_context_s      *fifo;
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
} _process_ipc_context_s;

void process_ipc_server_destroy(void)
{

}

hy_s32_t process_ipc_server_create(hy_u32_t fifo_len)
{

    return -1;
}

