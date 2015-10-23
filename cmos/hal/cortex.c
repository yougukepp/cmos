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
}
