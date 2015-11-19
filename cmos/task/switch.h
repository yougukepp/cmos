/******************************************************************************
 *
 * 文件名  ： switch.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 任务切换头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_SWITCH_H_
#define _CMOS_TASK_SWITCH_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"

/************************************ 宏定义 ***********************************/
/*******************************************************************************
 *
 * 7个 有效 优先级
 * cmos_priority_idle           0
 * cmos_priority_low            1
 * cmos_priority_below_normal   2
 * cmos_priority_normal         3
 * cmos_priority_above_normal   4
 * cmos_priority_high           5
 * cmos_priority_realtime       6
 *
 ******************************************************************************/
#define CMOS_PRIORITY_NUMS                      (7)

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_task_switch_add(const cmos_task_tcb_T *tcb);

#endif // #ifndef _CMOS_TASK_SWITCH_H_

