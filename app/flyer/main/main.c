/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20160112
 * 版本号  ： 1.2
 * 文件描述： 飞控主控模块 不使用os
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
/* 消除中文打印警告 */
#pragma  diag_suppress 870

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "misc.h"
#include "console.h"
#include "main.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static cmos_int32_T s_task_flag = 0;

/********************************** 函数声明区 *********************************/
static void init(void);
static void task_gyro(void);
static void task_accel(void);
static void task_mag(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20160112
* 函数功能: MPU9250 主函数
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 主程序永不返回
*
* 调用关系: 无
* 其 它   : 获取MPU9250数据 中断中完成
*
******************************************************************************/
int main(void)
{ 
#if 0
    int times = 0;

    while(1)
    {
        cmos_debug_log("%05d: % 5d, %.5f", times++, 3, 7.1);
    }
#else

    init();
    while(1)
    {
        if(TASK_GYRO && s_task_flag)
        {
            task_gyro();
        }
        else if(TASK_ACCEL && s_task_flag)
        {
            task_accel();
        }
        else if(TASK_MAG && s_task_flag)
        { 
            task_mag();
        }
        else
        { 
            assert_failed(__FILE__, __LINE__);
        }
    }

#endif
}

/* 初始化 */
static void init(void)
{ 
    /* step1: 初始化硬件 */

    /* step1: 启动任务 */
    s_task_flag = (TASK_GYRO | TASK_ACCEL | TASK_MAG);
}

/* 陀螺仪融合 */
static void task_gyro(void)
{}

/* 加计融合 */
static void task_accel(void)
{}

/* 磁场融合 */
static void task_mag(void)
{}

