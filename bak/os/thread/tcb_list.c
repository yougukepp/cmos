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

void tcb_list_add(cm_tcb_t *head, cm_tcb_t *ptr_tcb)
{
    cm_tcb_t *tail = NULL;

    if(NULL == head)
    {
        return;
    }

    if(NULL == ptr_tcb)
    {
        return;
    }

    tail = tcb_list_goto_tail(head);
    tail->next = ptr_tcb;

}

/* 去链表头 */
cm_tcb_t *tcb_list_del_head(cm_tcb_t *head)
{
    cm_tcb_t *next = NULL;
    if(NULL != head)
    {
        next = head->next;
    }

    return next;
}

/* 遍历 */
void *tcb_list_walk(cm_tcb_t *head, cm_tcb_list_walk_func_t func)
{
    void *rst = NULL;
    cm_tcb_t *walker = NULL;

    walker = head;

    if(NULL == walker)
    {
        return NULL;
    }

    do{
        rst = func(walker);
        if(NULL != rst)
        {
            return rst;
        }

        walker = walker->next;
    }while(NULL != walker->next);
    return NULL;
}

