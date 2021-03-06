/******************************************************************************
 *
 * 文件名  ： fcb.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151221 
 * 版本号  ： v1.0
 * 文件描述： 文件控制块的实现
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "fcb.h"
#include "misc.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_init
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 初始化fcb
*
* 输入参数: path        文件在vfs中的路径
*           driver      文件对应的驱动
*           driver_id   driver需要的底层参数 例如STM32F4Cube的的xxxHanler
*           mutex_lock  文件互斥锁
*
* 输出参数: fcb         文件控制块指针
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_fd_fcb_init(cmos_fd_fcb_T *fcb,
        const cmos_int8_T *path, 
        const cmos_hal_driver_T *driver, 
        const void *driver_id, 
        const cmos_fd_mutex_T *mutex_lock)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 

    cmos_fd_fcb_set_path(fcb, path);
    cmos_fd_fcb_set_driver(fcb, driver);
    cmos_fd_fcb_set_driver_id(fcb, driver_id);
    cmos_fd_fcb_set_lock(fcb, mutex_lock);
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_set_path
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 设置fcb的vfs路径
*
* 输入参数: path文件在vfs中的路径
* 输出参数: fcb 文件控制块指针
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void cmos_fd_fcb_set_path(cmos_fd_fcb_T *fcb, const cmos_int8_T *path)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    fcb->path = (cmos_int8_T *)path;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_set_driver
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 设置fcb的驱动
*
* 输入参数: driver  文件驱动
* 输出参数: fcb     文件控制块指针
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void cmos_fd_fcb_set_driver(cmos_fd_fcb_T *fcb, const cmos_hal_driver_T *driver)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    fcb->driver = (cmos_hal_driver_T *)driver;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_set_driver_id
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 设置fcb的驱动id
*
* 输入参数: driver_id   驱动句柄
* 输出参数: fcb         文件控制块指针
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void cmos_fd_fcb_set_driver_id(cmos_fd_fcb_T *fcb, const void *driver_id)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    fcb->driver_id = (void *)driver_id;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_set_lock
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 设置fcb的互斥锁
*
* 输入参数: mutex_lock  互斥锁
* 输出参数: fcb         文件控制块指针
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void cmos_fd_fcb_set_lock(cmos_fd_fcb_T *fcb, const cmos_fd_mutex_T *mutex)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    fcb->mutex = (cmos_fd_mutex_T *)mutex;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_get_lock
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 获取fcb的互斥锁
*
* 输入参数: fcb         文件控制块指针
* 输出参数: 无
* 返回值  : mutex_lock  互斥锁
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline cmos_fd_mutex_T *cmos_fd_fcb_get_lock(const cmos_fd_fcb_T *fcb)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    return fcb->mutex;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_get_path
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 获取fcb的vfs路径
*
* 输入参数: fcb  文件控制块指针
* 输出参数: 无
* 返回值  : path vfs路径
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline cmos_int8_T *cmos_fd_fcb_get_path(const cmos_fd_fcb_T *fcb)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    return fcb->path;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_get_driver
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 获取fcb的driver
*
* 输入参数: fcb  文件控制块指针
* 输出参数: 无
* 返回值  : driver 驱动指针
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline cmos_hal_driver_T *cmos_fd_fcb_get_driver(const cmos_fd_fcb_T *fcb)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    return fcb->driver;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_fcb_get_driver_id
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 获取fcb的driver_id
*
* 输入参数: fcb  文件控制块指针
* 输出参数: 无
* 返回值  : driver_id   driver需要的底层参数 例如STM32F4Cube的的xxxHanler
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void *cmos_fd_fcb_get_driver_id(const cmos_fd_fcb_T *fcb)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__); 
    return fcb->driver_id;
}

