/******************************************************************************
 *
 * 文件名  ： uart.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150531 
 * 版本号  ： v1.0
 * 文件描述： 串口底层输入输出
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>
#include <string.h>
#include "cmos_config.h"

#include "stm32f4xx_hal_conf.h"
#include "misc.h"
#include "uart.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
extern UART_HandleTypeDef UartHandle;
extern __IO ITStatus UartReady;

/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
char txBuf[10] = "txabc123\n";
char rxBuf[3];


static void uart_send(char *buf, int n);
static char printf_buf[UART_PRINTF_BUF_LEN] = {0};
//static int s_uart_len = 0;
//static char uart_buf[UART_BUF_LEN] = {0};

int uart_printf(char *fmt, ...)
{
    va_list args;
    int n = 0;
   
    va_start(args, fmt);
    n = vsprintf(printf_buf, fmt, args);
    va_end(args);
	
    //strcpy(uart_buf, printf_buf);
	  //s_uart_len += n;

    uart_send(printf_buf, n);

    return n;
}

static void uart_send(char *buf, int n)
{
	if(HAL_UART_Transmit(&UartHandle, (uint8_t*)buf, n, n/UART_TIMEOUT_DIV)!= HAL_OK)
	{
		Error_Handler();
	}	
	
	#if 0
  if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)buf, n)!= HAL_OK)
  {
    Error_Handler();
  }
  while (UartReady != SET)
  {
  }
  UartReady = RESET;
	#endif
}

	
void uart_test(void)
{ 
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)rxBuf, 1) != HAL_OK)
  {
    Error_Handler();
  }
  while (UartReady != SET)
  {
  }
  UartReady = RESET;
}

