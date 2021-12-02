/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_log.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    29/10 2021 20:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        29/10 2021      create the file
 * 
 *     last modified: 29/10 2021 20:29
 */
#ifndef __LIBHY_UTILS_INCLUDE_HY_LOG_H_
#define __LIBHY_UTILS_INCLUDE_HY_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

/**
 * @brief 打印等级定义
 *
 * @note 数字越小越紧急
 */
typedef enum {
    HY_LOG_LEVEL_FATAL,             ///< 致命错误，立刻停止程序
    HY_LOG_LEVEL_ERROR,             ///< 错误，停止程序
    HY_LOG_LEVEL_WARN,              ///< 警告
    HY_LOG_LEVEL_INFO,              ///< 追踪，记录程序运行到哪里
    HY_LOG_LEVEL_DEBUG,             ///< 调试程序相关打印
    HY_LOG_LEVEL_TRACE,             ///< 程序打点调试

    HY_LOG_LEVEL_MAX
} HyLogLevel_t;

/**
 * @brief 模块配置参数
 */
typedef struct {
    int32_t level;                  ///< 打印等级，详见HyLogLevel_t
    size_t  buf_len;                ///< 打印buf长度
    int32_t color_output;           ///< 是否颜色输出
} HyLogSaveConfig_t;

/**
 * @brief 模块配置参数
 */
typedef struct {
    HyLogSaveConfig_t save_config;  ///< 参数，详见HyLogSaveConfig_t
} HyLogConfig_t;

/**
 * @brief 创建log模块
 *
 * @param config 配置参数，详见HyLogConfig_t
 *
 * @return 模块句柄
 */
void *HyLogCreate(HyLogConfig_t *config);

/**
 * @brief 销毁log模块
 *
 * @param handle 模块句柄的地址
 */
void HyLogDestroy(void **handle);

#if 1
void HyLogWrite(int32_t level, const char *file,  const char *func,
        uint32_t line, char *fmt, ...);

void HyLogHex(const char *name, uint32_t line,
       void *buf, size_t len, int8_t flag);

/**
 * @brief 输出log宏转义
 *
 * note: 宏为内部用，最好不要在外面使用
 */
#define LOG(no_debug, level, fmt, ...)                                  \
    do {                                                                \
        if (no_debug) {                                                 \
            HyLogWrite(level, __FILE__, __func__, __LINE__,             \
                    (char *)fmt, ##__VA_ARGS__);                        \
        }                                                               \
    } while (0)
#endif

/**
 * @brief 输出对应的log等级函数
 */
#define LOGF(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_WARN,  fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_INFO,  fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...) LOG(ALONE_DEBUG, HY_LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#define LOG_HEX_ASCII(buf, len) HyLogHex(__func__, __LINE__, buf, len, 1)
#define LOG_HEX(buf, len)       HyLogHex(__func__, __LINE__, buf, len, 0)

#ifdef __cplusplus
}
#endif

#endif

