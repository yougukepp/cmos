/******************************************************************************
 *
 * 文件名  ： uart.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150614 
 * 版本号  ： 1.0
 * 文件描述： uart cmos hal,用于支持cmos控制台模块
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "uart.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f429idiscovery_hardware.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
UART_HandleTypeDef s_uart_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
cmos_status_T cmos_hal_uart_init(void)
{
    return cmos_OK_E;
}

cmos_int32_T cmos_hal_uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...)
{
    return 0;
}

cmos_status_T cmos_hal_uart_close(cmos_int32_T dev_id)
{
    return cmos_OK_E;
}

cmos_int32_T cmos_hal_uart_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

cmos_int32_T cmos_hal_uart_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

cmos_status_T cmos_hal_uart_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...)
{
    return cmos_OK_E;
}

