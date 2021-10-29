/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_dlopen.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    26/10 2021 09:09
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        26/10 2021      create the file
 * 
 *     last modified: 26/10 2021 09:09
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_DLOPEN_H_
#define __LIBHY_HAL_INCLUDE_HY_DLOPEN_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 打开动态库
 *
 * @param so_name 动态库的名字
 *
 * @return 成功返回操作句柄，失败返回NULL
 */
void *HyDlLibOpen(const char *so_name);

/**
 * @brief 关闭动态库
 *
 * @param handle 操作句柄的地址
 */
void HyDlLibClose(void **handle);

/**
 * @brief 加载动态库的特定函数
 *
 * @param handle 操作句柄
 * @param symbol 函数名
 *
 * @return 成功返回函数指针，失败返回NULL
 */
void *HyDlLibLoadSymbol(void *handle, const char *symbol);

#ifdef __cplusplus
}
#endif

#endif

