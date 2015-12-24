/******************************************************************************
 *
 * 文件名  ： syscall_task.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： task相关系统调用
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "syscall_task.h"
#include "misc.h"
#include "task.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
inline static void cmos_create_before(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute);
inline static void cmos_create_after(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute);

inline static void cmos_delay_before(cmos_int32_T millisec);
inline static void cmos_delay_after(cmos_int32_T millisec);

/* syscall.s中定义 */
void svc_create(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute);
void svc_delay(cmos_int32_T millisec);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_create
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_create
 *
 * 输入参数: task_attribute 任务入口 任务参数 任务属性 堆栈 优先级 等
 * 输出参数: task_id 任务id号
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_create(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute)
{ 
    cmos_assert(NULL != task_id, __FILE__, __LINE__);
    cmos_assert(NULL != task_attribute, __FILE__, __LINE__);

    cmos_create_before(task_id, task_attribute);
    svc_create(task_id, task_attribute);
    cmos_create_after(task_id, task_attribute);

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_create_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_create 特权代码
 *
 * 输入参数: 与cmos_create一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_create_svc(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute)
{
    cmos_assert(NULL != task_id, __FILE__, __LINE__);
    cmos_assert(NULL != task_attribute, __FILE__, __LINE__);

    cmos_task_create(task_id, task_attribute);

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_create_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_create 进入特权代码之前的代码
 *
 * 输入参数: 与cmos_create一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_create_before(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_create_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_create 出特权代码之后的代码
 *
 * 输入参数: 与cmos_create一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_create_after(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_delay
 * 负责人  : 彭鹏
 * 创建日期：20151224
 * 函数功能: delay系统调用
 *
 * 输入参数: millisec 延迟时间(CMOS_TICK_TIMES)数
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : CMOS_TICK_TIMES一般以ms为单位 该函数延迟任务millisec毫秒
 *
 ******************************************************************************/
void cmos_delay(cmos_int32_T millisec)
{ 
    cmos_assert(0 < millisec, __FILE__, __LINE__);

    cmos_delay_before(millisec);
    svc_delay(millisec);
    cmos_delay_after(millisec);

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_delay_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: delay系统调用 特权代码
 *
 * 输入参数: 与cmos_delay一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_delay_svc(cmos_int32_T millisec)
{
    cmos_task_delay(millisec);
}

/*******************************************************************************
 *
 * 函数名  : cmos_delay_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 特权代码执行前的用户态代码
 *
 * 输入参数: 与cmos_delay一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_delay_before(cmos_int32_T millisec)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_delay_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 特权代码执行后的用户态代码
 *
 * 输入参数: 与cmos_delay一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_delay_after(cmos_int32_T millisec)
{
}

