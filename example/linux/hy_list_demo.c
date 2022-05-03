/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_list_demo.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    30/10 2021 10:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        30/10 2021      create the file
 * 
 *     last modified: 30/10 2021 10:43
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_mem.h"
#include "hy_string.h"
#include "hy_type.h"
#include "hy_hal_utils.h"
#include "hy_log.h"

#include "hy_list.h"

typedef struct {
    char                    name[HY_STRING_BUF_MAX_LEN_32];
    hy_s32_t                id;

    struct hy_list_head     entry;
} _student_t;

typedef struct {
    struct hy_list_head     list;
    hy_u32_t                list_cnt;
} _main_context_s;

static void _dump_list(struct hy_list_head *list, const char *tag)
{
    LOGD("%s: \n", tag);

    _student_t *pos;
    hy_list_for_each_entry(pos, list, entry) {
        LOGI("name: %s, id: %d \n", pos->name, pos->id);
    }
}

static hy_u32_t _get_partition(struct hy_list_head *list, hy_s32_t low, hy_s32_t high)
{
    hy_s32_t tmp_id = 0;
    hy_s32_t pivot_data = HY_LIST_GET_NODE_DATA(list, low, _student_t, id);

    while (low < high) {
        tmp_id = HY_LIST_GET_NODE_DATA(list, high, _student_t, id);
        while (low < high && tmp_id >= pivot_data) {
            high--;
            tmp_id = HY_LIST_GET_NODE_DATA(list, high, _student_t, id);
        }

        if (low != high) {
            list_swap_node_ptr(list, low, high);
        }

        tmp_id = HY_LIST_GET_NODE_DATA(list, low, _student_t, id);
        while (low < high && tmp_id <= pivot_data) {
            low++;
            tmp_id = HY_LIST_GET_NODE_DATA(list, low, _student_t, id);
        }

        if (low != high) {
            list_swap_node_ptr(list, high, low);
        }
    }

    return low;
}

static void _sort(struct hy_list_head *list, hy_s32_t low, hy_s32_t high)
{
    if (low < high) {
        hy_s32_t pivot_point = _get_partition(list, low, high);

        _sort(list, low, pivot_point - 1);
        _sort(list, pivot_point + 1, high);
    }
}

static void _quick_sort(_main_context_s *context)
{
    _dump_list(&context->list, "before");
    _sort(&context->list, 0, context->list_cnt);
    _dump_list(&context->list, "after");
}

int main(int argc, char *argv[])
{
    _main_context_s *context = NULL;

    context = HY_MEM_MALLOC_RET_VAL(_main_context_s *, sizeof(*context), -1);

    HyLogInit_m(10 * 1024, HY_LOG_MODE_PROCESS_SINGLE, HY_LOG_LEVEL_TRACE, HY_LOG_OUTFORMAT_ALL);

    LOGI("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    // 初始化
    // INIT_LIST_HEAD
    // HY_LIST_HEAD

    // 曾
    // list_add
    // list_add_tail

    // 删
    // list_del

    // 改

    // 查
    // list_for_each
    // list_for_each_prev
    // list_for_each_safe
    // list_for_each_prev_safe
    // list_for_each_entry
    // list_for_each_entry_safe

    HY_INIT_LIST_HEAD(&context->list);

    hy_s32_t i;
    #define STUDENT_CNT (5)
    _student_t student[STUDENT_CNT];
    hy_s32_t id[STUDENT_CNT] = {5,4,3,2,1};
    for (i = 0; i < STUDENT_CNT; ++i) {
        _student_t *st = &student[i];

        HY_MEMSET(st->name, HY_STRING_BUF_MAX_LEN_32);
        snprintf(st->name, HY_STRING_BUF_MAX_LEN_32, "student%d", i);
        student[i].id = id[i];

        context->list_cnt++;
        hy_list_add_tail(&st->entry, &context->list);
    }

    _quick_sort(context);

    _student_t *pos, *n;
    hy_list_for_each_entry_safe(pos, n, &context->list, entry) {
        LOGI("name: %s, id: %d \n", pos->name, pos->id);

        hy_list_del(&pos->entry);
    }

    HyLogDeInit();

    HY_MEM_FREE_PP(&context);

    return 0;
}

