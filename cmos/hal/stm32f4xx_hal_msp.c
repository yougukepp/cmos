/******************************************************************************
 *
 * 文件名  ： stm32f4xx_hal_msp.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150615
 * 版本号  ： 1.0
 * 文件描述： stm32f4 cube hal 初始化回调
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "stm32f4xx_hal_msp.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : HAL_UART_MspInit
* 负责人  : 彭鹏
* 创建日期: 20150615
* 函数功能: stm32f4 hal uart初始化回调
*
* 输入参数: stm32 hal uart句柄
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
    /************************ 控制台使用的串口初始化 ************************/
    GPIO_InitTypeDef  GPIO_InitStruct;

    CONSOLE_UART_TX_GPIO_CLK_ENABLE();
    CONSOLE_UART_RX_GPIO_CLK_ENABLE();
    CONSOLE_UART_CLK_ENABLE();

    GPIO_InitStruct.Pin       = CONSOLE_UART_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = CONSOLE_UART_TX_AF;
    HAL_GPIO_Init(CONSOLE_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CONSOLE_UART_RX_PIN;
    GPIO_InitStruct.Alternate = CONSOLE_UART_RX_AF;
    HAL_GPIO_Init(CONSOLE_UART_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CONSOLE_UART_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(CONSOLE_UART_IRQn);

    /**************************** 其他串口初始化 ****************************/
}

/*******************************************************************************
*
* 函数名  : HAL_UART_MspDeInit
* 负责人  : 彭鹏
* 创建日期: 20150615
* 函数功能: stm32f4 hal uart解初始化回调
*
* 输入参数: stm32 hal uart句柄
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    /*********************** 控制台使用的串口解初始化 ***********************/
    CONSOLE_UART_FORCE_RESET();
    CONSOLE_UART_RELEASE_RESET();

    HAL_GPIO_DeInit(CONSOLE_UART_TX_GPIO_PORT, CONSOLE_UART_TX_PIN);
    HAL_GPIO_DeInit(CONSOLE_UART_RX_GPIO_PORT, CONSOLE_UART_RX_PIN);

    HAL_NVIC_DisableIRQ(CONSOLE_UART_IRQn);

    /*************************** 其他串口解初始化 ***************************/
}

/* TODO: 完善注释 */
void HAL_PCD_MspInit(PCD_HandleTypeDef *s_pcd_handle)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO for HS on FS mode*/
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_14 |GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure VBUS Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    /* Enable USB HS Clocks */
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 4, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *s_pcd_handle)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    /* Disable USB FS Clocks */
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
    __HAL_RCC_SYSCFG_CLK_DISABLE();
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
}

