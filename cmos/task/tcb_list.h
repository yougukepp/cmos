/******************************************************************************
 *
 * 文件名  ： tcb_list.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151119 
 * 版本号  ： v1.1
 * 文件描述： 任务控制块链表接口
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_TCB_LIST_H_
#define _CMOS_TASK_TCB_LIST_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"
#include "list.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef cmos_lib_list_node_T cmos_task_tcb_list_node_T;
typedef cmos_task_tcb_list_node_T cmos_task_tcb_list_T;

typedef void (*cmos_task_tcb_list_walk_func_T)(cmos_task_tcb_T *tcb, void *para);

typedef struct cmos_task_tcb_list_walk_func_para_T_tag
{
    cmos_task_tcb_list_walk_func_T  func;
    void                            *para;
}cmos_task_tcb_list_walk_func_para_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_task_tcb_list_node_T *cmos_task_tcb_list_malloc_node(const cmos_task_tcb_T *tcb);

cmos_status_T cmos_task_tcb_list_add(cmos_task_tcb_list_T **list, const cmos_task_tcb_T *tcb);
cmos_status_T cmos_task_tcb_list_del(cmos_task_tcb_list_T **list, const cmos_task_tcb_T *tcb);

cmos_task_tcb_T *cmos_task_tcb_list_get_head_tcb(const cmos_task_tcb_list_T *list);

void cmos_task_tcb_list_walk(cmos_task_tcb_list_T *tcb_list, cmos_task_tcb_list_walk_func_T func, void *para);

cmos_status_T cmos_task_tcb_list_head_move_to_tail(cmos_task_tcb_list_T **tcb_list);

#endif // #ifndef _CMOS_TASK_TCB_LIST_H_

