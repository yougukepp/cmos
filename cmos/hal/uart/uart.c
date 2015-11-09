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
const cmos_hal_driver_T g_uart_driver = {
    .open = uart_open,
    .read = uart_read,
    .write = uart_write,
    .ioctl = uart_ioctl,
    .close = uart_close
};

static UART_HandleTypeDef s_uart_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
cmos_int32_T uart_init(void)
{
    /* 依赖与 */
    s_uart_handle.Instance          = USARTx;

    s_uart_handle.Init.BaudRate     = 9600;
    s_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;
    s_uart_handle.Init.StopBits     = UART_STOPBITS_1;
    s_uart_handle.Init.Parity       = UART_PARITY_NONE;
    s_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    s_uart_handle.Init.Mode         = UART_MODE_TX_RX;
    s_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	
    if(HAL_UART_Init(&s_uart_handle) != HAL_OK)
    {
        CMOS_ERR_STR("tree root must not be NULL.");
        return cmos_CONSOLE_E;
    }

    return cmos_OK_E;
}

cmos_int32_T uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...)
{
    return 0;
}

cmos_status_T uart_close(cmos_int32_T dev_id)
{
    return cmos_OK_E;
}

cmos_int32_T uart_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

cmos_int32_T uart_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

cmos_status_T uart_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...)
{
    return cmos_OK_E;
}

