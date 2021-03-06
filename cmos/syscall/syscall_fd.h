/******************************************************************************
 *
 * 文件名  ： syscall_fd.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151223 
 * 版本号  ： v1.0
 * 文件描述： fd相关系统调用
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_SYSCALL_SYSCALL_FD_H_
#define _CMOS_SYSCALL_SYSCALL_FD_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "fcb.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_fd_fcb_T *cmos_open_svc(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
void cmos_close_svc(cmos_fd_fcb_T *fcb);
cmos_int32_T cmos_read_svc(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write_svc(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_read_poll_svc(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write_poll_svc(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes);
void cmos_ioctl_svc(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para);

#endif // #ifndef _CMOS_SYSCALL_SYSCALL_FD_H_







