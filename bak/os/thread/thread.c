/******************************************************************************
 *
 * 文件名  ： thread.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： 线程管理(Thread Management)
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "cmos_config.h"
#include "mem.h"
#include "misc.h"
#include "switch.h"
#include "tcb.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cm_uint32_t *thread_init_stack(cm_uint32_t *sp, cm_pthread_t funcName, void *argv);

/********************************** 变量实现区 *********************************/
/* 空闲的任务栈顶 */
static cm_uint32_t *s_user_stack_base = CMOS_THREAD_STACK_BASE;

/********************************** 函数实现区 *********************************/
cm_thread_id_t syscall_thread_create(const cm_thread_def_t *thread_def, void *argv)   
{ 
    cm_tcb_t *ptr_tcb = NULL;
    cm_uint32_t stack_size = 0;

    stack_size = thread_def->stack_size;
    /* 栈大小必须4Bytes对齐 */
    if(0 != (stack_size & 0x00000003))
    {
        return (cm_thread_id_t)NULL;
    }

    /* TODO: 线程终止时释放 避免内存泄露 */
    ptr_tcb = mem_malloc_tcb();
    if(NULL == ptr_tcb)
    {
        return (cm_thread_id_t)NULL;
    }

    /* 初始化ptr_tcb */
    tcb_init(ptr_tcb, thread_def, argv, s_user_stack_base);

    /* TODO:使用tcb模块 */
    /* 初始化栈内容 */
    ptr_tcb->psp = thread_init_stack(ptr_tcb->psp, ptr_tcb->pthread, ptr_tcb->argv);

    /* 计算下一任务栈顶 */
    s_user_stack_base -= (stack_size >> 2); /* 4Bytes对齐*/

    /* 通知线程切换模块有新线程 */ 
    switch_add(ptr_tcb);

    return (cm_thread_id_t)ptr_tcb;
}

static cm_uint32_t *thread_init_stack(cm_uint32_t *sp, cm_pthread_t funcName, void *argv)
{ 
    sp--;
    *sp = CMOS_INITIAL_XPSR; /* xPSR */

    sp--;
    *sp = (cm_uint32_t)funcName; /* PC */

    sp--;
    *sp = (cm_uint32_t )Error_Handler; /* LR */

    /* 保留寄存器 R12 R3 R2 R1 R0 的空间 */
#if 0
    sp -= 5;    /* R12, R3, R2 and R1. */
    *sp = (cm_uint32_t )argv; /* R0 */
#else /* 调试 */
    sp--;
    *sp = 0x12121212; /* R12 */

    sp--;
    *sp = 0x03030303; /* R3 */

    sp--;
    *sp = 0x02020202; /* R2 */

    sp--;
    *sp = 0x01010101; /* R1 */

    sp--;
    *sp = (cm_uint32_t)argv; /* R0 */
#endif

    /* 用于任务切换的中断返回 */
    sp--;
    *sp = CMOS_INITIAL_EXEC_RETURN;

    /* 保留R11, R10, R9, R8, R7, R6, R5 and R4. */
#if 0
    sp -= 8;
#else /* 调试 */
    sp--;
    *sp = 0x11111111; /* R11 */

    sp--;
    *sp = 0x10101010; /* R10 */

    sp--;
    *sp = 0x09090909; /* R9 */

    sp--;
    *sp = 0x08080808; /* R8 */

    sp--;
    *sp = 0x07070707; /* R7 */

    sp--;
    *sp = 0x06060606; /* R6 */

    sp--;
    *sp = 0x05050505; /* R5 */

    sp--;
    *sp = 0x04040404; /* R4 */
#endif

    return sp;
}

