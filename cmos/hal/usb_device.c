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

