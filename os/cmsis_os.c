/******************************************************************************
 *
 * 文件名  ： cmsis_os.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： CMSIS RTOS 适配层 便于CMOS扩展
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： cmsis_os.h中的API在该文件实现
 *            API的说明见CMSIS RTOS文档
 *            每个API都是用SVC的方式实现
 *            每个函数的return仅仅是哄编译器开心
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmsis_os.h"
#include "cmos.h"
#include "stm32f4xx_hal.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/ 

/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*------------------------------- 内核信息与控制 ------------------------------*/
osStatus osKernelInitialize(void)   
{
    return kernel_initialize();
}

osStatus osKernelStart(void)   
{
    return kernel_start();
}

/*---------------------------------- 线程管理 ---------------------------------*/
osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argv)   
{
    return thread_create(thread_def, argv);
}

