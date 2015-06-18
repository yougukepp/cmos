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

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "usb_descriptor.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
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
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

cmos_uint8_T *usb_hs_descriptor_get_lang_id_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

cmos_uint8_T *usb_hs_descriptor_get_manufacture_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

cmos_uint8_T *usb_hs_descriptor_get_product_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

cmos_uint8_T *usb_hs_descriptor_get_serial_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

cmos_uint8_T *usb_hs_descriptor_get_configuration_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

cmos_uint8_T *usb_hs_descriptor_get_interface_str_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

#if (USBD_LPM_ENABLED == 1)
cmos_uint8_T *usb_hs_descriptor_get_bos_descriptor(USBD_SpeedTypeDef speed, cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}
#endif  

