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
#include "stm32f429i_discovery.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static cm_uint32_t *thread_init_stack(cm_uint32_t *sp, cm_pthread_t funcName, void *argv);
static void thread_exit_error(void);
/*******************************************************************************
*
* 函数名  : job2
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: 任务2
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
static void job2 (void const *argv);
osThreadDef(job2, osPriorityNormal, 1, 0x1000);

/********************************** 变量实现区 *********************************/
/* 仅仅测试两个线程切换 */
cm_tcb_t g_thread_cb[2] = {{0}, {0}};
/* 当前线程 */
os_pthread g_cur_pthread = NULL;
/* 空闲的任务栈顶 */
static cm_uint32_t *s_user_stack_base = CMOS_THREAD_STACK_BASE;

/********************************** 函数实现区 *********************************/
cm_thread_id_t syscall_thread_create(const cm_thread_def_t *thread_def, void *argv)   
{
    static int times = 0;
    cm_thread_id_t id = NULL; 
    cm_uint32_t stack_size = 0;
    cm_uint32_t *new_psp = NULL;

    stack_size = thread_def->stacksize;
    
    /* 栈必须8 Bytes 对齐 */
    if(0 != (stack_size & 0x00000007))
    {
        return NULL;
    }
		
    /* 仅支持1、2次创建 */
    if((times < 0) || (times > 1))
    {
        return NULL;
    }

    if(0 == times)
    {
        id = &g_thread_cb[0];
    }
    else
    {
        id = &g_thread_cb[1];
    }
    times++; 
    
    /* 准备栈 */
    stack_size >>= 2;
    id->pthread = thread_def->pthread;
    id->argv = argv;

    id->priority = thread_def->tpriority;
    id->stack_size = stack_size; /* 以32Bytes为单位 */

    id->psp = s_user_stack_base;

    /* 初始化栈内容 */
    new_psp = thread_init_stack(id->psp, id->pthread, id->argv);
		id->psp = new_psp;

    /* 下一任务栈顶 */
    s_user_stack_base -= stack_size;

    return id;
}

/*******************************************************************************
 *
 * 函数名  : thread_switch
 * 负责人  : 彭鹏
 * 创建日期：20150331 
 * 函数功能: 任务切换
 *
 * 输入参数: 当前任务sp
 *
 * 输出参数: 无
 *
 * 返回值  : 准备运行任务的sp
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void *thread_switch(const void *cur_stack)
{
    static cm_uint32_t next_id = 0;
    cm_uint32_t *next_psp = NULL; 
    
    next_id++;
    next_id %= 2; 
    
    next_psp = g_thread_cb[next_id].psp; 
    return next_psp;
}

void thread_idle_create(void)
{
    osThreadCreate(osThread(job2), NULL);
}

static void job2 (void const *argument)
{
    int32_t i = 0;
    while (1)
    {
        while(i < 0x1fffff)
        {
            i++;
        }
        BSP_LED_Toggle(LED4);
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
