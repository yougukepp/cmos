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
/* 设备描述符 */
const cmos_uint8_T s_usb_hs_device_descriptor[CMOS_USB_HS_DEVICE_DESCRIPTOR_SIZE] =
{
    CMOS_USB_HS_DEVICE_DESCRIPTOR_SIZE,     /* bLength 设备描述符长度 */
    CMOS_USB_HS_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType 类型为设备描述符 */
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

/* 语言id描述符 */
const cmos_uint8_T s_usb_hs_lang_id_descriptor[CMOS_USB_HS_LANG_ID_DESCRIPTOR_SIZE] =
{
    CMOS_USB_HS_LANG_ID_DESCRIPTOR_SIZE,    /* bLength 语言ID描述符长度 */
    CMOS_USB_HS_STRING_DESCRIPTOR_TYPE,     /* bDescriptorType 字符串描述符类型 */
    LOBYTE(CMOS_USB_HS_LANG_ID),            /* 语言ID */
    HIBYTE(CMOS_USB_HS_LANG_ID)
};

/* 制造商和产品字符串unicode使用python生成 */
/* 制造商字符串描述符 */
const cmos_uint8_T s_usb_hs_manufacture_descriptor[0x2c] = /* */
{
    0x2c,
    CMOS_USB_HS_STRING_DESCRIPTOR_TYPE, 
    0x83,0x00,    //S
    0x84,0x00,    //T
    0x77,0x00,    //M
    0x51,0x00,    //3
    0x50,0x00,    //2
    0x70,0x00,    //F
    0x52,0x00,    //4
    0x32,0x00,    // 
    0x85,0x00,    //U
    0x83,0x00,    //S
    0x66,0x00,    //B
    0x32,0x00,    // 
    0x72,0x00,    //H
    0x83,0x00,    //S
    0x32,0x00,    // 
    0xbe,0x8b,    //设
    0x07,0x59,    //备
    0x68,0x51,    //全
    0x1f,0x90,    //速
    0x21,0x6a,    //模
    0x0f,0x5f     //式
};

/* 产品字符串描述符 */
const cmos_uint8_T s_usb_hs_product_descriptor[0x28] = 
{
    0x28,
    CMOS_USB_HS_STRING_DESCRIPTOR_TYPE,
    0x67,0x00,    //C
    0x77,0x00,    //M
    0x79,0x00,    //O
    0x83,0x00,    //S
    0x32,0x00,    // 
    0x83,0x00,    //S
    0x84,0x00,    //T
    0x77,0x00,    //M
    0x51,0x00,    //3
    0x50,0x00,    //2
    0x70,0x00,    //F
    0x52,0x00,    //4
    0x32,0x00,    // 
    0x85,0x00,    //U
    0x83,0x00,    //S
    0x66,0x00,    //B
    0x32,0x00,    // 
    0x72,0x00,    //H
    0x83,0x00     //S
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
    cmos_uint8_T *ptr_descriptor = NULL;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);

    ptr_descriptor = (cmos_uint8_T *)s_usb_hs_lang_id_descriptor;
    *length = CMOS_USB_HS_LANG_ID_DESCRIPTOR_SIZE;

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return ptr_descriptor;
}

cmos_uint8_T *usb_hs_descriptor_get_manufacture_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_uint8_T *ptr_descriptor = NULL;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);

    ptr_descriptor = (cmos_uint8_T *)s_usb_hs_manufacture_descriptor;
    *length = sizeof(s_usb_hs_manufacture_descriptor);

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return ptr_descriptor;
}

cmos_uint8_T *usb_hs_descriptor_get_product_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_uint8_T *ptr_descriptor = NULL;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);

    ptr_descriptor = (cmos_uint8_T *)s_usb_hs_product_descriptor;
    *length = sizeof(s_usb_hs_product_descriptor);

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return ptr_descriptor;
}

cmos_uint8_T *usb_hs_descriptor_get_serial_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{ 
    /* TODO:stm32 uid 实现 序列号字符串描述符 */
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    *length = 0;
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_configuration_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    *length = 0;
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

cmos_uint8_T *usb_hs_descriptor_get_interface_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    *length = 0;
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}

#if (USBD_LPM_ENABLED == 1)
cmos_uint8_T *usb_hs_descriptor_get_bos_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    *length = 0;
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return NULL;
}
#endif  

