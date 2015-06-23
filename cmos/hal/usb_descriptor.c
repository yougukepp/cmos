/******************************************************************************
 *
 * 文件名  ： usb_descriptor.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150617 
 * 版本号  ： 1.0
 * 文件描述： cmos hal usb 设备描述符
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/************************************ 头文件 ***********************************/
#include "usb_descriptor.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
const cmos_uint8_T s_usb_hs_device_descriptor[CMOS_USB_HS_DEVICE_DESCRIPTOR_SIZE] =
{
    CMOS_USB_HS_DEVICE_DESCRIPTOR_SIZE,     /* bLength 设备描述符长度 */
    CMOS_USB_HS_DEVICE_TYPE,                /* bDescriptorType 类型为设备描述符 */
    LOBYTE(CMOS_HS_BCD_USB),                /* bcdUSB USB版本 USB2.0 小端 */
    HIBYTE(CMOS_HS_BCD_USB),
    0x00,                                   /* bDeviceClass 自定义的设备 */
    0x00,                                   /* bDeviceSubClass */
    0x00,                                   /* bDeviceProtocol 自定义协议 */
    CMOS_USB_HS_MAX_EP0_SIZE,               /* bMaxPacketSize EP0最大包长 */
    LOBYTE(CMOS_HS_VID),                    /* idVender 厂商id */
    HIBYTE(CMOS_HS_VID),
    LOBYTE(CMOS_HS_PID),                    /* idProduct 产品id */
    HIBYTE(CMOS_HS_PID),
    LOBYTE(CMOS_HS_BCD_DEVICE),             /* bcdDevice rel 0.1 */
    HIBYTE(CMOS_HS_BCD_DEVICE),
    CMOS_USB_HS_IMANUFACTURER,              /* iManufacturer 厂商字符串索引 */
    CMOS_USB_HS_IPRODUCT,                   /* iProduct 产品字符串索引 */
    CMOS_USB_HS_ISERIALNUMBER,              /* iSerialNumber 序列号字符串索引 */
    0x01                                    /* bNumConfigurations 设备具有的配置数 */
};

/* 结构体中有回调 故申明需要提前 */
cmos_uint8_T *usb_hs_descriptor_get_device_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length);  
cmos_uint8_T *usb_hs_descriptor_get_lang_id_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length); 
cmos_uint8_T *usb_hs_descriptor_get_manufacture_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length);  
cmos_uint8_T *usb_hs_descriptor_get_product_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length);  
cmos_uint8_T *usb_hs_descriptor_get_serial_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length);  
cmos_uint8_T *usb_hs_descriptor_get_configuration_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length);  
cmos_uint8_T *usb_hs_descriptor_get_interface_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length); 
#if (USBD_LPM_ENABLED == 1)
cmos_uint8_T *usb_hs_descriptor_get_bos_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length); 
#endif  

USBD_DescriptorsTypeDef g_usb_hs_descriptor ={
    usb_hs_descriptor_get_device_descriptor,
    usb_hs_descriptor_get_lang_id_str_descriptor,
    usb_hs_descriptor_get_manufacture_str_descriptor,
    usb_hs_descriptor_get_product_str_descriptor,
    usb_hs_descriptor_get_serial_str_descriptor,
    usb_hs_descriptor_get_configuration_str_descriptor,
    usb_hs_descriptor_get_interface_str_descriptor,
#if (USBD_LPM_ENABLED == 1)
    usb_hs_descriptor_get_bos_descriptor,
#endif  
};

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
cmos_uint8_T *usb_hs_descriptor_get_device_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_uint8_T *ptr_descriptor = NULL;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
	
    ptr_descriptor = (cmos_uint8_T *)s_usb_hs_device_descriptor;
    *length = CMOS_USB_HS_DEVICE_DESCRIPTOR_SIZE;

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
	
    return ptr_descriptor;
}

cmos_uint8_T *usb_hs_descriptor_get_lang_id_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_manufacture_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_product_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_serial_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_configuration_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_interface_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

#if (USBD_LPM_ENABLED == 1)
cmos_uint8_T *usb_hs_descriptor_get_bos_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}
#endif  

