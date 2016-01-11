/******************************************************************************
 *
 * 文件名  ： driver.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 硬件驱动的接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_DRIVER_H_
#define _CMOS_HAL_DRIVER_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef void *(*cmos_open_func_T)(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
typedef cmos_int32_T (*cmos_read_func_T)(const void *dev_id, void *buf, cmos_int32_T n_bytes);
typedef cmos_int32_T (*cmos_write_func_T)(const void *dev_id, const void *buf, cmos_int32_T n_bytes);
typedef cmos_status_T (*cmos_ioctl_func_T)(const void *dev_id, cmos_uint32_T request, cmos_uint32_T para);
typedef cmos_status_T (*cmos_close_func_T)(const void *dev_id);

typedef struct cmos_hal_driver_T_tag{
    cmos_open_func_T    open;
    cmos_read_func_T    read;
    cmos_write_func_T   write;
    cmos_ioctl_func_T   ioctl;
    cmos_close_func_T   close;
    cmos_read_func_T    read_poll;
    cmos_write_func_T   write_poll;
}cmos_hal_driver_T;
/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* #ifndef _CMOS_HAL_DRIVER_H_ */

