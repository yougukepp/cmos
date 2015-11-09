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
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static cmos_int32_T uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...);
static cmos_int32_T uart_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes);
static cmos_int32_T uart_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes);
static cmos_status_T uart_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...);
static cmos_status_T uart_close(cmos_int32_T dev_id);

/* 驱动变量 加入到vfs */
const cmos_hal_driver_T g_uart_driver = {
    .open = uart_open,
    .read = uart_read,
    .write = uart_write,
    .ioctl = uart_ioctl,
    .close = uart_close
};

/* STM32F4Cube HAL驱动 */
static UART_HandleTypeDef s_uart_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_hal_uart_init
* 负责人  : 彭鹏
* 创建日期: 20151109
* 函数功能: cmos hal uart初始化
*
* 输入参数: 波特率和串口号
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: hal_init中和其他硬件的初始化一起调
* 其 它   : 无
*
******************************************************************************/
void cmos_hal_uart_init(void *para)
{ 
    const cmos_hal_uart_init_para_T *init_para = para;
    /* 仅实现一个串口 */
    switch(init_para->uart_index)
    {
        case 1:
            {
                s_uart_handle.Instance = USART1;
                break;
            }
        default:
            {
                cmos_err_log("mcu not support %d hardware uart.", init_para->uart_index);
                return;
            }
    }

    s_uart_handle.Init.BaudRate     = init_para->baud_rate;
    s_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;
    s_uart_handle.Init.StopBits     = UART_STOPBITS_1;
    s_uart_handle.Init.Parity       = UART_PARITY_NONE;
    s_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    s_uart_handle.Init.Mode         = UART_MODE_TX_RX;
    s_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	
    if(HAL_UART_Init(&s_uart_handle) != HAL_OK)
    {
        CMOS_ERR_STR("HAL_UART_Init failed.");
        return;
    }

    return;
}

static cmos_int32_T uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...)
{
    return 0;
}

static cmos_status_T uart_close(cmos_int32_T dev_id)
{
    return cmos_OK_E;
}

static cmos_int32_T uart_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

static cmos_int32_T uart_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

static cmos_status_T uart_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...)
{
    return cmos_OK_E;
}

