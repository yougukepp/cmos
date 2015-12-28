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
#ifndef _CMOS_FD_MUTEX_H_
#define _CMOS_FD_MUTEX_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"
#include "list.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef struct{ 
    cmos_task_tcb_T *highest_blocked_tcb; /* 阻塞的最高优先级tcb */
    cmos_lib_list_T *blocked_tcb_list; /* 阻塞tcb链表 */
}cmos_fd_mutex_T;

typedef struct{
        cmos_priority_T priority; /* 遍历过程中 当前最高优先级 输入 */
        cmos_task_tcb_T *highest_tcb; /* 最高优先级结果 输出 */
}cmos_fd_mutex_compare_para_T; /* mutex 模块专用 */

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_fd_mutex_T *cmos_fd_mutex_malloc(void);
void cmos_fd_mutex_free(cmos_fd_mutex_T *mutex);

void cmos_fd_mutex_lock(cmos_fd_mutex_T *mutex);
void cmos_fd_mutex_unlock(cmos_fd_mutex_T *mutex);

void cmos_fd_mutex_spin_lock(cmos_fd_mutex_T *mutex);
void cmos_fd_mutex_spin_unlock(cmos_fd_mutex_T *mutex);

/* 设置与获取函数 */
cmos_task_tcb_T *cmos_fd_mutex_get_highest_blocked_tcb(const cmos_fd_mutex_T *mutex);

#endif /* #ifndef _CMOS_FD_MUTEX_H_ */

