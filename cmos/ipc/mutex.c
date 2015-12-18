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
* 其 它   : svc(优先级高与外部中断)中执行等效于关中断 故无需要互斥访问
*
******************************************************************************/
void cmos_ipc_mutex_lock(cmos_ipc_mutex_T *mutex)
{
    if(NULL == mutex)
    {
        CMOS_ERR_STR("cmos_ipc_mutex_lock get a null mutex.");
        return;
    }

    cmos_task_tcb_T *tcb = NULL;
    cmos_priority_T highest_priority = cmos_priority_err;
    cmos_priority_T curr_priority = cmos_priority_err;

    if(CMOS_IPC_MUTEX_UNLOCKED == mutex->lock)
    { 
        /* 此后为关键域 */
        return;
    }
    else /* 已经锁定 需要阻塞 */
    {
        /* step1: 获取当前任务 */
        tcb = cmos_task_self(); 
        if(NULL == tcb)
        {
            CMOS_ERR_STR("cmos_ipc_mutex_lock get a null current tcb.");
            return;
        }
        
        /* step2: 更新阻塞tcb链表中最高优先级任务 */
        if(NULL == mutex->highest_blocked_tcb) /* 链表首任务 */
        { 
            mutex->highest_blocked_tcb = tcb; 
        }
        else
        {
            highest_priority = cmos_task_tcb_get_priority(mutex->highest_blocked_tcb);
            curr_priority = cmos_task_tcb_get_priority(tcb);
            if((highest_priority < curr_priority)    /**/
            || (NULL == mutex->highest_blocked_tcb)) /* 链表首任务 */
            {
                mutex->highest_blocked_tcb = tcb; 
            }
        }

        /* step3: 插入阻塞tcb链表 */
        cmos_lib_list_push_tail(&(mutex->blocked_tcb_list), tcb);

        /* step4: 阻塞当前任务 */
        cmos_task_suspend(tcb);
    }
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
* 其 它   : svc(优先级高与外部中断)中执行等效于关中断 故无法互斥访问
*
******************************************************************************/
void cmos_ipc_mutex_unlock(cmos_ipc_mutex_T *mutex)
{
    /* TODO: 恢复已&mutex作为键的链表中的一个任务 */
    /*cmos_task_resume(task_id); */
    if(NULL == mutex)
    {
        CMOS_ERR_STR("cmos_ipc_mutex_unlock get a null mutex.");
        return;
    }

    /* step1: 获取唤醒任务 阻塞列表中最高优先级任务 */
    cmos_task_tcb_T *next_tcb = mutex->highest_blocked_tcb;
    if(NULL == next_tcb)
    {
        CMOS_ERR_STR("cmos_ipc_mutex_unlock get a null highest_blocked_tcb.");
        return;
    }

    /* step2: 唤醒的任务从阻塞列表中删除 */
    cmos_lib_list_del_by_data(&(mutex->blocked_tcb_list), next_tcb); 
    
    /* step3: 恢复唤醒的任务 */ 
    cmos_task_resume(next_tcb);

    /* step4: 解锁 */ 
    mutex->lock = CMOS_IPC_MUTEX_UNLOCKED;
    /* 此后出关键域 */
}

