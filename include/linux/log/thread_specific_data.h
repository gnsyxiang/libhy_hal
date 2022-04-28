/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    thread_specific_data.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/04 2022 10:38
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/04 2022      create the file
 * 
 *     last modified: 28/04 2022 10:38
 */
#ifndef __LIBHY_HAL_INCLUDE_THREAD_SPECIFIC_DATA_H_
#define __LIBHY_HAL_INCLUDE_THREAD_SPECIFIC_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#include "hy_type.h"

typedef void * (*thread_specific_data_create_cb_t)(void);
typedef void (*thread_specific_data_destroy_cb_t)(void *handle);

typedef void (*thread_specific_data_reset_cb_t)(void *handle);

typedef struct {
    pthread_key_t                       thread_key;

    thread_specific_data_create_cb_t    create_cb;
    thread_specific_data_destroy_cb_t   destroy_cb;
    thread_specific_data_reset_cb_t     reset_cb;
} thread_specific_data_s;

hy_s32_t thread_specific_data_create(
        thread_specific_data_create_cb_t create_cb,
        thread_specific_data_destroy_cb_t destroy_cb,
        thread_specific_data_reset_cb_t reset_cb);
void thread_specific_data_destroy(void);

void *thread_specific_data_fetch(void);

#ifdef __cplusplus
}
#endif

#endif

