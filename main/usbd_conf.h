#ifndef _USBD_CONF_H_
#define _USBD_CONF_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "uart.h"

#define USBD_MAX_NUM_INTERFACES               1
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100
#define USBD_SUPPORT_USER_STRING              0 
#define USBD_SELF_POWERED                     1

#define DEVICE_FS                             0
#define DEVICE_HS                             1

 /* Memory management macros */   
//#define USBD_malloc               malloc
//#define USBD_free                 free
//#define USBD_memset               memset
//#define USBD_memcpy               memcpy

/* DEBUG macros */
#define USBD_DEBUG_LEVEL           3
#if (USBD_DEBUG_LEVEL > 0)
#define  USBD_UsrLog(...)   uart_printf(__VA_ARGS__);\
                            uart_printf("\n\r");
#else
#define USBD_UsrLog(...)   
#endif 
                            
                            
#if (USBD_DEBUG_LEVEL > 1)

#define  USBD_ErrLog(...)   uart_printf("ERROR: ") ;\
                            uart_printf(__VA_ARGS__);\
                            uart_printf("\n\r");
#else
#define USBD_ErrLog(...)   
#endif 
                            
                            
#if (USBD_DEBUG_LEVEL > 2)                         
#define  USBD_DbgLog(...)   uart_printf("DEBUG : ") ;\
                            uart_printf(__VA_ARGS__);\
                            uart_printf("\n\r");
#else
#define USBD_DbgLog(...)                         
#endif

#endif /* _USBD_CONF_H_ */
