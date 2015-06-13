#include "typedef.h"
#include "usb2serial.h"


cm_uint8_t Usb2Serial_Init(USBD_HandleTypeDef *pdev, cm_uint8_t cfgidx);
cm_uint8_t Usb2Serial_DeInit (USBD_HandleTypeDef *pdev, cm_uint8_t cfgidx); 
cm_uint8_t Usb2Serial_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
cm_uint8_t *Usb2Serial_GetCfgDesc (uint16_t *length);
cm_uint8_t *Usb2Serial_GetDeviceQualifierDesc (uint16_t *length);
cm_uint8_t Usb2Serial_DataIn (USBD_HandleTypeDef *pdev, cm_uint8_t epnum);
cm_uint8_t Usb2Serial_DataOut (USBD_HandleTypeDef *pdev, cm_uint8_t epnum);
cm_uint8_t Usb2Serial_EP0_RxReady (USBD_HandleTypeDef *pdev);
cm_uint8_t Usb2Serial_EP0_TxReady (USBD_HandleTypeDef *pdev);
cm_uint8_t Usb2Serial_SOF (USBD_HandleTypeDef *pdev);
cm_uint8_t Usb2Serial_IsoINIncomplete (USBD_HandleTypeDef *pdev, cm_uint8_t epnum);
cm_uint8_t Usb2Serial_IsoOutIncomplete (USBD_HandleTypeDef *pdev, cm_uint8_t epnum);

USBD_ClassTypeDef Usb2Serial = 
{
  Usb2Serial_Init,
  Usb2Serial_DeInit,
  Usb2Serial_Setup,
  Usb2Serial_EP0_TxReady,  
  Usb2Serial_EP0_RxReady,
  Usb2Serial_DataIn,
  Usb2Serial_DataOut,
  Usb2Serial_SOF,
  Usb2Serial_IsoINIncomplete,
  Usb2Serial_IsoOutIncomplete,      
  Usb2Serial_GetCfgDesc,
  Usb2Serial_GetCfgDesc, 
  Usb2Serial_GetCfgDesc,
  Usb2Serial_GetDeviceQualifierDesc,
};

static cm_uint8_t Usb2Serial_CfgDesc[USB2SERIAL_CONFIG_DESC_SIZ] =
{
  0x09, /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION, /* bDescriptorType: Configuration */
  USB2SERIAL_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x02,         /*iConfiguration: Index of string descriptor describing the configuration*/
  0xC0,         /*bmAttributes: bus powered and Supports Remote Wakeup */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
  /* 09 */
  
  /**********  Descriptor of TEMPLATE interface 0 Alternate setting 0 **************/  
 
};
  
/* USB Standard Device Descriptor */
cm_uint8_t Usb2Serial_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @brief  Usb2Serial_Init
  *         Initialize the TEMPLATE interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
cm_uint8_t  Usb2Serial_Init (USBD_HandleTypeDef *pdev, 
                               cm_uint8_t cfgidx)
{
  cm_uint8_t ret = 0; 
  USBD_DbgLog("%s", __func__);
  

  return ret;
}

/**
  * @brief  Usb2Serial_Init
  *         DeInitialize the TEMPLATE layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
cm_uint8_t  Usb2Serial_DeInit (USBD_HandleTypeDef *pdev, 
                                 cm_uint8_t cfgidx)
{

  USBD_DbgLog("%s", __func__);
  return USBD_OK;
}

/**
  * @brief  Usb2Serial_Setup
  *         Handle the TEMPLATE specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
cm_uint8_t  Usb2Serial_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req)
{
 
  USBD_DbgLog("%s", __func__);
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
      
    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL; 
    }
    //break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    
    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL;     
    }
  }
  return USBD_OK;
}


/**
  * @brief  Usb2Serial_GetCfgDesc 
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
cm_uint8_t  *Usb2Serial_GetCfgDesc (uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  *length = sizeof (Usb2Serial_CfgDesc);
  return Usb2Serial_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
cm_uint8_t  *Usb2Serial_DeviceQualifierDescriptor (uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  *length = sizeof (Usb2Serial_DeviceQualifierDesc);
  return Usb2Serial_DeviceQualifierDesc;
}


/**
  * @brief  Usb2Serial_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
cm_uint8_t  Usb2Serial_DataIn (USBD_HandleTypeDef *pdev, 
                              cm_uint8_t epnum)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}

/**
  * @brief  Usb2Serial_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
cm_uint8_t  Usb2Serial_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}
/**
  * @brief  Usb2Serial_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
cm_uint8_t  Usb2Serial_EP0_TxReady (USBD_HandleTypeDef *pdev)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}
/**
  * @brief  Usb2Serial_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
cm_uint8_t  Usb2Serial_SOF (USBD_HandleTypeDef *pdev)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}
/**
  * @brief  Usb2Serial_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
cm_uint8_t  Usb2Serial_IsoINIncomplete (USBD_HandleTypeDef *pdev, cm_uint8_t epnum)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}
/**
  * @brief  Usb2Serial_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
cm_uint8_t  Usb2Serial_IsoOutIncomplete (USBD_HandleTypeDef *pdev, cm_uint8_t epnum)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}
/**
  * @brief  Usb2Serial_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
cm_uint8_t  Usb2Serial_DataOut (USBD_HandleTypeDef *pdev, 
                              cm_uint8_t epnum)
{
  USBD_DbgLog("%s", __func__);

  return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
cm_uint8_t  *Usb2Serial_GetDeviceQualifierDesc (uint16_t *length)
{
  USBD_DbgLog("%s", __func__);
  *length = sizeof (Usb2Serial_DeviceQualifierDesc);
  return Usb2Serial_DeviceQualifierDesc;
}

