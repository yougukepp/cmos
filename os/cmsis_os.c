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
#include "cmos.h"
#include "misc.h"
#include "cmsis_os.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/ 

/********************************** 函数声明区 *********************************/
static cm_priority_t osPriority2cmPriority(osPriority priority);


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
    cm_thread_id_t id = NULL;
    cm_thread_def_t cm_thread_def;

    cm_thread_def.pthread = thread_def->pthread;
    cm_thread_def.priority = osPriority2cmPriority(thread_def->tpriority);
    cm_thread_def.stack_size = thread_def->stacksize;
    cm_thread_def.time_slice = 100; /* 默认使用100 SysTick为单位 */

    id = thread_create(&cm_thread_def, argv);

    return (osThreadId)id;
}

/* CMSIS优先级转换为CM优先级 
 * bit0 Idle
 * bit1 Low
 * bit2 BelowNormal
 * bit3 Normal
 * bit4 AboveNormal
 * bit5 High
 * bit6 Realtime
 */
static cm_priority_t osPriority2cmPriority(osPriority priority)
{
    cm_priority_t cm_priority = 0;
    switch(priority)
    {
        case osPriorityIdle:
            {
                cm_priority = 0x01;
                break;
            }
        case osPriorityLow:
            {
                cm_priority = 0x02;
                break;
            }
        case osPriorityBelowNormal:
            {
                cm_priority = 0x04;
                break;
            }
        case osPriorityNormal:
            {
                cm_priority = 0x08;
                break;
            }
        case osPriorityAboveNormal:
            {
                cm_priority = 0x10;
                break;
            }
        case osPriorityHigh:
            {
                cm_priority = 0x20;
                break;
            }
        case osPriorityRealtime:
            {
                cm_priority = 0x40;
                break;
            }
        default:
            {
                Error_Handler();
            }
    }

    return cm_priority;
}



