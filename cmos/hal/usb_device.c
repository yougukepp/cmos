/******************************************************************************
 *
 * 文件名  ： usb_device.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150617 
 * 版本号  ： 1.0
 * 文件描述： cmos hal usb 设备
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "usb_device.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
const cmos_uint8_T s_usb_hs_config_descriptor[CMOS_USB_HS_CONFIG_DESCRIPTOR_TOTAL_SIZE] = /* 总长度 9 + 9 + 7 + 7 */
{
    /* 配置描述符 */
    CMOS_USB_HS_CONFIG_DESCRIPTOR_SIZE,         /* bLength 配置描述符 */
    CMOS_USB_HS_CONFIG_DESCRIPTOR_TYPE,         /* bDescriptorType 配置描述符类型 */
    LOBYTE(CMOS_USB_HS_CONFIG_DESCRIPTOR_TOTAL_SIZE), /* wTotalLength 配置描述符集合总长 */
    HIBYTE(CMOS_USB_HS_CONFIG_DESCRIPTOR_TOTAL_SIZE),
    1,                                          /* bNumIntergace 该配置支持的接口数 */
    1,                                          /* bCOnfigurationValue 该配置的索引 */ 
    0,                                          /* iConfiguration 该配置的字符串索引 */
    CMOS_USB_HS_BM_ATTRIBUTES,                  /* bmAttributes 该设备属性 */
    0,                                          /* bMaxPower 设备需要的电流 */

    /* 接口描述符 */
    CMOS_USB_HS_INTERFACE_DESCRIPTOR_SIZE,      /* bLength 接口描述符 */
    CMOS_USB_HS_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType 接口描述符类型 */
    0,                                          /* bInterfaceNumber 该接口的编号 从0开始 */
    0,                                          /* bAlternateSetting 该接口备用编号 */
    CMOS_USB_HS_ENDPOINTS_NUM,                  /* bNumEndpoints 非零端点数目 1输入+1输出*/
    0,                                          /* bInterfaceClass 自定义类 */
    0,                                          /* bInterfaceSubClass 自定义类 */
    0,                                          /* bInterfaceProtocol 自定义接口协议 */
    0,                                          /* iConfiguration 字符串索引 0 */

    /* 输入端点描述符 */
    CMOS_USB_HS_ENDPOINT_DESCRIPTOR_SIZE,       /* bLength 端点描述符 */
    CMOS_USB_HS_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType 端点描述符类型 */
    CMOS_USB_HS_ENDPOINT_IN_ADDRESS,            /* bEndpointAddress 端点地址 */
    CMOS_USB_HS_ENDPOINT_IN_ATTRIBUTES,         /* bmAttributes 端点属性 */
    LOBYTE(CMOS_USB_HS_ENDPOINT_IN_MAX_PACKET_SIZE),/* wMaxPacketSize 端点最大包长 */
    HIBYTE(CMOS_USB_HS_ENDPOINT_IN_MAX_PACKET_SIZE),
    CMOS_USB_HS_ENDPOINT_IN_INTERVAL,           /* bInterval 中断间隔 */

    /* 输出端点描述符 */
    CMOS_USB_HS_ENDPOINT_DESCRIPTOR_SIZE,       /* bLength 端点描述符 */
    CMOS_USB_HS_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType 端点描述符类型 */
    CMOS_USB_HS_ENDPOINT_OUT_ADDRESS,           /* bEndpointAddress 端点地址 */
    CMOS_USB_HS_ENDPOINT_OUT_ATTRIBUTES,        /* bmAttributes 端点属性 */
    LOBYTE(CMOS_USB_HS_ENDPOINT_OUT_MAX_PACKET_SIZE),/* wMaxPacketSize 端点最大包长 */
    HIBYTE(CMOS_USB_HS_ENDPOINT_OUT_MAX_PACKET_SIZE),
    CMOS_USB_HS_ENDPOINT_OUT_INTERVAL,           /* bInterval 中断间隔 */
};

/* 结构体中有回调 故申明需要提前 */
static cmos_uint8_T usb_hs_device_init (USBD_HandleTypeDef *pdev, cmos_uint8_T cfgidx);
static cmos_uint8_T usb_hs_device_deInit (USBD_HandleTypeDef *pdev, cmos_uint8_T cfgidx);
static cmos_uint8_T usb_hs_device_setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static cmos_uint8_T *usb_hs_device_get_cfg_desc(cmos_uint16_T *length);
static cmos_uint8_T *usb_hs_device_get_device_qualifier_desc(cmos_uint16_T *length);
static cmos_uint8_T usb_hs_device_data_in(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum);
static cmos_uint8_T usb_hs_device_data_out(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum);
static cmos_uint8_T usb_hs_device_ep0_rx_ready(USBD_HandleTypeDef *pdev);
static cmos_uint8_T usb_hs_device_ep0_tx_ready(USBD_HandleTypeDef *pdev);
static cmos_uint8_T usb_hs_device_sof(USBD_HandleTypeDef *pdev);
static cmos_uint8_T usb_hs_device_iso_in_incomplete(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum);
static cmos_uint8_T usb_hs_device_iso_out_incomplete(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum);

USBD_ClassTypeDef  g_usb_hs_device = {
    usb_hs_device_init,
    usb_hs_device_deInit,
    usb_hs_device_setup,
    usb_hs_device_ep0_tx_ready,  
    usb_hs_device_ep0_rx_ready,
    usb_hs_device_data_in,
    usb_hs_device_data_out,
    usb_hs_device_sof,
    usb_hs_device_iso_in_incomplete,
    usb_hs_device_iso_out_incomplete,      
    usb_hs_device_get_cfg_desc,
    usb_hs_device_get_cfg_desc,
    usb_hs_device_get_cfg_desc,
    usb_hs_device_get_device_qualifier_desc,
};

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
static cmos_uint8_T usb_hs_device_init (USBD_HandleTypeDef *pdev, cmos_uint8_T cfgidx)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_deInit (USBD_HandleTypeDef *pdev, cmos_uint8_T cfgidx)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}


static cmos_uint8_T usb_hs_device_setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T *usb_hs_device_get_cfg_desc(cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);





    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T *usb_hs_device_get_device_qualifier_desc(cmos_uint16_T *length)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_data_in(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_data_out(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_ep0_rx_ready(USBD_HandleTypeDef *pdev)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_ep0_tx_ready(USBD_HandleTypeDef *pdev)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_sof(USBD_HandleTypeDef *pdev)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_iso_in_incomplete(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

static cmos_uint8_T usb_hs_device_iso_out_incomplete(USBD_HandleTypeDef *pdev, cmos_uint8_T epnum)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

