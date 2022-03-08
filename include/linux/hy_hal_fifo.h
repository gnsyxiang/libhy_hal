/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_hal_fifo.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/03 2022 19:54
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/03 2022      create the file
 * 
 *     last modified: 07/03 2022 19:54
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_HAL_FIFO_H_
#define __LIBHY_HAL_INCLUDE_HY_HAL_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief fifo模式
 *
 * @note 阻塞模式下，open时就阻塞
 */
typedef enum {
    HY_HAL_FIFO_FLAG_READ,              ///< 读
    HY_HAL_FIFO_FLAG_WRITE,             ///< 写
    HY_HAL_FIFO_FLAG_NOBLOCK_READ,      ///< 非阻塞读
    HY_HAL_FIFO_FLAG_NOBLOCK_WRITE,     ///< 非阻塞写
} HyHalFifoFlag_e;

/**
 * @brief 配置参数
 */
typedef struct {
    char                *name;  ///< fifo名字
    uint32_t            mode;   ///< fifo权限，为0的话则设置默认0777
    HyHalFifoFlag_e     flag;   ///< fifo读写模式
} HyHalFifoConfig_s;

/**
 * @brief 创建fifo
 *
 * @param fifo_c 配置参数
 *
 * @return 成功返回句柄，失败返回NULL
 *
 * @note 阻塞模式下，该函数不会返回
 */
void *HyHalFifoCreate(HyHalFifoConfig_s *fifo_c);

/**
 * @brief 销毁fifo
 *
 * @param pipe_h 句柄的地址(二级指针)
 */
void HyHalFifoDestroy(void **fifo_h);

/**
 * @brief 向fifo中读取数据
 *
 * @param fifo_h 句柄
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 
 *      (>  0)返回读取到的字节数
 *      (=  0)读到文件末尾
 *      (= -1)读取出错
 */
int32_t HyHalFifoRead(void *fifo_h, void *buf, int32_t len);

/**
 * @brief 向fifo中写入数据
 *
 * @param fifo_h 句柄
 * @param buf 数据的地址
 * @param len 数据的长度
 *
 * @return 成功返回写入的字节数，失败返回-1
 */
int32_t HyHalFifoWrite(void *fifo_h, const void *buf, int32_t len);

/**
 * @brief 创建fifo
 *
 * @param _name fifo名字
 * @param _flag fifo读写模式
 *
 * @return 成功返回句柄，失败返回NULL
 */
#define HyHalFifoCreate_m(_name, _flag)         \
    ({                                          \
        HyHalFifoConfig_s fifo_c;               \
        HY_MEMSET(&fifo_c, sizeof(fifo_c));     \
        fifo_c.flag = _flag;                    \
        fifo_c.name = _name;                    \
        HyHalFifoCreate(&fifo_c);               \
     })

#ifdef __cplusplus
}
#endif

#endif

