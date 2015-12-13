/******************************************************************************
 *
 * 文件名  ： task.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 任务实现接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_TASK_H_
#define _CMOS_TASK_TASK_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_task_create(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute); /* 创建任务 */

cmos_status_T cmos_task_delay(cmos_int32_T millisec);
void cmos_task_tick_callback(void);

#endif // #ifndef _CMOS_TASK_TASK_H_


