/******************************************************************************
 *
 * 文件名  ： mutex.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151127 
 * 版本号  ： v1.0
 * 文件描述： 互斥锁实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其 它   :  TODO: 实现优先级队列
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"
#include "mem.h"
#include "misc.h"
#include "mutex.h"
#include "task.h"
#include "kernel.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void lock(cmos_fd_mutex_T *mutex, cmos_bool_T spin);
static void unlock(cmos_fd_mutex_T *mutex, cmos_bool_T spin);
static void work(cmos_task_tcb_T *data, cmos_fd_mutex_compare_para_T *para);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_malloc
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
cmos_fd_mutex_T *cmos_fd_mutex_malloc(void)
{ 
    cmos_fd_mutex_T *mutex = NULL;

    mutex = cmos_malloc(sizeof(cmos_fd_mutex_T));
    cmos_assert(NULL != mutex, __FILE__, __LINE__);

    mutex->highest_blocked_tcb = NULL;
    cmos_lib_list_init(&(mutex->blocked_tcb_list));

    return mutex;
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_lock
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
inline void cmos_fd_mutex_lock(cmos_fd_mutex_T *mutex)
{ 
    lock(mutex, FALSE);
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_lock_spin
* 负责人  : 彭鹏
* 创建日期: 20160108
* 函数功能: 互斥加锁 自旋
*
* 输入参数: 互斥量指针
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void cmos_fd_mutex_lock_spin(cmos_fd_mutex_T *mutex)
{
    lock(mutex, TRUE);
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_unlock
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
inline void cmos_fd_mutex_unlock(cmos_fd_mutex_T *mutex)
{ 
    unlock(mutex, FALSE);
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_unlock_spin
* 负责人  : 彭鹏
* 创建日期: 20160108
* 函数功能: 互斥解锁 自旋
*
* 输入参数: 互斥量指针
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void cmos_fd_mutex_unlock_spin(cmos_fd_mutex_T *mutex)
{
    unlock(mutex, TRUE);
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_spin
* 负责人  : 彭鹏
* 创建日期: 20160108
* 函数功能: 自旋
*
* 输入参数: 互斥量指针
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_fd_mutex_spin(cmos_fd_mutex_T *mutex)
{
    cmos_task_tcb_T *tcb = cmos_task_self();

    /* 自旋 */
    /* 等待高优先级的任务解锁 */
    while(tcb != mutex->highest_blocked_tcb);
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_free
* 负责人  : 彭鹏
* 创建日期: 20151222
* 函数功能: 释放互斥锁占用的内存
*
* 输入参数: mutex 互斥锁
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 立即返回
*
******************************************************************************/
inline void cmos_fd_mutex_free(cmos_fd_mutex_T *mutex)
{
    cmos_assert(NULL != mutex, __FILE__, __LINE__); 
    cmos_free(mutex);
    mutex = NULL;
}

/*******************************************************************************
*
* 函数名  : work
* 负责人  : 彭鹏
* 创建日期: 20151223
* 函数功能: 供cmos_fd_mutex_unlock中 获取链表中最高优先级任务使用
*
* 输入参数: data tcb结点
*           para 定制化参数
*
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void work(cmos_task_tcb_T *tcb, cmos_fd_mutex_compare_para_T *para)
{
    cmos_assert(NULL != tcb, __FILE__, __LINE__); 
    cmos_assert(NULL != para, __FILE__, __LINE__); 
    cmos_priority_T cur_priority = cmos_priority_err;

    cur_priority = cmos_task_tcb_get_priority(tcb);
    if(cur_priority > para->priority)
    {
        para->highest_tcb = tcb;
    }
}

/*******************************************************************************
*
* 函数名  : cmos_fd_mutex_get_highest_blocked_tcb
* 负责人  : 彭鹏
* 创建日期: 20151223
* 函数功能: 获取mutex中阻塞的最高优先级tcb
*
* 输入参数: mutex 锁
*
* 输出参数: 获取的tcb指针
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline cmos_task_tcb_T *cmos_fd_mutex_get_highest_blocked_tcb(const cmos_fd_mutex_T *mutex)
{
    cmos_assert(NULL != mutex, __FILE__, __LINE__); 

    return mutex->highest_blocked_tcb;
}

/*******************************************************************************
*
* 函数名  : lock
* 负责人  : 彭鹏
* 创建日期: 20151217
* 函数功能: 互斥加锁
*
* 输入参数: mutex 互斥量指针
*           spin  是否自旋
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : 使用highest_blocked_tcb判断是否锁定 若无该tcb则未锁定
*           svc中 运行与内核 不需要显示互斥访问(关中断)
*
******************************************************************************/
static void lock(cmos_fd_mutex_T *mutex, cmos_bool_T spin)
{
    cmos_assert(NULL != mutex, __FILE__, __LINE__); 

    cmos_task_tcb_T *tcb = NULL;
    cmos_priority_T highest_priority = cmos_priority_err;
    cmos_priority_T curr_priority = cmos_priority_err; 
    
    /* step1: 获取当前任务 */
    tcb = cmos_task_self(); 
    
    if(NULL == mutex->highest_blocked_tcb) /* 未锁定 */
    { 
        mutex->highest_blocked_tcb = tcb;  /* 加锁 */
    }
    else /* 已经锁定 需要睡眠 */
    { 
        /* step2: 更新内部数据接口 */
        cmos_assert(NULL != tcb, __FILE__, __LINE__);
        curr_priority = cmos_task_tcb_get_priority(tcb);

        highest_priority = cmos_task_tcb_get_priority(mutex->highest_blocked_tcb);
        if(highest_priority < curr_priority)    /* 当前任务优先级最高 */
        {
            /* step2.1: 将老的最高优先级tcb加入表 */
            cmos_lib_list_push_tail(&(mutex->blocked_tcb_list), mutex->highest_blocked_tcb);
            /* step2.2: 更新最高优先级tcb */
            mutex->highest_blocked_tcb = tcb; 
        }
        else
        {
            cmos_lib_list_push_tail(&(mutex->blocked_tcb_list), tcb);
        }

        /* step3: 阻塞或自旋当前任务 */
        if(TRUE != spin)
        {
            cmos_task_suspend(tcb); /* 阻塞模式则阻塞 */
        } 

        /* svc中自旋会死锁 */
        /* 自旋锁的自旋 在出svc之后做 */
    }
}

/*******************************************************************************
*
* 函数名  : unlock
* 负责人  : 彭鹏
* 创建日期: 20151228
* 函数功能: 解锁
*
* 输入参数: mutex 互斥量指针
*           spin  是否自旋
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : svc中 运行与内核 不需要显示互斥访问(关中断)
*
******************************************************************************/
static void unlock(cmos_fd_mutex_T *mutex, cmos_bool_T spin)
{
    cmos_assert(NULL != mutex, __FILE__, __LINE__);

    /* step1: 获取阻塞的最高优先级任务 */
    cmos_task_tcb_T *next_tcb = mutex->highest_blocked_tcb;

    /* step2: 更新阻塞的最高优先级任务 */
    /* FIXME: 使用链表遍历十分低效 */
    cmos_fd_mutex_compare_para_T compare_para = {cmos_priority_idle, NULL};
    cmos_lib_list_walk(mutex->blocked_tcb_list, (cmos_lib_list_walk_func_T)work, &compare_para); /* 遍历tcb链表 */
    mutex->highest_blocked_tcb = compare_para.highest_tcb; /* 链表空 则可以赋值为空 */ 
    
    /* step3: 恢复唤醒的任务 */ 
    if(FALSE == spin)
    {
        cmos_task_resume(next_tcb);
    }

    /* 此后出关键域 */
}

