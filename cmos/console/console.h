/********************************************************************************
*
* 文件名  ： console.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： cmos 控制台模块
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_CONSOLE_H_
#define _CMOS_CONSOLE_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/
#define  cmos_err_log(...)  cmos_printf_poll("ERR: ") ;\
                            cmos_printf_poll(__VA_ARGS__);\
                            cmos_printf_poll("\n");

/* CMOS_DEBUG_LEVEL > 0 输出基础打印信息 */
#if (CMOS_DEBUG_LEVEL > 0)
#define cmos_debug_log(...) cmos_printf_poll("DEBUG : ") ;\
                            cmos_printf_poll(__VA_ARGS__);\
                            cmos_printf_poll("\n");
#else
#define cmos_debug_log(...)                         
#endif

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_int32_T console_init(cmos_int32_T baud_rate);
cmos_int32_T printf_poll(char *fmt, ...);

#endif /* _CMOS_HAL_UART_H_ */

