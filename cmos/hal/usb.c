/******************************************************************************
 *
 * 文件名  ： usb.c
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
#include "cmos_config.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "lib.h"
#include "usb.h"
#include "usbd_core.h"
#include "usb_device.h"
#include "usb_descriptor.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
USBD_HandleTypeDef s_usb_hs_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : usb_init
* 负责人  : 彭鹏
* 创建日期: 20150617
* 函数功能: cmos hal usb 初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T usb_init(void)
{

    USBD_StatusTypeDef status = USBD_FAIL;

    status = USBD_Init(&s_usb_hs_handle, &g_usb_hs_descriptor, CMOS_HAL_USB_DEVICE_HS);
    if(USBD_OK != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    status = USBD_RegisterClass(&s_usb_hs_handle, &g_usb_hs_device); 
    if(USBD_OK != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    status = USBD_Start(&s_usb_hs_handle);
    if(USBD_OK != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : USB_OTG_HS_Handler
 * 负责人  : 彭鹏
 * 创建日期：20150619
 * 函数功能: USB OTG HS ISR
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : USB OTG HS中断调用
 *
 ******************************************************************************/
void USB_OTG_HS_Handler(void)
{
#define PRINTF_OTG_HS_ISR
	
    cmos_uint32_T *addr = NULL;
  	cmos_uint8_T byte1 = 0;
	  cmos_uint8_T byte2 = 0;
	  cmos_uint8_T byte3 = 0;
	  cmos_uint8_T byte4 = 0;	
#if 0
    cmos_uint8_T val = 0;
    cmos_int32_T i = 0;
    cmos_int32_T j = 0;
    cmos_int32_T k = 0;
    cmos_uint32_T int_val = 0;
	
    int_val = (g_pcd_handle.Instance->GINTSTS) & (g_pcd_handle.Instance->GINTMSK);
    console_printf("int_val=0x%08x:", int_val);
#endif
	
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);

    HAL_NVIC_ClearPendingIRQ(OTG_HS_IRQn);
    HAL_PCD_IRQHandler(&g_pcd_handle);

#ifdef PRINTF_OTG_HS_ISR
    addr = g_pcd_handle.Setup;
    if(!(addr[0] | addr[1]))
    {
        goto END;
    }

    byte1 = (cmos_uint8_T)((addr[0] & 0x000000FF));
    byte2 = (cmos_uint8_T)((addr[0] & 0x0000FF00) >> 8);
    byte3 = (cmos_uint8_T)((addr[0] & 0x00FF0000) >> 16);
    byte4 = (cmos_uint8_T)((addr[0] & 0xFF000000) >> 24);

    console_printf("0x%02x,0x%02x:", byte1, byte2);
    if(0x80 & byte1)
    {
        console_printf("主机=>设备,");
    }
    else
    {
        console_printf("设备=>主机,");
    }

    if(0x06 == byte2)
    {
        if(0x01 == byte4)
        {
            console_printf("请求设备描述符.");
        }
        else if(0x02 == byte4)
        {
            console_printf("请求配置描述符.");
        }
        else if(0x03 == byte4)
        {
            console_printf("请求字符串描述符.");
        }
        else
        {
            console_printf("byte4=0x%02x未实现.", byte4);
        }
    }
    else if(0x05 == byte2)
    {
        console_printf("设置地址.");
    }
    else
    {
        console_printf("byte2=0x%02x未实现.", byte2);
    }

    console_printf("0x%08x,0x%08x.", addr[0], addr[1]);
    console_printf("\r\n");

END:
#endif

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
}

