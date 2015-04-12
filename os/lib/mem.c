/******************************************************************************
 *
 * 文件名  ： mem.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150412 
 * 版本号  ： v1.0
 * 文件描述： 实现动态内存分配
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 动态内存分配 分为三种情况
 *            1. 待分配大小固定 栈算法 当前采用(大小固定为sizeof(cm_tcb_t)
 *            2. 待分配大小若干 使用内存池 
 *            3. 待分配大小不定 可以采用深入理解计算机系统(第2版)中9.9.6算法
 *
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "cmos_config.h"
#include "mem.h"
#include "stm32f4xx_hal.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* heap_base <= s_sp_top < heap_limit */
static cm_uint32_t s_heap_base = 0;  /* 堆区基地址 */
static cm_uint32_t s_heap_limit = 0; /* 堆区上限地址 */ 

/* 空闲内存链表首地址 */
static cm_mem_block_t *s_free_blk_head = NULL;
static cm_uint32_t    s_free_blk_max = 0;

/********************************** 函数声明区 *********************************/
cm_uint32_t get_heap_base(void);
cm_uint32_t get_heap_limit(void);
cm_uint32_t *get_initial_sp(void);

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void mem_init(void)
{
    cm_uint32_t heap_size = 0;
    cm_uint32_t mem_block_size = 0;
    cm_uint32_t count = 0; 
    cm_mem_block_t *mem_block_ptr = NULL;

    /* psp任务栈顶 */
    cm_uint32_t *psp_top = CMOS_THREAD_STACK_BASE;
    __set_PSP((cm_uint32_t)psp_top); /* 初始化PSP */

    /* 用户栈初始化为"USTK" */
    cm_uint32_t *u_ptr = CMOS_THREAD_STACK_BASE;
    cm_uint32_t *thread_stack_limit = get_initial_sp();
    do
    {
        *--u_ptr = 0x5553544B; /* 先减后赋值 ASCII "USTK" */
    }while(u_ptr > thread_stack_limit);

		/* 系统栈初始化为"SSTK" */
    cm_uint32_t *s_ptr = (cm_uint32_t *)get_heap_limit();
    cm_uint32_t *msp_now = (cm_uint32_t *)__get_MSP();
    do
    {
        *--s_ptr = 0x5353544B; /* 先减后赋值 ASCII "SSTK" */
    }while(s_ptr >= msp_now);

    /* 初始化堆 */
    /* 计算空闲内存块个数 */
    s_heap_base = get_heap_base();
    s_heap_limit = get_heap_limit();

    heap_size = s_heap_limit - s_heap_base;
    mem_block_size = sizeof(cm_mem_block_t);

    count = heap_size / mem_block_size; 
    
    /* 初始化空闲链表 */
    mem_block_ptr = (cm_mem_block_t *)s_heap_base;
    for(int i = 0; i<count; i++)
    { 
        for(int j = 0; j<CM_TCB_SIZE; j++)
        { 
            mem_block_ptr->data[j] = 0x48454150; /* ASCII "HEAP" */
        }
        mem_block_ptr->next = mem_block_ptr + 1;
        mem_block_ptr++;
    }

    s_free_blk_head = (cm_mem_block_t *)s_heap_base;
    s_free_blk_max = count;
}

cm_tcb_t *mem_malloc_tcb(void)
{
    cm_mem_block_t *new_addr = NULL;
    static cm_uint32_t malloced_count = 0;

    /* 已经用完堆内存 */
    if(malloced_count >= s_free_blk_max)
    {
        return NULL;
    } 
    
    malloced_count++;

    new_addr = s_free_blk_head;
    s_free_blk_head = s_free_blk_head->next;

    return (cm_tcb_t *)new_addr;
}

void mem_free_tcb(cm_tcb_t *ptr)
{
    cm_mem_block_t *mem_block_ptr = NULL;

    /* 参数检查 */
    if(NULL == ptr)
    {
        return;
    }

    mem_block_ptr = (cm_mem_block_t *)ptr; 
    
    /* 初始化堆内存 */
    for(int j = 0; j<CM_TCB_SIZE; j++)
    { 
        mem_block_ptr->data[j] = 0x48454150; /* ASCII "HEAP" */
    }

    /* 直接插入表头 */
    mem_block_ptr->next = s_free_blk_head;
    s_free_blk_head = mem_block_ptr;

}

