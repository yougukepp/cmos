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
#include "misc.h"
#include "mem.h"
#include "console.h"
#include "cmos_api.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 控制台文件句柄 */
/* 仅init(单任务)中写 其他都是读 故无需锁 */
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
void cmos_console_init(cmos_int32_T baud_rate)
{
    s_console_uart_fd = cmos_open(CMOS_CONSOLE_PATH, CMOS_O_RDWR); 
    cmos_assert(-1 != s_console_uart_fd, __FILE__, __LINE__);

    /* FIXME:暂时未实现uart ioctl系统调用 */
    /* 波特率的设置位于 hal/hardware/stm32f429idiscovery_hardware.c */
    /* status = cmos_ioctl(s_console_uart_fd, CMOS_I_SETBAUDRATE, baud_rate); */

    return;
}

/*******************************************************************************
*
* 函数名  : cmos_console_printf
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: 控制台打印
*
* 输入参数: 与printf参数含义一致
* 输出参数: 无
*
* 返回值  : 输出的字节数
* 调用关系: 无
* 其 它   : 有阻塞可能
*
******************************************************************************/
cmos_int32_T cmos_console_printf(char *fmt, ...)
{ 
    if(NULL == fmt) /* 无需打印 */
    {
        return 0;
    }

    char *printf_buf = cmos_malloc(CMOS_PRINTF_BUF_SIZE);
    cmos_assert(NULL != printf_buf, __FILE__, __LINE__);

    /* FIXME: 避免竞态 */
    va_list args;
    cmos_int32_T n = 0;
    cmos_int32_T n_writes = 0;

    va_start(args, fmt); 
    n = vsnprintf(printf_buf, CMOS_PRINTF_BUF_SIZE, fmt, args);
    cmos_assert( (n >= CMOS_PRINTF_BUF_SIZE) /* 出错 */ ||( n< 0), __FILE__, __LINE__);
    va_end(args);

    /* 传输 */
    n_writes = cmos_write(s_console_uart_fd, (cmos_uint8_T *)printf_buf, n); /* 此处肯能会任务切换 */
    cmos_assert( (n_writes != n), __FILE__, __LINE__);

    cmos_free(printf_buf);

    return n;
}

/*******************************************************************************
*
* 函数名  : cmos_console_printf_poll
* 负责人  : 彭鹏
* 创建日期: 20151218
* 函数功能: 控制台打印
*
* 输入参数: 与printf参数含义一致
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 某些情况下打印不可阻塞,如idle任务
*
******************************************************************************/
cmos_int32_T cmos_console_printf_poll(char *fmt, ...)
{ 
    if(NULL == fmt) /* 无需打印 */
    {
        return 0;
    }
    char *printf_buf = cmos_malloc(CMOS_PRINTF_BUF_SIZE);
    cmos_assert(NULL != printf_buf, __FILE__, __LINE__);

    va_list args;
    cmos_int32_T n = 0;
    cmos_int32_T n_writes = 0;

    va_start(args, fmt); 
    n = vsnprintf(printf_buf, CMOS_PRINTF_BUF_SIZE, fmt, args);
    cmos_assert( (n >= CMOS_PRINTF_BUF_SIZE) /* 出错 */ ||( n< 0), __FILE__, __LINE__);
    va_end(args);

    /* 传输 */
    n_writes = cmos_write_poll(s_console_uart_fd, (cmos_uint8_T *)printf_buf, n); /* 此处肯能会任务切换 */
    cmos_assert( (n_writes != n), __FILE__, __LINE__);

    cmos_free(printf_buf);

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

