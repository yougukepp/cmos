/******************************************************************************
 *
 * 文件名  ： device.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030
 * 版本号  ： v1.0
 * 文件描述： 硬件设备树的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tree.h"
#include "device.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
cmos_status_T hal_device_init(void)
{
    cmos_status_T status = cmos_ERR_E; 

    status = cmos_OK_E;
    return status;
}

cmos_status_T hal_device_add(const cmos_uint8_T *path)
{
    cmos_status_T status = cmos_ERR_E; 

    status = cmos_OK_E;
    return status;
}

cmos_status_T hal_device_del(const cmos_uint8_T *path)
{
    cmos_status_T status = cmos_ERR_E; 

    status = cmos_OK_E;
    return status;
}
