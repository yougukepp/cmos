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
/*******************************************************************************
*
* 函数名  : uart_init
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: uart中间层初始化
*
* 输入参数: uart_index 硬件串口编号
*           baud_rate  串口波特率
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 
*           TODO:以后通信串口也会使用
*                本模块内部管理多串口的共享问题
*                实现解初始化
*
******************************************************************************/
cmos_status_T uart_init(cmos_uint32_T uart_index, cmos_int32_T baud_rate)
{
    /* 由索引赋值串口基地址 */
    switch(uart_index)
    {
        case 1:
            {
                s_uart_handle.Instance = CONSOLE_UART;
                break;
            }
        default:
            {
                assert_failed(__FILE__, __LINE__);
                break;
            }

    }

    s_uart_handle.Init.BaudRate     = baud_rate;
    s_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;
    s_uart_handle.Init.StopBits     = UART_STOPBITS_1;
    s_uart_handle.Init.Parity       = UART_PARITY_NONE;
    s_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    s_uart_handle.Init.Mode         = UART_MODE_TX_RX;
    s_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;

    if(HAL_UART_Init(&s_uart_handle) != HAL_OK)
    {
        assert_failed(__FILE__, __LINE__);
    }

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : uart_send_poll
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: uart轮询方式输出
*
* 输入参数: buf 欲输出的字节流
*           len 字节流长度
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : TODO:实现中断、DMA方式
*
******************************************************************************/
cmos_status_T uart_send_poll(cmos_uint8_T *buf, cmos_int32_T len)
{
    HAL_StatusTypeDef status = HAL_ERROR;
    /* 传完才退出 */
    status= HAL_UART_Transmit(&s_uart_handle, (uint8_t*)buf, len, HAL_MAX_DELAY);
    if(HAL_OK != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : uart_send_poll
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: uart轮询方式输出
*
* 输入参数: buf 欲读取的字节流
*           buf_len 字节流长度
*
* 输出参数: 无
*
* 返回值  : 读出的字节流长度
*
* 调用关系: 无
* 其 它   : TODO:实现中断、DMA方式
*
******************************************************************************/
cmos_uint32_T uart_recv_poll(cmos_uint8_T *buf, cmos_uint32_T buf_len)
{
    HAL_StatusTypeDef status = HAL_ERROR;
	
	/* 使参数timeout为，等效于立即(1ms)返回 */
    status = HAL_UART_Receive(&s_uart_handle, (uint8_t *)buf, buf_len, 1);
    if(HAL_OK != status)
    {
        if(HAL_TIMEOUT == status)
        {
            return 0;
        }
        assert_failed(__FILE__, __LINE__);
    }
    return buf_len;
}
