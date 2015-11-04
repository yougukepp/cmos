/********************************************************************************
*
* 文件名  ： uart.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： cmos hal uart模块头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_UART_H_
#define _CMOS_HAL_UART_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "driver.h"

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/
extern const cmos_hal_driver_T g_uart_driver;


/*********************************** 接口函数 **********************************/
/* 驱动函数 */
cmos_int32_T uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...);
cmos_int32_T uart_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes);
cmos_int32_T uart_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes);
cmos_status_T uart_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...);
cmos_status_T uart_close(cmos_int32_T dev_id);

#endif /* _CMOS_HAL_UART_H_ */

