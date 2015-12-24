/******************************************************************************
 *
 * 文件名  ： status.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： status系统调用
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "status.h"
#include "task.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
inline static void cmos_status_before(void);
inline static void cmos_status_after(void);

/* syscall.s中定义 */
void svc_status(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_status
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_status
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_status(void)
{ 
    cmos_status_before();
    svc_status();
    cmos_status_after();
}

/*******************************************************************************
 *
 * 函数名  : cmos_status_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_status 特权代码
 *
 * 输入参数: 与cmos_status一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_status_svc(void)
{ 
    /* 获取当前任务 */
    cmos_task_tcb_T *s_current_tcb = cmos_task_self();
    if(NULL == s_current_tcb)
    {
        return cmos_RUNNING_E;
    }
    return cmos_SINGLE_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_status_before
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_status 进入特权代码之前的代码
 *
 * 输入参数: 与cmos_status一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_status_before(void)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_status
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_status 出特权代码之后的代码
 *
 * 输入参数: 与cmos_status一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_status_after(void)
{
}

