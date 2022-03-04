/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_pipe.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    03/03 2022 19:04
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        03/03 2022      create the file
 * 
 *     last modified: 03/03 2022 19:04
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_PIPE_H_
#define __LIBHY_HAL_INCLUDE_HY_PIPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief pipe阻塞状态
 */
typedef enum {
    HY_PIPE_BLOCK_STATE_BLOCK,              ///< 阻塞
    HY_PIPE_BLOCK_STATE_NOBLOCK,            ///< 非阻塞
} HyPipeBlockState_e;

/**
 * @brief 配置参数
 */
typedef struct {
    HyPipeBlockState_e  read_block_state;   ///< 读端阻塞状态
} HyPipeSaveConfig_s;

/**
 * @brief 配置参数
 */
typedef struct {
    HyPipeSaveConfig_s save_c;              ///< 配置参数
} HyPipeConfig_s;

/**
 * @brief 创建pipe
 *
 * @param pipe_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 */
void *HyPipeCreate(HyPipeConfig_s *pipe_c);

/**
 * @brief 销毁pipe
 *
 * @param pipe_h 句柄的地址(二级指针)
 */
void HyPipeDestroy(void **pipe_h);

/**
 * @brief 向pipe中读取数据
 *
 * @param pipe_h 句柄
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return
 *      (>  0)返回读取到的字节数
 *      (=  0)读到文件末尾
 *      (= -1)读取出错
 */
int32_t HyPipeRead(void *pipe_h, void *buf, int32_t len);

/**
 * @brief 向pipe中写入数据
 *
 * @param pipe_h 句柄
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 成功返回写入的字节数，失败返回-1
 */
int32_t HyPipeWrite(void *pipe_h, const void *buf, int32_t len);

/**
 * @brief 创建pipe宏
 *
 * @param _read_block_state 读端阻塞状态
 *
 * @return 成功返回句柄，失败返回NULL
 */
#define HyPipeCreate_m(_read_block_state)                       \
    ({                                                          \
        HyPipeConfig_s pipe_c;                                  \
        HY_MEMSET(&pipe_c, sizeof(pipe_c));                     \
        pipe_c.save_c.read_block_state = _read_block_state;     \
        HyPipeCreate(&pipe_c);                                  \
     })

#ifdef __cplusplus
}
#endif

#endif

