/******************************************************************************
 *
 * 文件名  ： api.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： 1.1
 * 文件描述： 封装CMOS内部各模块 对应用层提供统一接口
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"
#include "hal.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : syscall_c
 * 负责人  : 彭鹏
 * 创建日期：20150409 
 * 函数功能: 系统调用C主逻辑
 *
 * 输入参数: sp任务堆栈
 *
 * 输出参数: 无
 *
 * 返回值  : 调用状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void syscall_c(cmos_uint32_T *sp)
{
    cmos_uint32_T stacked_r0 = 0;
    cmos_uint32_T stacked_r1 = 0;

    cmos_uint8_T svc_number = 0;

    svc_number = ((cmos_uint8_T *) sp[6])[-2];
    stacked_r0 = sp[0];
    stacked_r1 = sp[1];

    /***************************************************************************
     *
     *  系统调用编号规则:
     *  - 系统调用编号1 Byte.
     *  - 高四位指示系统调用类别()
     *  - 低四位指示系统调用类别中的不同调用 
     *    目前已经定义的系统调用号：
     *      0xa 驱动系统调用(利用Linux VFS思想)
     *        0xa1 cmos_open
     *        0xa2 cmos_close
     *        0xa3 cmos_read
     *        0xa4 cmos_write
     *        0xa5 cmos_ioctl
     *
     **************************************************************************/
    switch(svc_number)
    {
        case 0xa1:
            {
                break;
            }
        case 0xa2:
            {
                break;
            }
        case 0xa3:
            {
                break;
            }
        case 0xa4:
            {
                break;
            }
        case 0xa5:
            {
                break;
            }
        default:
            {
                assert_failed(__FILE__, __LINE__);
                break;
            }
    }

    return;
}

#if 0
cmos_int32_T cmos_hal_uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...);
cmos_int32_T cmos_hal_uart_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_hal_uart_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes);
cmos_status_T cmos_hal_uart_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...);
cmos_status_T cmos_hal_uart_close(cmos_int32_T dev_id);
#endif

/*******************************************************************************
*
* 函数名  : cmos_init
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: CMOS 初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无需汇编实现
*
******************************************************************************/
cmos_status_T cmos_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* cmos hal 硬件底层初始化 */
    status = hal_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    /* 尽早初始化控制台便于打印 所以放在这里而没有放在hal_init函数执行之后 */
    status = cmos_console_init(CMOS_CONSOLE_BAUDRATE);
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }
    /* 后面的初始化可以使用控制台输出了 */

    /* TODO:进入非特权级别 */
    return cmos_OK_E;
}

