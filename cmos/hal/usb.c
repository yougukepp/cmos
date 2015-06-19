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
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
}


