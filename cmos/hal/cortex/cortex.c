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
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "cortex.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/

/*******************************************************************************
 *
 * 函数名  : SysTick_Handler
 * 负责人  : 彭鹏
 * 创建日期：20150321 
 * 函数功能: SysTick ISR
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 1ms 该函数会被调用一次
 *
 ******************************************************************************/
void SysTick_Handler(void)
{
    /* tick自增 */
    HAL_IncTick();

    /* step1: 处理 tcb_list 时间信息 */
    /* switch_update_tcb_time(); */
    
    /* 调度 */ 
    cmos_hal_cortex_cortex_set_pendsv();
}

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
void cmos_hal_cortex_cortex_goto_unprivileged(void)
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
void cmos_hal_cortex_cortex_set_pendsv(void)
{
    /* 悬起PendSV异常(此时必然为咬尾中断) 准备任务切换 */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
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
void cmos_hal_cortex_cortex_systick_disable(void)
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
 * 输入参数: ticks_num systick中断间隔tick数 
 * 输出参数: 无
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_cortex_cortex_systick_start(cmos_int32_T ticks_num)
{
    if(0 == HAL_SYSTICK_Config(ticks_num))
    {
        return cmos_OK_E;
    }
    else
    {
        CMOS_ERR_STR("cmos_hal_cortex_cortex_systick_start set ticks.");
        return cmos_ERR_E;
    }
}

