/******************************************************************************
 *
 * 文件名  ： mutex.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151127 
 * 版本号  ： v1.0
 * 文件描述： 互斥锁接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_IPC_MUTEX_H_
#define _CMOS_IPC_MUTEX_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"
#include "list.h"

/************************************ 宏定义 ***********************************/
#define CMOS_IPC_MUTEX_UNLOCKED     (0)
#define CMOS_IPC_MUTEX_LOCKED       (1)

/*********************************** 类型定义 **********************************/
typedef struct{ 
    cmos_uint8_T lock;

    cmos_task_tcb_T *highest_blocked_tcb; /* 阻塞的最高优先级tcb */
    cmos_lib_list_T *blocked_tcb_list; /* 阻塞tcb链表 */
}cmos_ipc_mutex_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_ipc_mutex_T *cmos_ipc_mutex_malloc(void);

void cmos_ipc_mutex_lock(cmos_ipc_mutex_T *mutex);
void cmos_ipc_mutex_unlock(cmos_ipc_mutex_T *mutex);

void cmos_ipc_mutex_spin_lock(cmos_ipc_mutex_T *mutex);
void cmos_ipc_mutex_spin_unlock(cmos_ipc_mutex_T *mutex);

void cmos_ipc_mutex_free(cmos_ipc_mutex_T *mutex);

#endif /* #ifndef _CMOS_IPC_MUTEX_H_ */

