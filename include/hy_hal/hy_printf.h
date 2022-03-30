/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_printf.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/01 2022 11:28
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/01 2022      create the file
 * 
 *     last modified: 08/01 2022 11:28
 */
#ifndef __LIBHY_HAL_INCLUDE_HY_PRINTF_H_
#define __LIBHY_HAL_INCLUDE_HY_PRINTF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*设置输出前景色*/
#define PRINT_FONT_BLA      "\033[30m"      //黑色
#define PRINT_FONT_RED      "\033[31m"      //红色
#define PRINT_FONT_GRE      "\033[32m"      //绿色
#define PRINT_FONT_YEL      "\033[33m"      //黄色
#define PRINT_FONT_BLU      "\033[34m"      //蓝色
#define PRINT_FONT_PUR      "\033[35m"      //紫色
#define PRINT_FONT_CYA      "\033[36m"      //青色
#define PRINT_FONT_WHI      "\033[37m"      //白色

/*设置输出背景色*/ 
#define PRINT_BACK_BLA      "\033[40m"      //黑色
#define PRINT_BACK_RED      "\033[41m"      //红色
#define PRINT_BACK_GRE      "\033[42m"      //绿色
#define PRINT_BACK_YEL      "\033[43m"      //黄色
#define PRINT_BACK_BLU      "\033[44m"      //蓝色
#define PRINT_BACK_PUR      "\033[45m"      //紫色
#define PRINT_BACK_CYA      "\033[46m"      //青色
#define PRINT_BACK_WHI      "\033[47m"      //白色

/*输出属性设置*/
#define PRINT_ATTR_RESET    "\033[0m"       //重新设置属性到缺省设置 
#define PRINT_ATTR_BOL      "\033[1m"       //设置粗体 
#define PRINT_ATTR_LIG      "\033[2m"       //设置一半亮度(模拟彩色显示器的颜色) 
#define PRINT_ATTR_LIN      "\033[4m"       //设置下划线(模拟彩色显示器的颜色) 
#define PRINT_ATTR_GLI      "\033[5m"       //设置闪烁 
#define PRINT_ATTR_REV      "\033[7m"       //设置反向图象 
#define PRINT_ATTR_THI      "\033[22m"      //设置一般密度 
#define PRINT_ATTR_ULIN     "\033[24m"      //关闭下划线 
#define PRINT_ATTR_UGLI     "\033[25m"      //关闭闪烁 
#define PRINT_ATTR_UREV     "\033[27m"      //关闭反向图象

// printf("\033[字背景颜色;字体颜色m字符串\033[0m" );

#ifdef __cplusplus
}
#endif

#endif
