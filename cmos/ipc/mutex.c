/******************************************************************************
 *
 * 文件名  ： mutex.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151127 
 * 版本号  ： v1.0
 * 文件描述： 互斥锁实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "mem.h"
#include "mutex.h"
#include "task.h"
#include "console.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_ipc_mutex_malloc
* 负责人  : 彭鹏
* 创建日期: 20151217
* 函数功能: 分配一个互斥量
*
* 输入参数: 波特率和串口号
* 输出参数: 无
*
* 返回值  : 互斥量指针
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_ipc_mutex_T *cmos_ipc_mutex_malloc(void)
{ 
    cmos_ipc_mutex_T *mutex = NULL;

    mutex = cmos_malloc(sizeof(cmos_ipc_mutex_T));
    if(NULL == mutex)
    {
        CMOS_ERR_STR("cmos_malloc failed.");
        return NULL;
    }
    mutex->lock = CMOS_IPC_MUTEX_UNLOCKED;
    mutex->highest_blocked_tcb = NULL;

    cmos_lib_list_init(&(mutex->blocked_tcb_list));


    return mutex;
}

/*******************************************************************************
*
* 函数名  : cmos_ipc_mutex_lock
* 负责人  : 彭鹏
* 创建日期: 20151217
* 函数功能: 互斥加锁
*
* 输入参数: 互斥量指针
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_ipc_mutex_lock(cmos_ipc_mutex_T *mutex)
{
    cmos_task_tcb_T *tcb = NULL;
    cmos_uint32_T rst = 0;

    rst = __LDREXW(&(mutex->lock));
    if(CMOS_IPC_MUTEX_LOCKED == rst) /* 已有任务锁定 */
    {
        goto suspend;
    }

    rst = __STREXW(CMOS_IPC_MUTEX_LOCKED, &(mutex->lock));
    if(0 != rst) /* 已有任务锁定 */
    {
        goto suspend;
    }

    /* 正常 */
    /* 此后为关键域 */
    return;

    /* 阻塞 */
suspend:
    /* FIXME: 如何保证此处的互斥？*/
    /* TODO: 保存task_id到 以&mutex作为键的链表 便于unlock的恢复 */
    mutex->highest_blocked_tcb = NULL;
    &(mutex->blocked_tcb_list);

    tcb = cmos_task_self();
    cmos_task_suspend(tcb);

}

/*******************************************************************************
*
* 函数名  : cmos_ipc_mutex_unlock
* 负责人  : 彭鹏
* 创建日期: 20151217
* 函数功能: 互斥解锁
*
* 输入参数: 互斥量指针
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_ipc_mutex_unlock(cmos_ipc_mutex_T *mutex)
{
    /* TODO: 恢复已&mutex作为键的链表中的一个任务 */
    /*cmos_task_resume(task_id); */

    mutex->lock = CMOS_IPC_MUTEX_UNLOCKED;
    /* 此后出关键域 */
}

