/******************************************************************************
 *
 * 文件名  ： console.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150614 
 * 版本号  ： 1.0
 * 文件描述： 控制台模块
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>
#include <stdio.h>
#include "cmos_config.h"
#include "lib.h"
#include "console.h"
#include "cmos_api.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 控制台缓存 */
static char s_printf_buf[CMOS_PRINTF_BUF_SIZE] = {0};
/* 控制台文件句柄 */
static cmos_int32_T s_console_uart_fd = 0;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_console_init
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: 控制台初始化
*
* 输入参数: baud_rate 控制台串口的波特率
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T cmos_console_init(cmos_int32_T baud_rate)
{
    cmos_status_T status = cmos_ERR_E;

    s_console_uart_fd = cmos_open("/dev/uart/console", CMOS_O_RDWR);
    if(-1 == s_console_uart_fd)
    {
        assert_failed(__FILE__, __LINE__);
    }

    status = cmos_ioctl(s_console_uart_fd, CMOS_I_SETBAUDRATE, baud_rate);

    return status;
}

/*******************************************************************************
*
* 函数名  : cmos_console_printf
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: 控制台打印
*
* 输入参数: 与printf参数含义一致
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 系统错误或调试打印需要第一时间输出 采用轮询方式输出
*
******************************************************************************/
cmos_int32_T cmos_console_printf(char *fmt, ...)
{ 
    cmos_status_T status = cmos_ERR_E;
    va_list args;
    int n = 0;
   
    va_start(args, fmt);
    n = vsprintf(s_printf_buf, fmt, args);
    va_end(args);

    /* 传输 */
    status = cmos_write(s_console_uart_fd, (cmos_uint8_T)s_printf_buf, n);
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    return n;
}

/*******************************************************************************
*
* 函数名  : cmos_console_uninit
* 负责人  : 彭鹏
* 创建日期: 20151023
* 函数功能: 解除控制台串口的管腿复用
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T cmos_console_uninit(void)
{
    /* FIXME:不使用 预留 */
    cmos_status_T status = cmos_ERR_E;
    status = cmos_close(s_console_uart_fd);

    return status;
}
