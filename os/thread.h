/******************************************************************************
 *
 * 文件名  ： thread.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： 线程管理头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef __THREAD_H__
#define __THREAD_H__

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "thread_switch.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cm_thread_id_t syscall_thread_create(const cm_thread_def_t *thread_def, void *argv);

#endif // #ifndef __THREAD_H__


