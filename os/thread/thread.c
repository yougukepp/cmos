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
#include "switch.h"
#include "stm32f429i_discovery.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cm_uint32_t *thread_init_stack(cm_uint32_t *sp, cm_pthread_t funcName, void *argv);
static void thread_exit_error(void);
/*******************************************************************************
*
* 函数名  : cm_idle_thread
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: idle线程
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void cm_idle_thread (void const *argv);
osThreadDef(cm_idle_thread, osPriorityIdle, 1, 0x1000);

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
        return NULL;
    }

    ptr_tcb = mem_malloc_tcb();
    /* 初始化ptr_tcb */
    ptr_tcb->pthread = thread_def->pthread;
    ptr_tcb->argv = argv;
    ptr_tcb->stack_size = stack_size;
    ptr_tcb->psp = s_user_stack_base;
    ptr_tcb->priority = thread_def->priority;
    ptr_tcb->time_slice = thread_def->time_slice;
    ptr_tcb->tick = thread_def->time_slice;
    ptr_tcb->next = NULL;

    /* 初始化栈内容 */
    ptr_tcb->psp = thread_init_stack(ptr_tcb->psp, ptr_tcb->pthread, ptr_tcb->argv);

    /* 计算下一任务栈顶 */
    s_user_stack_base -= (stack_size >> 2); /* 4Bytes对齐*/

    /* 通知线程切换模块有新线程 */ 
    thread_switch_add_thread(ptr_tcb);

    return (cm_thread_id_t)ptr_tcb;
}

void thread_idle_create(void)
{
    osThreadCreate(osThread(cm_idle_thread), NULL);
}

static void cm_idle_thread(void const *argument)
{
    int32_t i = 0;
    while (1)
    {
        while(i < 0x1fffff)
        {
            i++;
        }
        BSP_LED_Toggle(LED3);
        i = 0;
    }
}

static cm_uint32_t *thread_init_stack(cm_uint32_t *sp, cm_pthread_t funcName, void *argv)
{ 
    sp--;
    *sp = CMOS_INITIAL_XPSR; /* xPSR */

    sp--;
    *sp = (cm_uint32_t)funcName; /* PC */

    sp--;
    *sp = (cm_uint32_t )thread_exit_error; /* LR */

    /* 保留寄存器 R12 R3 R2 R1 R0 的空间 */
#if 0
    sp -= 5;	/* R12, R3, R2 and R1. */
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

static void thread_exit_error(void)
{
  while(1)
  {
  }
}
