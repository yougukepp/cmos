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

#include "usbd_conf.h"
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

static void pp_PCD_IRQHandler(PCD_HandleTypeDef *hpcd);

/**
* @brief This function handles USB On The Go HS global interrupt.
*/
void OTG_HS_IRQHandler(void)
{
  HAL_NVIC_ClearPendingIRQ(OTG_HS_IRQn);
  //HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
  pp_PCD_IRQHandler(&hpcd_USB_OTG_HS);
}

static void pp_PCD_IRQHandler(PCD_HandleTypeDef *hpcd)
{
	#if 0
  USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
  uint32_t i = 0, ep_intr = 0, epint = 0, epnum = 0;
  uint32_t fifoemptymsk = 0, temp = 0;
  USB_OTG_EPTypeDef *ep;
	#endif
	
  USB_OTG_GlobalTypeDef *USBx = NULL;

	USBx = hpcd_USB_OTG_HS.Instance;

  uint32_t v = 0;  
  v = USBx->GINTSTS;
  v &= USBx->GINTMSK;

  /* ensure that we are in device mode */
  if (USB_GetMode(hpcd->Instance) != USB_OTG_MODE_DEVICE)
  {
      USBD_DbgLog("1");
      return;
  }

  /* avoid spurious interrupt */
  if(__HAL_PCD_IS_INVALID_INTERRUPT(hpcd)) 
  {
      USBD_DbgLog("2");
      return;
  }

  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_MMIS))
  {
      USBD_DbgLog("3");
      /* incorrect mode, acknowledge the interrupt */
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_MMIS);
  }

  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OEPINT))
  {
      USBD_DbgLog("4");
      ;
  }

  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IEPINT))
  {
      USBD_DbgLog("5");
      ;
  }

  /* Handle Resume Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT))
  {    
      USBD_DbgLog("6");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_WKUINT);
  }

  /* Handle Suspend Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP))
  {
      USBD_DbgLog("7");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBSUSP);
  }

#ifdef USB_OTG_GLPMCFG_LPMEN
  /* Handle LPM Interrupt */ 
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_LPMINT))
  {
      USBD_DbgLog("8");
      ;
  }
#endif /* USB_OTG_GLPMCFG_LPMEN */

  /* Handle Reset Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_USBRST))
  {
      USBD_DbgLog("9");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_USBRST);
  }

  /* Handle Enumeration done Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE))
  {
      USBD_DbgLog("10");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_ENUMDNE);
  }

  /* Handle RxQLevel Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_RXFLVL))
  {
      USBD_DbgLog("11");
  }

  /* Handle SOF Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SOF))
  {
      USBD_DbgLog("12");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SOF);
  }

  /* Handle Incomplete ISO IN Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR))
  {
      USBD_DbgLog("13");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_IISOIXFR);
  }

  /* Handle Incomplete ISO OUT Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT))
  {
      USBD_DbgLog("14");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_PXFR_INCOMPISOOUT);
  }

  /* Handle Connection event Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT))
  {
      USBD_DbgLog("15");
      __HAL_PCD_CLEAR_FLAG(hpcd, USB_OTG_GINTSTS_SRQINT);
  }

  /* Handle Disconnection event Interrupt */
  if(__HAL_PCD_GET_FLAG(hpcd, USB_OTG_GINTSTS_OTGINT))
  {
		  uint32_t temp = 0;
      USBD_DbgLog("16");
		  temp = hpcd->Instance->GOTGINT;
      hpcd->Instance->GOTGINT |= temp;
  }
}

