/******************************************************************************
 *
 * 文件名  ： cortex.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150617
 * 版本号  ： v1.0
 * 文件描述： CMSIS 及 stam32fcube cortex hal封装
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cortex.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 调度器锁 默认关闭 */
static cmos_int8_T s_cmos_hal_cortex_cortex_pendsv_lock = 1;

/********************************** 函数声明区 *********************************/
inline static cmos_bool_T cmos_hal_cortex_cortex_switch_locked(void);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_goto_unprivileged
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 进入非特权级别 使能psp
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_goto_unprivileged(void)
{
    __set_CONTROL(0x00000003);
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_set_pendsv
 * 负责人  : 彭鹏
 * 创建日期：20151121 
 * 函数功能: 悬起任务切换中断 
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_set_pendsv(void)
{
    /* 未锁才可调度 */
    if(!cmos_hal_cortex_cortex_switch_locked())
    { 
        /* 悬起PendSV异常 准备任务切换 */
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_systick_disable
 * 负责人  : 彭鹏
 * 创建日期：20151121 
 * 函数功能: 关闭systick时钟
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_systick_disable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_systick_start
 * 负责人  : 彭鹏
 * 创建日期：20151121 
 * 函数功能: 启动systick时钟
 *
 * 输入参数: ticks_num systick中断间隔毫秒数 
 * 输出参数: 无
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_cortex_cortex_systick_start(cmos_int32_T ticks)
{ 
    /* ticks 毫秒中断一次 */
    HAL_SYSTICK_Config(ticks * HAL_RCC_GetHCLKFreq()/1000); 
    
    /* 设置tick优先级 TICK_INT_PRIORITY 定义于 stm32f4xx_hal_conf.h */
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY,0);

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_disable_interrupt
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 关中断
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_disable_interrupt(void)
{
    __disable_irq();
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_enable_interrupt
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 开中断
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_enable_interrupt(void)
{
    __enable_irq();
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_enable_switch
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 开调度器锁
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_enable_switch(void)
{ 
    /* 需要使用关中断保护 */
    cmos_hal_cortex_cortex_disable_interrupt();
    s_cmos_hal_cortex_cortex_pendsv_lock = 0;
    cmos_hal_cortex_cortex_enable_interrupt();
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_enable_switch
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 关调度器锁
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_hal_cortex_cortex_disalbe_switch(void)
{
    /* 需要使用关中断保护 */
    cmos_hal_cortex_cortex_disable_interrupt();
    s_cmos_hal_cortex_cortex_pendsv_lock = 0;
    cmos_hal_cortex_cortex_enable_interrupt();
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_cortex_cortex_switch_locked
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 调度器是否上锁
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : TRUE  调度器上锁   禁止任务切换
 *           FALSE 调度器未上锁 可以任务切换
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static cmos_bool_T cmos_hal_cortex_cortex_switch_locked(void)
{
    if(0 == s_cmos_hal_cortex_cortex_pendsv_lock)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

