/******************************************************************************
 *
 * 文件名  ： semaphore.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151127 
 * 版本号  ： v1.0
 * 文件描述： 信号量接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_IPC_SEMAPHORE_H_
#define _CMOS_IPC_SEMAPHORE_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb_list.h"
/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef struct cmos_ipc_semaphore_T_tag{
    cmos_int32_T count;                 /* 信号了计数 */
    cmos_priority_T priority;           /* 信号量的优先级 */
    cmos_task_tcb_list_T *tcb_list;     /* 等待信号量的任务列表 */

    cmos_int32_T *bit_flag;             /* 使用bit-band实现互斥 */
}cmos_ipc_semaphore_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_ipc_semaphore_malloc(cmos_ipc_semaphore_T *semaphore, cmos_int32_T init_count);
cmos_status_T cmos_ipc_semaphore_P(cmos_ipc_semaphore_T *semaphore);
cmos_status_T cmos_ipc_semaphore_V(cmos_ipc_semaphore_T *semaphore);
cmos_status_T cmos_ipc_semaphore_free(cmos_ipc_semaphore_T *semaphore);

#endif /* #ifndef _CMOS_IPC_SEMAPHORE_H_ */

