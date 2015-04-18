/******************************************************************************
 *
 * 文件名  ： tcb.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416 
 * 版本号  ： v1.0
 * 文件描述： 线程控制块头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CM_TCB_H_
#define _CM_TCB_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cm_priority_t tcb_get_priority(cm_tcb_t *ptr_tcb);
void tcb_init(cm_tcb_t *ptr_tcb, const cm_thread_def_t *thread_def, void *argv, cm_uint32_t *init_psp);

void tcb_set_delay(cm_tcb_t *ptr_tcb, cm_uint32_t mil_sec);

void tcb_tick_dec(cm_tcb_t *ptr_tcb);

cm_bool_t tcb_tick_over(cm_tcb_t *ptr_tcb);

void tcb_tick_reset(cm_tcb_t *ptr_tcb);

void tcb_delay_dec(cm_tcb_t *ptr_tcb);

cm_uint32_t tcb_get_delay(cm_tcb_t *ptr_tcb);

cm_uint32_t *tcb_get_psp(cm_tcb_t *ptr_tcb);


#endif // #ifndef _CM_TCB_H_


