/********************************************************************************
*
* 文件名  ： uart.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150615
* 版本号  ： v1.0
* 文件描述： stm32f4 cube hal 初始化回调头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
#ifndef _STM32F4XX_HAL_MSP_H_
#define _STM32F4XX_HAL_MSP_H_

#include "cmos_config.h"
#include "stm32f4xx_hal_conf.h"
//#include "stm32f429i_discovery.h"

#define CONSOLE_UART                           USART1
#define CONSOLE_UART_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define CONSOLE_UART_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define CONSOLE_UART_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define CONSOLE_UART_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define CONSOLE_UART_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for CONSOLE_UART Pins */
#define CONSOLE_UART_TX_PIN                    GPIO_PIN_9
#define CONSOLE_UART_TX_GPIO_PORT              GPIOA
#define CONSOLE_UART_TX_AF                     GPIO_AF7_USART1
#define CONSOLE_UART_RX_PIN                    GPIO_PIN_10
#define CONSOLE_UART_RX_GPIO_PORT              GPIOA
#define CONSOLE_UART_RX_AF                     GPIO_AF7_USART1

/* Definition for CONSOLE_UART's NVIC */
#define CONSOLE_UART_IRQn                      USART1_IRQn
#define CONSOLE_UART_IRQHandler                USART1_IRQHandler

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _STM32F4XX_HAL_MSP_H_ */

