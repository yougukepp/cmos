/********************************************************************************
*
* 文件名  ： usbd_conf.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150617
* 版本号  ： v1.0
* 文件描述： stm32 usb 设备库配置头文件 包含与PCD的接口
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_USBD_CONF_H_
#define _CMOS_HAL_USBD_CONF_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "console.h"

/************************************ 宏定义 ***********************************/
/* stm32 usb 设备库需要使用 */
#define USBD_MAX_NUM_INTERFACES             (1)
#define USBD_MAX_NUM_CONFIGURATION          (1)
#define USBD_ErrLog(...)                    cmos_err_log(__VA_ARGS__)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_HAL_USBD_CONF_H_ */

