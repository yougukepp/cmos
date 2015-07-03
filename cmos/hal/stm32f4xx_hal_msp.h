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
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _STM32F4XX_HAL_MSP_H_
#define _STM32F4XX_HAL_MSP_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "stm32f4xx_hal_conf.h"

/************************************ 宏定义 ***********************************/
/******************************** 控制台串口定义 *******************************/
#define CONSOLE_UART                            USART1
#define CONSOLE_UART_CLK_ENABLE()               __HAL_RCC_USART1_CLK_ENABLE()
#define CONSOLE_UART_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define CONSOLE_UART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define CONSOLE_UART_FORCE_RESET()              __HAL_RCC_USART1_FORCE_RESET()
#define CONSOLE_UART_RELEASE_RESET()            __HAL_RCC_USART1_RELEASE_RESET()
#define CONSOLE_UART_TX_PIN                     GPIO_PIN_9
#define CONSOLE_UART_TX_GPIO_PORT               GPIOA
#define CONSOLE_UART_TX_AF                      GPIO_AF7_USART1
#define CONSOLE_UART_RX_PIN                     GPIO_PIN_10
#define CONSOLE_UART_RX_GPIO_PORT               GPIOA
#define CONSOLE_UART_RX_AF                      GPIO_AF7_USART1
#define CONSOLE_UART_IRQn                       USART1_IRQn
#define CONSOLE_UART_IRQHandler                 USART1_IRQHandler

/********************************** IMU I2C定义 ********************************/
#define IMU_I2C                                 I2C3
#define IMU_I2C_CLOCK_ENABLE()                  __I2C3_CLK_ENABLE()
#define IMU_I2C_FORCE_RESET()                   __I2C3_FORCE_RESET()
#define IMU_I2C_RELEASE_RESET()                 __I2C3_RELEASE_RESET()
#define IMU_I2C_SDA_GPIO_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define IMU_I2C_SCL_GPIO_CLK_ENABLE()           __GPIOA_CLK_ENABLE() 
#define IMU_I2C_SDA_GPIO_CLK_DISABLE()          __GPIOC_CLK_DISABLE()
#define IMU_I2C_SCL_GPIO_CLK_DISABLE()          __GPIOA_CLK_DISABLE() 
#define IMU_I2C_SCL_PIN                         GPIO_PIN_8
#define IMU_I2C_SCL_GPIO_PORT                   GPIOA
#define IMU_I2C_SCL_SDA_AF                      GPIO_AF4_I2C3
#define IMU_I2C_SDA_PIN                         GPIO_PIN_9
#define IMU_I2C_SDA_GPIO_PORT                   GPIOC
#define IMU_I2C_EV_IRQn                         I2C3_EV_IRQn
#define IMU_I2C_ER_IRQn                         I2C3_ER_IRQn


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _STM32F4XX_HAL_MSP_H_ */

