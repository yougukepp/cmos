/******************************************************************************
 *
 * 文件名  ： running.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151211 
 * 版本号  ： v1.0
 * 文件描述： 当前运行任务接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_POOL_RUNNING_H_
#define _CMOS_TASK_POOL_RUNNING_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/ 
void cmos_task_pool_running_set_tcb(cmos_task_tcb_T *tcb); 
cmos_task_tcb_T *cmos_task_pool_running_get_tcb(void); 
void cmos_task_pool_running_set_delay(cmos_int32_T millisec);
void cmos_task_pool_running_set_psp(cmos_task_tcb_psp_T cur_psp);
void cmos_task_pool_running_update_tick(void);

#endif // #ifndef _CMOS_TASK_POOL_RUNNING_H_

