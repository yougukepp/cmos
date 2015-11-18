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

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/

/* 任务控制块 */
typedef struct cmos_task_tcb_T_tag cmos_task_tcb_T;
struct cmos_task_tcb_T_tag{
    cmos_func_T     entry;      /* 入口函数 */
    void            *argv;      /* 入口函数参数 */

    cmos_int32_T    stack_size; /* 栈大小 Byte单位*/
    cmos_uint32_T   *psp;       /* 任务sp指针 */

    cmos_priority_T priority;   /* 优先级 */

    cmos_int32_T    tick_total; /* 时间片总数 */
    cmos_int32_T    tick;       /* 当前时间片 */
};

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_task_create(cmos_task_id_T *task_id, 
        cmos_func_T task_func,
        void *argv,
        const cmos_task_attribute_T *task_attribute);

#endif // #ifndef _CMOS_TASK_TASK_H_


