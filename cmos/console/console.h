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
#define  cmos_err_log(...)  console_printf("ERR: ") ;\
                            console_printf(__VA_ARGS__);\
                            console_printf("\r\n");

/* CMOS_DEBUG_LEVEL > 0 输出基础打印信息 */
#if (CMOS_DEBUG_LEVEL > 0)
#define cmos_debug_log(...) console_printf(__VA_ARGS__);                            
#else
#define cmos_debug_log(...)
#endif

/* CMOS_DEBUG_LEVEL > 1 输出跟踪信息 */
#if (CMOS_DEBUG_LEVEL > 1)
#define cmos_trace_log(...) console_printf("TRACE : ") ;\
                            console_printf(__VA_ARGS__);\
                            console_printf("\r\n");
#else
#define cmos_trace_log(...)
#endif


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_console_init(cmos_int32_T baud_rate);
cmos_int32_T cmos_console_printf(char *fmt, ...);

#endif /* _CMOS_HAL_UART_H_ */
