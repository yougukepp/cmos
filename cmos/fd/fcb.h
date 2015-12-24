/******************************************************************************
 *
 * 文件名  ： fcb.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151221 
 * 版本号  ： v1.0
 * 文件描述： 文件控制块(描述符)接口
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_FD_FCB_H_
#define _CMOS_FD_FCB_H_

/************************************ 头文件 ***********************************/
#include "driver.h"
#include "mutex.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
/* 文件控制块 */
typedef struct cmos_fd_fcb_T_tag cmos_fd_fcb_T;
struct cmos_fd_fcb_T_tag{
    cmos_int8_T         *path;      /* vfs路径 */
    cmos_hal_driver_T   *driver;    /* 驱动指针 如uart等 */
    void                *driver_id; /* 驱动底层句柄 如STM32Cube4 HAL Handle等 */
    cmos_fd_mutex_T     *mutex;     /* 互斥锁 */
};

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/ 
void cmos_fd_fcb_init(cmos_fd_fcb_T *fcb,
        const cmos_int8_T *path_str, 
        const cmos_hal_driver_T *driver, 
        const void *driver_id, 
        const cmos_fd_mutex_T *mutex_lock);

void cmos_fd_fcb_set_path(cmos_fd_fcb_T *fcb, const cmos_int8_T *path_str);
void cmos_fd_fcb_set_driver(cmos_fd_fcb_T *fcb, const cmos_hal_driver_T *driver);
void cmos_fd_fcb_set_driver_id(cmos_fd_fcb_T *fcb, const void *driver_id);
void cmos_fd_fcb_set_lock(cmos_fd_fcb_T *fcb, const cmos_fd_mutex_T *mutex_lock);


cmos_int8_T *cmos_fd_fcb_get_path(const cmos_fd_fcb_T *fcb);
cmos_hal_driver_T *cmos_fd_fcb_get_driver(const cmos_fd_fcb_T *fcb);
void *cmos_fd_fcb_get_driver_id(const cmos_fd_fcb_T *fcb);
cmos_fd_mutex_T *cmos_fd_fcb_get_lock(const cmos_fd_fcb_T *fcb);

#endif // #ifndef _CMOS_FD_FCB_H_

