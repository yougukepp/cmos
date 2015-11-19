/******************************************************************************
 *
 * 文件名  ： kernel.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117
 * 版本号  ： v1.0
 * 文件描述： 系统整体功能的头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_KERNEL_KERNEL_H_
#define _CMOS_KERNEL_KERNEL_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_kernel_init(void);
void cmos_kernel_start(void);

#endif // #ifndef _CMOS_KERNEL_KERNEL_H_

