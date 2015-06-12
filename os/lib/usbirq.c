/******************************************************************************
 *
 * 文件名  ： usbirq.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150612 
 * 版本号  ： v1.0
 * 文件描述： usb中断服务(ISR)定义
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 所有新增ISR都在该文件中定义
 *            函数名需要与startup_stm32f429xx.s保持一致,定义格式如下：
 *            void ISR_NAME_IRQHandler(void)
 *            {
 *            }
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

/**
* @brief This function handles USB On The Go HS global interrupt.
*/
void OTG_HS_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(OTG_HS_IRQn);
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
}


