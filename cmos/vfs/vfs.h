/******************************************************************************
 *
 * 文件名  ： driver.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 虚拟文件系统(vfs)的接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_VFS_H_
#define _CMOS_VFS_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T vfs_init(void);

#endif /* #ifndef _CMOS_VFS_H_ */
