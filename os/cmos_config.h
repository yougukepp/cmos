/******************************************************************************
 *
 * 文件名  ： cmos_config.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： cmos配置文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CM_CMOS_CONFIG_H_
#define _CM_CMOS_CONFIG_H_

/************************************ 头文件 ***********************************/
#include "typedef.h"

/************************************ 宏定义 ***********************************/
/* 任务栈 顶部 */
#define CMOS_THREAD_STACK_BASE_32           (0x20030000)
#define CMOS_THREAD_STACK_BASE              ((cm_uint32_t *)CMOS_THREAD_STACK_BASE_32)

/* */
#define CMOS_INITIAL_XPSR                   (0x01000000)
/* */
#define CMOS_INITIAL_EXEC_RETURN            (0xfffffffd)

/* */
#define CMOS_IDLE_STACK_SIZE                (4096)

/* */
#define UART_PRINTF_BUF_LEN                 (128)
#define UART_BUF_LEN                        (1024)
#define UART_TIMEOUT_DIV                    (1)

/* 优先级个数
 * 0 Idle
 * 1 Low
 * 2 BelowNormal
 * 3 Normal
 * 4 AboveNormal
 * 5 High
 * 6 Realtime
 * */
#define CMOS_PRIORITY_MAX                   (7)

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif // #ifndef _CM_CMOS_CONFIG_H_

