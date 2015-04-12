/******************************************************************************
 *
 * 文件名  ： thread_switch.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： v1.0
 * 文件描述： 线程调度实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 算法描述 见 thread_switch.c文件
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _THREAD_SWITCH_H_
#define _THREAD_SWITCH_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/*******************************************************************************
 *
 * 函数名  : thread_switch
 * 负责人  : 彭鹏
 * 创建日期：20150331 
 * 函数功能: 线程切换
 *
 * 输入参数: 当前任务sp
 *
 * 输出参数: 无
 *
 * 返回值  : 准备运行任务的sp
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void *thread_switch(const void *cur_stack);

#endif // #ifndef _THREAD_SWITCH_H_

