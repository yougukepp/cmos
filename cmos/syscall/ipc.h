/******************************************************************************
 *
 * 文件名  ： ipc.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151223 
 * 版本号  ： v1.0
 * 文件描述： ipc系统调用
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_SYSCALL_IPC_H_
#define _CMOS_SYSCALL_IPC_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void cmos_ipc_svc(cmos_ipc_type_T type, void *para);

#endif // #ifndef _CMOS_SYSCALL_SYSCALL_H_

