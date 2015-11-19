/******************************************************************************
 *
 * 文件名  ： tcb.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 任务控制块接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_TCB_H_
#define _CMOS_TASK_TCB_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/
/* 任务控制块 */
typedef struct cmos_task_tcb_T_tag cmos_task_tcb_T;
struct cmos_task_tcb_T_tag{
    cmos_func_T     entry;      /* 入口函数 */
    void            *argv;      /* 入口函数参数 */

    cmos_int32_T    stack_size; /* 栈大小 Byte单位*/
    cmos_word_T     *psp;       /* 任务psp指针 以字为单位 */

    cmos_priority_T priority;   /* 优先级 */

    cmos_int32_T    tick_total; /* 时间片总数 */
    cmos_int32_T    tick;       /* 当前时间片 */
};

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_task_tcb_init(cmos_task_tcb_T *tcb, 
        cmos_func_T entry,
        void *argv,
        const cmos_task_attribute_T *task_attribute,
        cmos_word_T *stack_base);

cmos_int32_T cmos_task_tcb_get_stack_size(const cmos_task_tcb_T *tcb);
cmos_priority_T cmos_task_tcb_get_priority(const cmos_task_tcb_T *tcb);

#endif // #ifndef _CMOS_TASK_TCB_H_

