/******************************************************************************
 *
 * 文件名  ： switch.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150411 
 * 版本号  ： v1.0
 * 文件描述： 线程切换算法的实现
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 
 *            TODO:
 *            1. 算法描述 *
 *            2. 该模块关键段较多,需要关中断
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "cmos_config.h"
#include "switch.h"
#include "tcb_list.h"
#include "thread.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/* 优先级 */
/*
  osPriorityIdle         0
  osPriorityLow          1
  osPriorityBelowNormal  2
  osPriorityNormal       3
  osPriorityAboveNormal  4
  osPriorityHigh         5
  osPriorityRealtime     6
  osPriorityError        7
  sys_thread_create维护
  bit7不能为1
  s_priority_cur不能为0x00
  0x00 < s_priority_cur <= 0x7F
  */
static cm_uint8_t s_priority_cur = 0x00;

/* 优先级线程链表数组 
 * 每个元素指向某一优先级的线程链表
 * 0 Idle
 * 1 Low
 * 2 BelowNormal
 * 3 Normal
 * 4 AboveNormal
 * 5 High
 * 6 Realtime
 * */
static cm_tcb_t *s_priority_tcb_table[CMOS_PRIORITY_MAX] = {NULL};

/* 
 * 每个位置表示s_priority_cur的值
 * 每个值表示一个优先级 0-6 同时是s_priority_tcb_table中的索引
 * tools/generate_bitmap.py生成
 * */
static const cm_uint8_t s_priority_bitmap[] = {
    0,  /* 00000000 Idle 没有任何优先级线程就绪 这是不可能的情况 */
    0,  /* 00000001 Idle */
    1,  /* 00000010 Low */
    1,  /* 00000011 Low */
    2,  /* 00000100 BelowNormal */
    2,  /* 00000101 BelowNormal */
    2,  /* 00000110 BelowNormal */
    2,  /* 00000111 BelowNormal */
    3,  /* 00001000 Normal */
    3,  /* 00001001 Normal */
    3,  /* 00001010 Normal */
    3,  /* 00001011 Normal */
    3,  /* 00001100 Normal */
    3,  /* 00001101 Normal */
    3,  /* 00001110 Normal */
    3,  /* 00001111 Normal */
    4,  /* 00010000 AboveNormal */
    4,  /* 00010001 AboveNormal */
    4,  /* 00010010 AboveNormal */
    4,  /* 00010011 AboveNormal */
    4,  /* 00010100 AboveNormal */
    4,  /* 00010101 AboveNormal */
    4,  /* 00010110 AboveNormal */
    4,  /* 00010111 AboveNormal */
    4,  /* 00011000 AboveNormal */
    4,  /* 00011001 AboveNormal */
    4,  /* 00011010 AboveNormal */
    4,  /* 00011011 AboveNormal */
    4,  /* 00011100 AboveNormal */
    4,  /* 00011101 AboveNormal */
    4,  /* 00011110 AboveNormal */
    4,  /* 00011111 AboveNormal */
    5,  /* 00100000 High */
    5,  /* 00100001 High */
    5,  /* 00100010 High */
    5,  /* 00100011 High */
    5,  /* 00100100 High */
    5,  /* 00100101 High */
    5,  /* 00100110 High */
    5,  /* 00100111 High */
    5,  /* 00101000 High */
    5,  /* 00101001 High */
    5,  /* 00101010 High */
    5,  /* 00101011 High */
    5,  /* 00101100 High */
    5,  /* 00101101 High */
    5,  /* 00101110 High */
    5,  /* 00101111 High */
    5,  /* 00110000 High */
    5,  /* 00110001 High */
    5,  /* 00110010 High */
    5,  /* 00110011 High */
    5,  /* 00110100 High */
    5,  /* 00110101 High */
    5,  /* 00110110 High */
    5,  /* 00110111 High */
    5,  /* 00111000 High */
    5,  /* 00111001 High */
    5,  /* 00111010 High */
    5,  /* 00111011 High */
    5,  /* 00111100 High */
    5,  /* 00111101 High */
    5,  /* 00111110 High */
    5,  /* 00111111 High */
    6,  /* 01000000 Realtime */
    6,  /* 01000001 Realtime */
    6,  /* 01000010 Realtime */
    6,  /* 01000011 Realtime */
    6,  /* 01000100 Realtime */
    6,  /* 01000101 Realtime */
    6,  /* 01000110 Realtime */
    6,  /* 01000111 Realtime */
    6,  /* 01001000 Realtime */
    6,  /* 01001001 Realtime */
    6,  /* 01001010 Realtime */
    6,  /* 01001011 Realtime */
    6,  /* 01001100 Realtime */
    6,  /* 01001101 Realtime */
    6,  /* 01001110 Realtime */
    6,  /* 01001111 Realtime */
    6,  /* 01010000 Realtime */
    6,  /* 01010001 Realtime */
    6,  /* 01010010 Realtime */
    6,  /* 01010011 Realtime */
    6,  /* 01010100 Realtime */
    6,  /* 01010101 Realtime */
    6,  /* 01010110 Realtime */
    6,  /* 01010111 Realtime */
    6,  /* 01011000 Realtime */
    6,  /* 01011001 Realtime */
    6,  /* 01011010 Realtime */
    6,  /* 01011011 Realtime */
    6,  /* 01011100 Realtime */
    6,  /* 01011101 Realtime */
    6,  /* 01011110 Realtime */
    6,  /* 01011111 Realtime */
    6,  /* 01100000 Realtime */
    6,  /* 01100001 Realtime */
    6,  /* 01100010 Realtime */
    6,  /* 01100011 Realtime */
    6,  /* 01100100 Realtime */
    6,  /* 01100101 Realtime */
    6,  /* 01100110 Realtime */
    6,  /* 01100111 Realtime */
    6,  /* 01101000 Realtime */
    6,  /* 01101001 Realtime */
    6,  /* 01101010 Realtime */
    6,  /* 01101011 Realtime */
    6,  /* 01101100 Realtime */
    6,  /* 01101101 Realtime */
    6,  /* 01101110 Realtime */
    6,  /* 01101111 Realtime */
    6,  /* 01110000 Realtime */
    6,  /* 01110001 Realtime */
    6,  /* 01110010 Realtime */
    6,  /* 01110011 Realtime */
    6,  /* 01110100 Realtime */
    6,  /* 01110101 Realtime */
    6,  /* 01110110 Realtime */
    6,  /* 01110111 Realtime */
    6,  /* 01111000 Realtime */
    6,  /* 01111001 Realtime */
    6,  /* 01111010 Realtime */
    6,  /* 01111011 Realtime */
    6,  /* 01111100 Realtime */
    6,  /* 01111101 Realtime */
    6,  /* 01111110 Realtime */
    6,  /* 01111111 Realtime */
};

/********************************** 函数声明区 *********************************/
/* O(1)算法 */
static cm_tcb_t *thread_switch_get_highest_tcb(void);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
void *thread_switch(const void *cur_psp)
{
    cm_uint32_t *next_psp = NULL;
    cm_tcb_t *next_tcb = NULL;

    /* 查找最高优先级线程 */
    next_tcb = thread_switch_get_highest_tcb();
    if(NULL == next_tcb) /* 无合适的可以切换线程 */
    {
        next_psp = (cm_uint32_t *)cur_psp;
    }
    else 
    {
        next_psp = next_tcb->psp;
    }

    return next_psp;
} 

/* O(1)算法 */
static cm_tcb_t *thread_switch_get_highest_tcb(void)
{
    cm_tcb_t *higighest_tcb = NULL;

    cm_uint8_t tcb_table_index = 0;
   
    /* 正确的优先级 0x00 < s_priority_cur <= 0x7F */
    if(! ((0x00 == s_priority_cur) || (0x80 & s_priority_cur)) )
    {
        tcb_table_index = s_priority_bitmap[s_priority_cur]; /* 查最高优先级 */
        higighest_tcb = s_priority_tcb_table[tcb_table_index]; /* 获取最高优先级线程TCB */
    }

    /* 错误(或无线程)返回 NULL */
    return higighest_tcb;
}

cm_tcb_t *get_tcb_head(cm_priority_t priority)
{
    cm_tcb_t *head = NULL;

    head = s_priority_tcb_table[priority];

    return head;
} 

void thread_switch_init_one_tcb(cm_tcb_t *ptr_tcb)
{ 
    cm_priority_t priority = 0;

    priority = thread_get_priority(ptr_tcb);
    /* 跟新优先级位图索引 */
    s_priority_cur |= (1 << priority);

    /* 初始化该优先级TCB链表 */
    s_priority_tcb_table[priority] = ptr_tcb;
}

void thread_switch_update_timeslice(void)
{
    cm_uint8_t tcb_table_index = 0;
    cm_tcb_t *higighest_tcb = NULL;
    cm_tcb_t *head = NULL;
    cm_tcb_t *tail = NULL;

    /* 查找最高优先级线程 */
    higighest_tcb = thread_switch_get_highest_tcb();

    higighest_tcb->tick--;
	
    /* 当前线程时间片结束 */
    if(0 == higighest_tcb->tick)
    {
        /* 重置时间片 */
        higighest_tcb->tick = higighest_tcb->time_slice;

        /* 若有其他线程把本线程移动到链表尾部 */
        if(NULL != higighest_tcb->next)
        {
            /* 获取表尾 */
            head = higighest_tcb;
            tail = tcb_list_goto_tail(head);
           
            /* 后一线程前移 */ 
            tcb_table_index = s_priority_bitmap[s_priority_cur];
            s_priority_tcb_table[tcb_table_index] = higighest_tcb->next;

            /* higighest_tcb 加入到表尾巴 */
            tail->next = higighest_tcb;
            higighest_tcb->next = NULL;
        }
    }
}


