/******************************************************************************
 *
 * 文件名  ： printf.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： v1.0
 * 文件描述： 使用ITM打印输出
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/
#pragma import(__use_no_semihosting_swi)
/************************************ 头文件 ***********************************/
#include "stm32f4xx.h"
#include <stdio.h>

#include "typedef.h"
/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : printf
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 使用ITM打印输出
 *
 * 输入参数: str待打印的内容
 *
 * 输出参数: 无
 *
 * 返回值:   无
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
/*void printf(const cm_uint8_t *str)
{
    return;
}*/

struct __FILE {
int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
	{
	return (ITM_SendChar(ch));
}

volatile int32_t ITM_RxBuffer=0x5AA55AA5; // Initialize as EMPTY
int fgetc(FILE *f) {
char tmp;
while (ITM_CheckChar()==0); // Wait if buffer is empty
tmp = ITM_ReceiveChar();
if (tmp==13) tmp = 10;
return (ITM_SendChar(tmp));
}

void _sys_exit(int return_code) {
label: goto label; /* endless loop */
	
}
