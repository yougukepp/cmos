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

/************************************ 宏定义 ***********************************/
/* xPSR寄存器初值 Thumb状态 */
#define CMOS_INITIAL_XPSR                                   (0x01000000)
/* LR初值 Thread mode with float and psp */
#define CMOS_INITIAL_EXEC_RETURN_WITH_FLOAT                 (0xFFFFFFED)
#define CMOS_INITIAL_EXEC_RETURN_WITHOUT_FLOAT              (0xFFFFFFFD)

/* TODO:求取合适的值 FPSCR初值 */
#define CMOS_INITIAL_FPSCR                                  (0x00000000)

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/ 
void cmos_hal_cortex_cortex_goto_unprivileged(void);
void cmos_hal_cortex_cortex_set_pendsv(void);
void cmos_hal_cortex_cortex_systick_disable(void);
cmos_status_T cmos_hal_cortex_cortex_systick_start(cmos_int32_T ticks_num);

#endif /* #ifndef _CMOS_HAL_CORTEX_CORTEX_H_ */

