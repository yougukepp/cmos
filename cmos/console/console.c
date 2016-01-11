/******************************************************************************
 *
 * 文件名  ： console.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150614 
 * 版本号  ： 1.0
 * 文件描述： 控制台模块
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： TODO: printf三个版本 想办法重构 合并
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>
#include <stdio.h>
#include "cmos_config.h"
#include "misc.h"
#include "mem.h"
#include "syscall.h"
#include "console.h"
#include "cmos_api.h"

/*----------------------------------- 声明区 ----------------------------------*/
typedef cmos_int32_T (*write_func_T)(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes);

/********************************** 变量声明区 *********************************/

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

    /* step1: 打开文件(处理锁定) */ 
    /* 控制台文件句柄 */
    cmos_fd_T console_uart_fd = (cmos_fd_T)cmos_open(CMOS_CONSOLE_PATH, CMOS_O_RDWR); 
    cmos_assert(0 != console_uart_fd, __FILE__, __LINE__);

    /* step2: 解析可变参数指示的字符串 */
    char *printf_buf = cmos_malloc(CMOS_PRINTF_BUF_SIZE);
    cmos_assert(NULL != printf_buf, __FILE__, __LINE__);

    va_list args;
    cmos_int32_T n = 0;
    cmos_int32_T n_writes = 0;

    va_start(args, fmt); 
    n = vsnprintf(printf_buf, CMOS_PRINTF_BUF_SIZE, fmt, args);
    cmos_assert((0 < n) && (n < CMOS_PRINTF_BUF_SIZE), __FILE__, __LINE__);
    va_end(args);

    n_writes = cmos_write(console_uart_fd, (cmos_uint8_T *)printf_buf, n); /* 此处可能会阻塞 */
    cmos_assert( (n_writes == n), __FILE__, __LINE__);

    cmos_free(printf_buf);
    printf_buf = NULL;

    /* step3: 关闭文件(处理锁定) */
    cmos_close(console_uart_fd);

    return n;
}

/*******************************************************************************
*
* 函数名  : cmos_console_printf_poll
* 负责人  : 彭鹏
* 创建日期: 20160110
* 函数功能: 控制台打印 非阻塞模式
*
* 输入参数: 与printf参数含义一致
* 输出参数: 无
*
* 返回值  : 输出的字节数
* 调用关系: 无
* 其 它   : 无阻塞可能
*
******************************************************************************/
cmos_int32_T cmos_console_printf_poll(char *fmt, ...)
{
    return 0;
}

/*******************************************************************************
*
* 函数名  : cmos_console_uninit
* 负责人  : 彭鹏
* 创建日期: 20151023
* 函数功能: 解除控制台串口的管腿复用
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 函数执行状态
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_console_uninit(void)
{
    return;
}

