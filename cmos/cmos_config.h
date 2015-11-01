/********************************************************************************
*
* 文件名  ： cmos_config.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： cmos配置
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_CONFIG_H_
#define _CMOS_CONFIG_H_

/************************************ 头文件 ***********************************/
#include "cmos_typedef.h"

/************************************ 宏定义 ***********************************/
/********************************** 控制台配置 *********************************/
/* 调试级别 */
/* 0: 输出cmos_err_log */
/* 1: 输出cmos_debug_log */
/* 2: 输出cmos_trace_log */
#define CMOS_DEBUG_LEVEL                                        (2)

/* printf缓存 */
#define CMOS_PRINTF_BUF_SIZE                                    (1024)

/* 调试串口 */
/* 串口号 UART1 => 1*/
#define CMOS_CONSOLE_INDEX                                      (1)
/* 波特率 */
#define CMOS_CONSOLE_BAUDRATE                                   (115200)
/* 调试串口设备路径 */
#define CMOS_CONSOLE_DEVICE_PATH                                ("/dev/console")

/************************************ USB配置 **********************************/
/* 不使用USB */
//#define CMOS_USB

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_CONFIG_H_ */

