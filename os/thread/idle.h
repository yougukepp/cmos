/******************************************************************************
 *
 * 文件名  ： idle.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416 
 * 版本号  ： v1.0
 * 文件描述： 空闲线程头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CM_IDLE_H_
#define _CM_IDLE_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void idle_create(void);

/*******************************************************************************
*
* 函数名  : cm_idle_thread
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: idle线程
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void cm_idle_thread (void const *argv);
osThreadDef(cm_idle_thread, osPriorityIdle, 1, 0x1000);

#endif // #ifndef _CM_IDLE_H_

