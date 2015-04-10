/******************************************************************************
 *
 * 文件名  ： kernel.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： 内核管理 头文件
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef __KERNEL_H__
#define __KERNEL_H__

/************************************ 头文件 ***********************************/
#include "typedef.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cm_uint32_t syscall_kernel_initialize(void);
cm_uint32_t syscall_kernel_start(void);

#endif // #ifndef __KERNEL_H__


