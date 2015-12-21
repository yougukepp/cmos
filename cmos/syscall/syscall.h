/******************************************************************************
 *
 * 文件名  ： syscall.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151217 
 * 版本号  ： v1.0
 * 文件描述： 系统调用内部接口
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 供内核代码使用 移除
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_SYSCALL_SYSCALL_H_
#define _CMOS_SYSCALL_SYSCALL_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "fd.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void cmos_init_p(void);
void cmos_start_p(void);
cmos_bool_T cmos_running_p(void);

cmos_status_T cmos_create_p(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute);

cmos_status_T cmos_delay_p(cmos_int32_T millisec);

void cmos_enable_interrupt_p(void);
void cmos_disable_interrupt_p(void);
void cmos_enable_switch_p(void);
void cmos_disable_switch_p(void);

cmos_fd_T cmos_open_p(const cmos_uint8_T *path, cmos_uint32_T flag, ...);
cmos_status_T cmos_close_p(cmos_fd_T fd);
cmos_int32_T cmos_read_p(cmos_fd_T fd, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write_p(cmos_fd_T fd, void *buf, cmos_int32_T n_bytes);
cmos_status_T cmos_ioctl_p(cmos_fd_T fd, cmos_uint32_T request, ...);
cmos_int32_T cmos_read_poll_p(cmos_fd_T fd, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write_poll_p(cmos_fd_T fd, void *buf, cmos_int32_T n_bytes);

#endif // #ifndef _CMOS_SYSCALL_SYSCALL_H_

