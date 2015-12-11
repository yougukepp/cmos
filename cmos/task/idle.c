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
#include "cmos_api.h"
#include "idle.h"
#include "console.h"

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
    
    while(TRUE)
    { 
        cmos_console_printf("idle running %d\r\n", times);
        times++;
    } 
    
    /* 
     * 不可达
     * return; */
}

