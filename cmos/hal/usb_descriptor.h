/********************************************************************************
*
* 文件名  ： usb_descriptor.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150617
* 版本号  ： v1.0
* 文件描述： cmos hal usb 设备描述符
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_USB_DESCRIPTOR_H_
#define _CMOS_HAL_USB_DESCRIPTOR_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "usbd_def.h"

/************************************ 宏定义 ***********************************/
/* 设备描述符长度 */
#define CMOS_USB_HS_DEVICE_DESCRIPTOR_SIZE      (18)
/* 描述符类型 */
#define CMOS_USB_HS_DEVICE_DESCRIPTOR_TYPE      (USB_DESC_TYPE_DEVICE)
/* bMaxPacketSize EP0最大包长 */
#define CMOS_USB_HS_MAX_EP0_SIZE                (USB_MAX_EP0_SIZE)
/* bcdUSB USB版本 USB2.0 小端 */
#define CMOS_HS_BCD_USB                         (0x0200)
/* idVender 厂商id  临时一用 */
#define CMOS_HS_VID                             (0x1234)
/* idProduct 产品id 0.1 */
#define CMOS_HS_PID                             (0x0001)
/* bcdDevice rel 0.1 */
#define CMOS_HS_BCD_DEVICE                      (0x0001)
/* 字符串索引 */
#define CMOS_USB_HS_IMANUFACTURER               (0x01)
#define CMOS_USB_HS_IPRODUCT                    (CMOS_USB_HS_IMANUFACTURER + 1)
#define CMOS_USB_HS_ISERIALNUMBER               (0)

/* 语言ID描述符长度 */
#define CMOS_USB_HS_LANG_ID_DESCRIPTOR_SIZE     (4)
/* 字符串描述符类型 */
#define CMOS_USB_HS_STRING_DESCRIPTOR_TYPE      (USB_DESC_TYPE_STRING)
/* 语言ID 美式英语 0x0409 */
#define CMOS_USB_HS_LANG_ID                     (0x0409)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/
extern USBD_DescriptorsTypeDef g_usb_hs_descriptor;

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_HAL_USB_DESCRIPTOR_H_ */

