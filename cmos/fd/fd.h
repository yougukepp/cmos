/******************************************************************************
 *
 * 文件名  ： fd.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 文件描述符接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_FD_FD_H_
#define _CMOS_FD_FD_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "fcb.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef struct{
    const void *driver_id;  /* 任务控制块 输入 */
    cmos_fd_mutex_T *mutex; /* 锁 输出 */
}cmos_fd_compare_para_T;    /* fd模块专用 */

typedef struct{
    const cmos_int8_T *path;/* 文件路径 */
    cmos_fd_fcb_T *fcb;     /* 文件控制块 输出 */
}cmos_fd_find_para_T;    /* fd模块专用 */

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* 标准文件操作 */
/* 注意:以下函数 运行与内核态 svc之后 */
cmos_fd_fcb_T *cmos_fd_open(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
cmos_int32_T cmos_fd_read(cmos_fd_fcb_T *fd, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_fd_write(cmos_fd_fcb_T *fd, void *buf, cmos_int32_T n_bytes);
cmos_status_T cmos_fd_ioctl(cmos_fd_fcb_T *fd, cmos_uint32_T request, cmos_uint32_T mode);
void cmos_fd_close(cmos_fd_fcb_T *fd);
cmos_int32_T cmos_fd_read_poll(cmos_fd_fcb_T *fd, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_fd_write_poll(cmos_fd_fcb_T *fd, void *buf, cmos_int32_T n_bytes);

/* 处理锁问题 */
/* 注意:以下函数 运行与用户态 */
void cmos_fd_write_u(const cmos_fd_fcb_T *fcb);
void cmos_fd_read_u(const cmos_fd_fcb_T *fcb);

/* 驱动中使用 */
void cmos_fd_unlock_by_driver_id(const void *driver_id);
#if 0
void cmos_fd_unlock_by_driver_id_spin(const void *driver_id);
#endif

#endif // #ifndef _CMOS_FD_FD_H_

