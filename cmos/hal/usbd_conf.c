/******************************************************************************
 *
 * 文件名  ： usb_conf.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150617 
 * 版本号  ： 1.0
 * 文件描述： stm32 usb 设备库 与PCD的接口
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "usbd_core.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/* TODO: 完善注释 */

USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
{    
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev,
                                  uint8_t ep_addr,
                                  uint8_t ep_type,
                                  uint16_t ep_mps)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  return USBD_OK; 
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
   return 0;
}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)
{
  return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, 
                                    uint8_t ep_addr,
                                    uint8_t *pbuf,
                                    uint16_t size)
{
  return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, 
                                          uint8_t ep_addr,
                                          uint8_t *pbuf,
                                          uint16_t size)
{
  return USBD_OK;
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
  return 0;
}

void USBD_LL_Delay(uint32_t Delay)
{
}

