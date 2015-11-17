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
#ifndef _CMOS_VFS_FD_H_
#define _CMOS_VFS_FD_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_int32_T vfs_fd_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
cmos_int32_T vfs_fd_read(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes);
cmos_int32_T vfs_fd_write(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes);
cmos_status_T vfs_fd_ioctl(cmos_int32_T fd, cmos_uint32_T request, cmos_uint32_T mode);
cmos_status_T vfs_fd_close(cmos_int32_T fd);

#endif // #ifndef _CMOS_VFS_FD_H_

