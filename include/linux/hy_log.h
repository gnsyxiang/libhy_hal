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
#ifndef __LIBHY_HAL_INCLUDE_HY_LOG_H_
#define __LIBHY_HAL_INCLUDE_HY_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>

#if 1
#define HY_CHECK_FMT_WITH_PRINTF(a, b) __attribute__((format(printf, a, b)))
#else
#define HY_CHECK_FMT_WITH_PRINTF(a, b)
#endif

#define HY_STRRCHR_FILE (strrchr(__FILE__, '/'))
#define HY_FILENAME     (HY_STRRCHR_FILE ? (HY_STRRCHR_FILE + 1) : __FILE__)

/**
 * @brief 打印等级定义
 *
 * @note 数字越小越紧急
 */
typedef enum {
    HY_LOG_LEVEL_FATAL,                     ///< 致命错误，立刻停止程序
    HY_LOG_LEVEL_ERROR,                     ///< 错误，停止程序
    HY_LOG_LEVEL_WARN,                      ///< 警告
    HY_LOG_LEVEL_INFO,                      ///< 追踪，记录程序运行到哪里
    HY_LOG_LEVEL_DEBUG,                     ///< 调试程序相关打印
    HY_LOG_LEVEL_TRACE,                     ///< 程序打点调试

    HY_LOG_LEVEL_MAX
} HyLogLevel_e;

/**
 * @brief 配置参数
 */
typedef struct {
    uint32_t            buf_len_min;        ///< 单条日志的最短长度
    uint32_t            buf_len_max;        ///< 单条日志的最大长度，超过该长度会被截断

    HyLogLevel_e        level:4;            ///< 打印等级
    int32_t             color_enable:1;     ///< 是否颜色输出
    int32_t             reserved;           ///< 预留
} HyLogSaveConfig_s;

/**
 * @brief 配置参数
 */
typedef struct {
    HyLogSaveConfig_s   save_config;        ///< 配置参数
} HyLogConfig_s;

/**
 * @brief 创建log模块
 *
 * @param log_c配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyLogCreate(HyLogConfig_s *log_c);

/**
 * @brief 销毁log模块
 *
 * @param handle 模块句柄的地址
 */
void HyLogDestroy(void **handle);

/**
 * @brief log函数
 *
 * @param level 打印等级
 * @param file 所在的文件
 * @param func 所在的函数
 * @param line 所在的行号
 * @param fmt 格式
 * @param ... 参数
 */
void HyLogWrite(HyLogLevel_e level, const char *err_str,
        const char *file, uint32_t line, pthread_t tid, long pid,
        char *fmt, ...) HY_CHECK_FMT_WITH_PRINTF(7, 8);

#define LOG(level, err_str, fmt, ...) \
    HyLogWrite(level, err_str, HY_FILENAME, __LINE__, \
            pthread_self(), syscall(SYS_gettid), fmt, ##__VA_ARGS__)

#define LOGF(fmt, ...)  LOG(HY_LOG_LEVEL_FATAL, strerror(errno), fmt, ##__VA_ARGS__)
#define LOGES(fmt, ...) LOG(HY_LOG_LEVEL_ERROR, strerror(errno), fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...)  LOG(HY_LOG_LEVEL_ERROR, NULL,            fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...)  LOG(HY_LOG_LEVEL_WARN,  NULL,            fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...)  LOG(HY_LOG_LEVEL_INFO,  NULL,            fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...)  LOG(HY_LOG_LEVEL_DEBUG, NULL,            fmt, ##__VA_ARGS__)
#define LOGT(fmt, ...)  LOG(HY_LOG_LEVEL_TRACE, NULL,            fmt, ##__VA_ARGS__)

/**
 * @brief 创建log模块
 *
 * @param _buf_len_min 单条日志的最短长度
 * @param _buf_len_max 单条日志的最大长度，超过该长度会被截断
 * @param _level 打印等级
 * @param _color_enable 是否颜色输出
 *
 * @return 成功返回句柄，失败返回NULL
 */
#define HyLogCreate_m(_buf_len_min, _buf_len_max, _level, _color_enable)    \
    ({                                                                      \
        HyLogConfig_s __config;                                             \
        __config.save_config.buf_len_min  = _buf_len_min;                   \
        __config.save_config.buf_len_max  = _buf_len_max;                   \
        __config.save_config.level        = _level;                         \
        __config.save_config.color_enable = _color_enable;                  \
        HyLogCreate(&__config);                                             \
     })

/**
 * @brief 打印二进制信息
 *
 * @param name 函数名
 * @param line 行号
 * @param buf 被打印数据
 * @param len 被打印数据的长度
 * @param flag 是否需要打印对应的ascii
 */
void HyLogHex(const char *name, uint32_t line,
       const void *buf, uint32_t len, int32_t flag);

#define HY_LOG_HEX_ASCII(buf, len) HyLogHex(__func__, __LINE__, buf, len, 1)
#define HY_LOG_HEX(buf, len)       HyLogHex(__func__, __LINE__, buf, len, 0)

#ifdef __cplusplus
}
#endif

#endif
