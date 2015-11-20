/******************************************************************************
 *
 * 文件名  ： systick.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150418 
 * 版本号  ： v1.0
 * 文件描述： systick接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_CORTEX_CORTEX_H_
#define _CMOS_HAL_CORTEX_CORTEX_H_

/************************************ 头文件 ***********************************/
#include "stm32f4xx_hal.h"

/************************************ 宏定义 ***********************************/
/* xPSR寄存器初值 Thumb状态 */
#define CMOS_INITIAL_XPSR                                   (0x01000000)
/* LR初值 Thread mode with float and psp */
#define CMOS_INITIAL_EXEC_RETURN                            (0xfffffffd)
/* TODO:求取合适的值 FPSCR初值 */
#define CMOS_INITIAL_FPSCR                                  (0x00000000)

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/ 
/* 
 * float使能  进非特权
 * */
#define CMOS_HAL_CORTEX_CORTEX_GOTO_TASK_CONTEXT() __set_CONTROL(0x00000005)
/* 
 * 设置psp
 * */
#define CMOS_HAL_CORTEX_CORTEX_SET_PSP(psp) __set_PSP(psp)
/* 
 * 使能
 * */
#define CMOS_HAL_CORTEX_CORTEX_ENABLE_PSP() \
    do{ \
        cmos_int32_T reg = __get_CONTROL(); \
        __set_CONTROL(reg | 0x00000002); \
    }while(0);

#endif /* #ifndef _CMOS_HAL_CORTEX_CORTEX_H_ */

