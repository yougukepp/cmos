/******************************************************************************
 *
 * 文件名  ： ready.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151211 
 * 版本号  ： v1.0
 * 文件描述： 就绪任务接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_POOL_READY_H_
#define _CMOS_TASK_POOL_READY_H_

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
#define CMOS_TASK_POOL_READY_PRIORITY_MAX   (7)

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void cmos_task_pool_ready_add(const cmos_task_tcb_T *tcb); /* 加入任务 */
void cmos_task_pool_ready_del(const cmos_task_tcb_T *tcb); /* 删除任务(移出就绪表) */

cmos_task_tcb_T *cmos_task_pool_ready_get_tcb(void); /* 获取最高优先级的就绪任务 */
cmos_task_tcb_T *cmos_task_pool_ready_pop_tcb(void); /* 弹出最高优先级的就绪任务 */ 

void cmos_task_pool_ready_update_tick(void); /* tick定时到 */

#endif // #ifndef _CMOS_TASK_POOL_READY_H_

