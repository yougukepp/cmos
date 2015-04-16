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
#ifndef __CMOS_CONFIG_H__
#define __CMOS_CONFIG_H__

/************************************ 头文件 ***********************************/
#include "typedef.h"

/************************************ 宏定义 ***********************************/
/* 任务栈 顶部 */
#define CMOS_THREAD_STACK_BASE_32           (0x20030000)
#define CMOS_THREAD_STACK_BASE              ((cm_uint32_t *)CMOS_THREAD_STACK_BASE_32)
/* 初始任务栈结构
       xPSR
       PC
       LR
       R12
       R3
       R2
       R1
       R0
thread_init_stack负责构造 */
#define CMOS_FIRST_STACK_INIT_TOP           ((cm_uint32_t *)(CMOS_THREAD_STACK_BASE_32 - 8 * 4))

/* */
#define CMOS_INITIAL_XPSR                   (0x01000000)
/* */
#define CMOS_INITIAL_EXEC_RETURN            (0xfffffffd)

/* */
#define CMOS_IDLE_STACK_SIZE                (4096)

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

#endif // #ifndef __CMOS_CONFIG_H__

