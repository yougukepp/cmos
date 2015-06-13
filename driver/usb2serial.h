#ifndef _USB_2_SERIAL_H_
#define _USB_2_SERIAL_H_

#include "usbd_ioreq.h"
#include "usbd_ctlreq.h"

#define TEMPLATE_EPIN_ADDR              (0x81)
#define TEMPLATE_EPIN_SIZE              (0x10)

#define USB2SERIAL_CONFIG_DESC_SIZ    (64)

extern USBD_ClassTypeDef  Usb2Serial;

#endif  /* _USB_2_SERIAL_H_ */
