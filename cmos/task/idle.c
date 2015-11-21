/******************************************************************************
 *
 * 文件名  ： idle.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 实现空闲任务
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 系统至少会运行空闲任务
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "idle.h"
#include "console.h"
#include "cortex.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_idle_task
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 空闲任务入口函数
 *
 * 输入参数: argv 空闲任务参数
 *
 * 输出参数: 无
 * 返回值:   无
 *
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
void cmos_task_idle_task(void *argv)
{
    static cmos_int32_T times = 0;

    /* 设置中断优先级 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SVCall_IRQn, SVC_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(PendSV_IRQn, PENDSV_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);

    /* step0: 启动systick */ 
    cmos_hal_cortex_cortex_systick_start(CMOS_TICK_TIMES);
    /* step1: 进入非特权级别 */
    cmos_hal_cortex_cortex_goto_unprivileged();

    while(TRUE)
    { 
        cmos_console_printf("idle running %d\r\n", times);
        times++;
    } 
    
    /* 
     * 不可达
     * return; */
}

