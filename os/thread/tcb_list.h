/******************************************************************************
 *
 * 文件名  ： tcb_list.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416 
 * 版本号  ： v1.0
 * 文件描述： 线程模块所需链表操作的头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 实现通用的链表操作可以参考
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CM_TCB_LIST_H_
#define _CM_TCB_LIST_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/*******************************************************************************
 *
 * 函数名  : tcb_list_add
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: TCB链表中加入新TCB
 *
 * 输入参数: head    - 待加入的TCB链表头
 *           ptr_tcb - 待加入的TCB的地址
 * 输出参数: 无
 *
 * 返回值:   无
 * 调用关系: 无
 * 其 它:    链表加入  可以参考
 *
 ******************************************************************************/
void tcb_list_add(cm_tcb_t *head, cm_tcb_t *ptr_tcb);

/*******************************************************************************
 *
 * 函数名  : tcb_list_goto_tail
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 遍历到TCB链表表尾
 *
 * 输入参数: head - 链表头
 * 输出参数: 无
 *
 * 返回值:   NULL 目前链表是空的
 *           其它 链表尾结点指针
 * 调用关系: 无
 * 其 它:    链表找尾 可以参考
 *
 ******************************************************************************/
cm_tcb_t *tcb_list_goto_tail(cm_tcb_t *head);

cm_tcb_t *tcb_list_del_head(cm_tcb_t *head);

void *tcb_list_walk(cm_tcb_t *head, cm_tcb_list_walk_func_t func);

#endif // #ifndef _CM_TCB_LIST_H_

