/******************************************************************************
 *
 * 文件名  ： list.c
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

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
cm_tcb_t *thread_switch_goto_tail(cm_tcb_t *head)
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

void thread_switch_add_thread(cm_tcb_t *ptr_tcb)
{
    cm_tcb_t *head = NULL;
    cm_tcb_t *tail = NULL;
    cm_priority_t priority = 0;

    priority = thread_get_priority(ptr_tcb);

    head = get_tcb_head(priority);
    if(NULL == head) /* ptr_tcb是头节点 */
    { 
        thread_switch_init_one_tcb(ptr_tcb);
    }
    else
    {
        tail = thread_switch_goto_tail(head);
        tail->next = ptr_tcb;
    }
}
