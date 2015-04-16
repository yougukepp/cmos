/******************************************************************************
 *
 * 文件名  ： tcb_list.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416 
 * 版本号  ： v1.0
 * 文件描述： 实现TCB链表需要的链表操作
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "tcb_list.h"
#include "switch.h"
#include "thread.h"
#include "tcb.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
cm_tcb_t *tcb_list_goto_tail(cm_tcb_t *head)
{
    cm_tcb_t *tail = NULL;

    cm_tcb_t *pre = NULL;
    const cm_tcb_t *cur = NULL;

    if(NULL == head) /* 头都是空的,头就是尾 */
    {
        return NULL;
    }

    pre = head;
    do
    {
        cur = pre->next; 
        if(NULL == cur) /* 位置找到 */
        {
            tail = pre;
            break;
        }
        pre = pre->next; /* 继续向后找 */
    }while(TRUE);

    return tail;
}

void tcb_list_add(cm_tcb_t *ptr_tcb)
{
    cm_tcb_t *head = NULL;
    cm_tcb_t *tail = NULL;
    cm_priority_t priority = 0;

    priority = tcb_get_priority(ptr_tcb);

    head = switch_get_first_tcb(priority);
    if(NULL == head) /* ptr_tcb是头节点 */
    { 
        switch_init_first_tcb(ptr_tcb);
    }
    else
    {
        tail = tcb_list_goto_tail(head);
        tail->next = ptr_tcb;
    }
}

/* 遍历 */
void tcb_list_walk(cm_tcb_t *head, cm_tcb_list_walk_func_t func)
{
    cm_tcb_t *walker = NULL;

    walker = head;

    if(NULL == walker)
    {
        return;
    }

    do{
        func(walker);
        walker = walker->next;
    }while(NULL != walker->next);
}

