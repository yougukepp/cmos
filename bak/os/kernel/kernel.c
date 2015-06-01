/******************************************************************************
 *
 * 文件名  ： kernel.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： 内核信息与控制(Kernel Information and Control)
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "cmos_config.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "mem.h"
#include "switch.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/


/* 汇编定义 */
void first_thread_start(cm_uint32_t initial_exec_return);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
void syscall_kernel_initialize(void)
{
    mem_init();
	
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
	
    /* TODO:SVC中无法实现 不开浮点 使用MSP 非特权级 */ 
    __set_CONTROL(0x00000001);
}

void syscall_kernel_start(void)
{
    switch_start();
    
    /* TODO:SVC中无法实现 不开浮点 使用PSP 非特权级 */ 
    __set_CONTROL(0x00000003);

    first_thread_start(CMOS_INITIAL_EXEC_RETURN);
}

