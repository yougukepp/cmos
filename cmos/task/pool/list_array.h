/******************************************************************************
 *
 * 文件名  ： list_array.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151214 
 * 版本号  ： v1.0
 * 文件描述： tcb链表数组通用接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TASK_POOL_LIST_ARRAY_H_
#define _CMOS_TASK_POOL_LIST_ARRAY_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "list.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/ 
void cmos_task_pool_list_array_set(cmos_lib_list_T *tcb_list_array[], cmos_int32_T index, cmos_lib_list_T *tcb_list);
cmos_lib_list_T *cmos_task_pool_list_array_get(cmos_lib_list_T *tcb_list_array[], cmos_int32_T index);

#endif // #ifndef _CMOS_TASK_POOL_LIST_ARRAY_H_

