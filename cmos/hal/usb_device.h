/********************************************************************************
*
* 文件名  ： usb_device.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150617
* 版本号  ： v1.0
* 文件描述： cmos hal usb 设备
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_USB_DEVICE_H_
#define _CMOS_HAL_USB_DEVICE_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "usbd_def.h"

/************************************ 宏定义 ***********************************/
/* 配置描述符 */
/* 配置描述符长度 */
#define CMOS_USB_HS_CONFIG_DESCRIPTOR_SIZE      (9)
/* 描述符类型 */
#define CMOS_USB_HS_CONFIG_TYPE                 (USB_DESC_TYPE_CONFIGURATION)
/*****************************
 * 设备属性
 * bit7 必须为1
 *
 * bit6 1 自供电
 *      0 总线供电
 * bit5 1 支持远程唤醒
 *      0 不支持远程唤醒:w
 *
 * bit4-bit0 保留
 ****************************/
#define CMOS_USB_HS_BM_ATTRIBUTES               (0xC0)


/* 接口描述符 */
/* 接口描述符长度 */
#define CMOS_USB_HS_INTERFACE_DESCRIPTOR_SIZE   (9)
/* 接口描述符类型 */
#define CMOS_USB_HS_INTERFACE_TYPE              (USB_DESC_TYPE_INTERFACE)
/* bNumEndpoints 非零端点数目 1输入+1输出*/
#define CMOS_USB_HS_ENDPOINTS_NUM               (2)


/* 端点描述符 */ 
/* 端点描述符长度 */
#define CMOS_USB_HS_ENDPOINT_DESCRIPTOR_SIZE    (7)
/* 端点描述符类型 */
#define CMOS_USB_HS_ENDPOINT_TYPE               (USB_DESC_TYPE_ENDPOINT)

/* 输入端点 */
/********************
 * 地址 
 * bit7 1 输入
 *      0 输出
 * bit6-bit4 保留
 * bit3-bit0 端点号
 *******************/
#define CMOS_USB_HS_ENDPOINT_IN_ADDRESS         (0x81)
/********************
 * 属性 
 * 0 控制传输
 * 1 等时传输
 * 2 批量传输
 * 3 中断传输
 *******************/
#define CMOS_USB_HS_ENDPOINT_IN_ATTRIBUTES      (0x03)
/* 最大包长 */
/* 间隔(TODO:批量传输需要修改 因为传输类型不同含义不同) */
#define CMOS_USB_HS_ENDPOINT_IN_MAX_PACKET_SIZE (USB_FS_MAX_PACKET_SIZE)
/* 中断间隔 */
#define CMOS_USB_HS_ENDPOINT_IN_INTERVAL        (0x0A)

/* 输出端点 */
/* 地址 */
#define CMOS_USB_HS_ENDPOINT_OUT_ADDRESS        (0x01)
/* 属性 */
#define CMOS_USB_HS_ENDPOINT_OUT_ATTRIBUTES     (CMOS_USB_HS_ENDPOINT_IN_ATTRIBUTES)
/* 最大包长 */
/* 间隔(TODO:批量传输需要修改 因为传输类型不同含义不同) */
#define CMOS_USB_HS_ENDPOINT_OUT_MAX_PACKET_SIZE (CMOS_USB_HS_ENDPOINT_IN_MAX_PACKET_SIZE)
/* 中断间隔 */
#define CMOS_USB_HS_ENDPOINT_OUT_INTERVAL        (CMOS_USB_HS_ENDPOINT_IN_INTERVAL)


/* 配置描述符集合总长度 */
#define CMOS_USB_HS_CONFIG_DESCRIPTOR_TOTAL_SIZE (CMOS_USB_HS_CONFIG_DESCRIPTOR_SIZE \
        + CMOS_USB_HS_INTERFACE_DESCRIPTOR_SIZE + (CMOS_USB_HS_ENDPOINT_DESCRIPTOR_SIZE * 2))


/*--------------------------------- 接口声明区 --------------------------------*/
extern USBD_ClassTypeDef  g_usb_hs_device;

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_HAL_USB_DEVICE_H_ */

