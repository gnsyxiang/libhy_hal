/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_assert.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/10 2021 08:53
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/10 2021      create the file
 * 
 *     last modified: 26/10 2021 08:53
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_ASSERT_H_
#define __LIBHY_HAL_INCLUDE_HY_ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#define HY_ASSERT(zero) assert(zero);

#ifdef NDEBUG
#define HY_ASSERT_BREAK(val)
#define HY_ASSERT_RET(val)
#define HY_ASSERT_RET_VAL(val, ret)
#else
#define HY_ASSERT_BREAK(val)            \
    if (val) {                          \
        LOGE("the param is error \n");  \
        break;                          \
    }

#define HY_ASSERT_RET(val)              \
    if (val) {                          \
        LOGE("the param is error \n");  \
        return ;                        \
    }

#define HY_ASSERT_RET_VAL(val, ret)     \
    if (val) {                          \
        LOGE("the param is error \n");  \
        return ret;                     \
    }
#endif

#ifdef __cplusplus
}
#endif

#endif

