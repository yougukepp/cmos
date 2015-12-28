/******************************************************************************
 *
 * 文件名  ： syscall_ipc.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： ipc系统调用
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "syscall.h"
#include "syscall_ipc.h"

#include "misc.h"
#include "cortex.h"
#include "mutex.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
inline static void cmos_ipc_before(cmos_ipc_type_T type, void *para);
inline static void cmos_ipc_after(cmos_ipc_type_T type, void *para);

/* syscall.s中定义 */
void svc_ipc(cmos_ipc_type_T type, void *para);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_ipc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ipc
 *
 * 输入参数: type ipc类型
 *           para 与type有关的第二参数
 *
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_ipc(cmos_ipc_type_T type, void *para)
{ 
    cmos_assert(cmos_ipc_err != type, __FILE__, __LINE__);

    cmos_ipc_before(type, para);
    svc_ipc(type, para);
    cmos_ipc_after(type, para);

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_ipc_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ipc 特权代码
 *
 * 输入参数: 与cmos_ipc一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_ipc_svc(cmos_ipc_type_T type, void *para)
{
    switch(type)
    {
        case cmos_ipc_interrupt_enable:
            { 
                cmos_hal_cortex_cortex_enable_interrupt();
                break;
            }
        case cmos_ipc_interrupt_disable:
            {
                cmos_hal_cortex_cortex_disable_interrupt();
                break;
            }
        case cmos_ipc_switch_enable:
            {
                cmos_hal_cortex_cortex_enable_switch();
                break;
            }
        case cmos_ipc_switch_disable:
            {
                cmos_hal_cortex_cortex_disalbe_switch();
                break;
            }
        case cmos_ipc_mutex_lock:
            { 
                cmos_assert(NULL != para, __FILE__, __LINE__);
                cmos_fd_mutex_lock(para);
                break;
            }
        case cmos_ipc_mutex_unlock:
            {
                cmos_assert(NULL != para, __FILE__, __LINE__);
                cmos_fd_mutex_unlock(para);
                break;
            }

        default:
            {
                cmos_assert(FALSE, __FILE__, __LINE__);
                break;
            }
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_ipc_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ipc 进入特权代码之前的代码
 *
 * 输入参数: 与cmos_ipc一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_ipc_before(cmos_ipc_type_T type, void *para)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_ipc_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ipc 出特权代码之后的代码
 *
 * 输入参数: 与cmos_ipc一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_ipc_after(cmos_ipc_type_T type, void *para)
{
}

