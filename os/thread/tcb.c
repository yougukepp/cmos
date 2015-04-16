/******************************************************************************
 *
 * 文件名  ： tcb.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416
 * 版本号  ： v1.0
 * 文件描述： 实现线程控制块
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "tcb.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
cm_priority_t tcb_get_priority(cm_tcb_t *ptr_tcb)
{
    return ptr_tcb->priority;
}

void tcb_init(cm_tcb_t *ptr_tcb, const cm_thread_def_t *thread_def, void *argv, cm_uint32_t *init_psp)
{ 
    ptr_tcb->pthread = thread_def->pthread;
    ptr_tcb->argv = argv;
    ptr_tcb->stack_size = thread_def->stack_size;
    ptr_tcb->psp = init_psp;
    ptr_tcb->priority = thread_def->priority;
    ptr_tcb->tick_total = thread_def->time_slice;
    ptr_tcb->tick = thread_def->time_slice;
    ptr_tcb->next = NULL;
}

void tcb_set_delay(cm_tcb_t *ptr_tcb, cm_uint32_t mil_sec)
{
    ptr_tcb->delay = mil_sec;
}

void tcb_tick_dec(cm_tcb_t *ptr_tcb)
{
    ptr_tcb->tick--;
}

cm_bool_t tcb_tick_over(cm_tcb_t *ptr_tcb)
{
    return (0 == ptr_tcb->tick);
}

void tcb_tick_reset(cm_tcb_t *ptr_tcb)
{
    ptr_tcb->tick = ptr_tcb->tick_total;
}

void tcb_delay_dec(cm_tcb_t *ptr_tcb)
{
    /* TODO: TCB 实现线程的多个状态 */

    ptr_tcb->delay = 0;
}

