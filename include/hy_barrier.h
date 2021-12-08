/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_barrier.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/12 2021 14:08
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/12 2021      create the file
 * 
 *     last modified: 08/12 2021 14:08
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_BARRIER_H_
#define __LIBHY_HAL_INCLUDE_HY_BARRIER_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#else
    #ifdef __GNUC__
        #if __GNUC__ >= 4 || defined(__arm__)
            /* Optimization barrier */
            /* The "volatile" is due to gcc bugs */
            #ifndef HY_BARRIER
            #define HY_BARRIER()   __asm__ __volatile__("": : :"memory")    // volatile: 告诉编译barrier()周围的指令不要被优化；
                                                                            // memory: 告诉编译器汇编代码会使内存里面的值更改，编译器应使用内存里的新值而非寄存器里保存的老值。
            #endif

            #ifndef HY_SMP_MB
            #define HY_SMP_MB()    HY_BARRIER()
            #endif

            #ifndef HY_SMP_WMB
            #define HY_SMP_WMB()   HY_BARRIER()
            #endif

            #ifndef HY_SMP_RMB
            #define HY_SMP_RMB()   HY_BARRIER()
            #endif
        #else
            #error "gcc version too low"
        #endif
    #endif
#endif

#ifdef __cplusplus
}
#endif

#endif

