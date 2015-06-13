#include "typedef.h"
#include "usbd_core.h"
#include "usbd_conf.h"
#include "usb2serial_desc.h"

#define USB2SERIAL_VID                  1155
#define USB2SERIAL_LANGID_STRING        1033
#define USB2SERIAL_MANUFACTURER_STRING  "pengpeng"
#define USB2SERIAL_PID                  22315
#define USB2SERIAL_PRODUCT_STRING       "pp usb2serial"
#define USB2SERIAL_SERIALNUMBER_STRING  "00000000001A"
#define USB2SERIAL_CONFIGURATION_STRING "pp config"
#define USB2SERIAL_INTERFACE_STRING     "pp Interface"

cm_uint8_t *Usb2Serial_DeviceDescriptor( USBD_SpeedTypeDef speed , uint16_t *length);
cm_uint8_t *Usb2Serial_LangIDStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length);
cm_uint8_t *Usb2Serial_ManufacturerStrDescriptor ( USBD_SpeedTypeDef speed , uint16_t *length);
cm_uint8_t *Usb2Serial_ProductStrDescriptor ( USBD_SpeedTypeDef speed , uint16_t *length);
cm_uint8_t *Usb2Serial_SerialStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length);
cm_uint8_t *Usb2Serial_ConfigStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length);
cm_uint8_t *Usb2Serial_InterfaceStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length);

#ifdef USB_SUPPORT_USER_STRING_DESC
cm_uint8_t *Usb2Serial_USRStringDesc (USBD_SpeedTypeDef speed, cm_uint8_t idx , uint16_t *length);  
#endif

USBD_DescriptorsTypeDef Usb2SerialDesc =
{
  Usb2Serial_DeviceDescriptor,
  Usb2Serial_LangIDStrDescriptor, 
  Usb2Serial_ManufacturerStrDescriptor,
  Usb2Serial_ProductStrDescriptor,
  Usb2Serial_SerialStrDescriptor,
  Usb2Serial_ConfigStrDescriptor,
  Usb2Serial_InterfaceStrDescriptor,
};

__ALIGN_BEGIN cm_uint8_t Usb2Serial_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
  {
    0x12,                       /*bLength */
    USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,          /*bMaxPacketSize*/
    LOBYTE(USB2SERIAL_VID),           /*idVendor*/
    HIBYTE(USB2SERIAL_VID),           /*idVendor*/
    LOBYTE(USB2SERIAL_PID),           /*idVendor*/
    HIBYTE(USB2SERIAL_PID),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
  } ; 
/* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
__ALIGN_BEGIN cm_uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
     USB_LEN_LANGID_STR_DESC,         
     USB_DESC_TYPE_STRING,       
     LOBYTE(USB2SERIAL_LANGID_STRING),
     HIBYTE(USB2SERIAL_LANGID_STRING), 
};

__ALIGN_BEGIN cm_uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

/**
* @brief  Usb2Serial_DeviceDescriptor 
*         return the device descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_DeviceDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  *length = sizeof(Usb2Serial_DeviceDesc);
  return Usb2Serial_DeviceDesc;
}

/**
* @brief  Usb2Serial_LangIDStrDescriptor 
*         return the LangID string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_LangIDStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  *length =  sizeof(USBD_LangIDDesc);  
  return USBD_LangIDDesc;
}

/**
* @brief  Usb2Serial_ProductStrDescriptor 
*         return the product string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_ProductStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  if(speed == 0)
  {   
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_PRODUCT_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_PRODUCT_STRING, USBD_StrDesc, length);    
  }
  return USBD_StrDesc;
}

/**
* @brief  Usb2Serial_ManufacturerStrDescriptor 
*         return the manufacturer string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_ManufacturerStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  USBD_GetString ((cm_uint8_t *)USB2SERIAL_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

/**
* @brief  Usb2Serial_SerialStrDescriptor 
*         return the serial number string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_SerialStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  if(speed  == USBD_SPEED_HIGH)
  {    
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_SERIALNUMBER_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_SERIALNUMBER_STRING, USBD_StrDesc, length);    
  }
  return USBD_StrDesc;
}

/**
* @brief  Usb2Serial_ConfigStrDescriptor 
*         return the configuration string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_ConfigStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  if(speed  == USBD_SPEED_HIGH)
  {  
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_CONFIGURATION_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_CONFIGURATION_STRING, USBD_StrDesc, length); 
  }
  return USBD_StrDesc;  
}

/**
* @brief  Usb2Serial_InterfaceStrDescriptor 
*         return the interface string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
cm_uint8_t *  Usb2Serial_InterfaceStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  if(speed == 0)
  {
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_INTERFACE_STRING, USBD_StrDesc, length);
  }
  else
  {
    USBD_GetString ((cm_uint8_t *)USB2SERIAL_INTERFACE_STRING, USBD_StrDesc, length);
  }
  return USBD_StrDesc;  
}

