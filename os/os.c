/******************************************************************************
 *
 * 文件名  ： OS最小实现
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： v1.0
 * 文件描述： 任务切换
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmsis_os.h"

#include "typedef_tm.h"

#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"

#define OS_INITIAL_XPSR         (0x01000000)
#define OS_INITIAL_EXEC_RETURN  (0xfffffffd)

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/ 
/* 仅仅测试两个线程切换 */
osThreadCb_t g_thread_cb[2] = {{0}, {0}};
/* 当前线程 */
os_pthread g_cur_pthread = NULL;

/* 用户栈 */
/* 向下生长 故基地址: g_user_stack + ?? 问题？ */
//#define OS_USER_STACK_TOTAL  (1024 * 10)
//extern uint32 g_user_stack[OS_USER_STACK_TOTAL] = {0};
//extern uint32 *g_user_stack_base = g_user_stack + OS_USER_STACK_TOTAL; /* 初始栈顶 */
#define OS_USER_STACK_BASE ((uint32 *)0x20030000);
uint32 *g_user_stack_base = OS_USER_STACK_BASE;

/********************************** 函数声明区 *********************************/
static void osThreadExitError(void);
static uint32 *InitTaskStack(uint32 *sp, os_pthread funcName, void *argv);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*------------------------------- 内核信息与控制 ------------------------------*/
/*******************************************************************************
 *
 * 函数名  : osKernelInitialize
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 内核初始化
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值:   osStatus类型 定义于cmsis_os.h
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
osStatus osKernelInitialize(void)   
{
    /* 从用户堆栈顶 初始化4kBytes */
    uint32 *ptr = OS_USER_STACK_BASE;
    ptr--;
    for(int i = 0; i < 4096; i++)
    {
        *ptr-- = 0xA5A5A5A5;
    }

    /* TODO:定义一个Idle线程 */

    return osOK;
}

/*******************************************************************************
 *
 * 函数名  : osKernelStart
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 内核启动
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值:   osStatus类型 定义于cmsis_os.h
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
osStatus osKernelStart(void)   
{
    /* psp指向g_thread_cb[0] 启动之 */
    uint32 *psp_top = OS_USER_STACK_BASE;
    __set_PSP((uint32)psp_top); /* 初始化PSP */
	
    /* 使能所有的Fault */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk; 
    
    /* TODO:关中断 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SVCall_IRQn, SVC_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(PendSV_IRQn, PENDSV_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);
    /* TODO:开中断 */

    /* 不开浮点 使用PSP 非特权级 */ 
    __set_CONTROL(0x00000003);
    
    /* 启动线程 */
    g_cur_pthread = g_thread_cb[0].pthread;
    g_thread_cb[0].pthread(g_thread_cb[0].argv);

    return osOK;
}

/*******************************************************************************
 *
 * 函数名  : osKernelSysTick
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 获取SysTick值
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值:   g_tick定义于stm32f4xx_it.c
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
uint32 osKernelSysTick(void)
{
    return g_tick;
}

/*---------------------------------- 线程管理 ---------------------------------*/
/*******************************************************************************
 *
 * 函数名  : InitTaskStack
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 初始化任务栈的内容 使任务在首次运行时的上下文切换可以统一
 *
 * 输入参数: sp        - 堆栈指针
 *           funcName  - 任务入口函数名
 *           argv      - 任务参数
 *
 * 输出参数: 无
 *
 * 返回值:   新的堆栈指针
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
static uint32 *InitTaskStack(uint32 *sp, os_pthread funcName, void *argv)
{ 
    sp--;
    *sp = OS_INITIAL_XPSR; /* xPSR */

    sp--;
    *sp = (uint32 )funcName; /* PC */

    sp--;
    *sp = (uint32 )osThreadExitError; /* LR */

    /* 保留寄存器 R12 R3 R2 R1 R0 的空间 */
#if 1
    sp -= 5;	/* R12, R3, R2 and R1. */
    *sp = (uint32 )argv; /* R0 */
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
    *sp = (uint32)argv; /* R0 */
#endif

    /* 用于任务切换的中断返回 */
    sp--;
    *sp = OS_INITIAL_EXEC_RETURN;

    /* 保留R11, R10, R9, R8, R7, R6, R5 and R4. */
#if 1
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

/*******************************************************************************
 *
 * 函数名  : osThreadExitError
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 任务线程不可能退出 退出时调用该函数 便于调试
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值:   无
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
static void osThreadExitError(void)
{
    while(1)
    {
        ;
    }
}

/*******************************************************************************
 *
 * 函数名  : osThreadCreate
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 内核启动
 *
 * 输入参数: thread_def - 线程定义
 *           argument   - 线程参数
 *
 * 输出参数: 无
 *
 * 返回值:   线程ID 出错时为NULL
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argv)   
{
    static int times = 0;
    osThreadId id = NULL; 
    uint32 stack_size = 0;
    uint32 *new_psp = NULL;

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

    id->psp = g_user_stack_base;

    /* 初始化栈内容 */
    new_psp = InitTaskStack(id->psp, id->pthread, id->argv);
		id->psp = new_psp;

    /* 下一任务栈顶 */
    g_user_stack_base -= stack_size;

    return id;
}

/*---------------------------------- 等待函数 ---------------------------------*/
/*******************************************************************************
 *
 * 函数名  : osDelay
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 内核启动
 *
 * 输入参数: delayTime 延迟时间(ms)
 *
 * 输出参数: 无
 *
 * 返回值:   线程ID 出错时为NULL
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
osStatus osDelay(uint32 delayTime)
{
    uint32 start = 0;
    
    start = osKernelSysTick();

    /* 阻塞知道超时 */
    /* tick为uint32 可能溢出 */
    while(osKernelSysTick() - start < delayTime);
    {
        ;
    }

    return osOK;
}

