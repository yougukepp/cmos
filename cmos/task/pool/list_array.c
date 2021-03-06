/******************************************************************************
 *
 * 文件名  ： list_array.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151214 
 * 版本号  ： v1.0
 * 文件描述： tcb链表数组通用接口的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "list_array.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_list_array_set
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: 设置数组中第index个元素的为tcb_list
 *
 * 输入参数: tcb_list_array 链表数组
 *           index          数组索引
 *           tcb_list       欲设置的链表
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_task_pool_list_array_set(cmos_lib_list_T *tcb_list_array[], cmos_int32_T index, cmos_lib_list_T *tcb_list)
{
    tcb_list_array[index] = tcb_list;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_list_array_get
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: 获取数组中第index个元素
 *
 * 输入参数: tcb_list_array 链表数组
 *           index          数组索引
 * 输出参数: 无
 * 返回值  : 获取到的链表
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_lib_list_T *cmos_task_pool_list_array_get(cmos_lib_list_T *tcb_list_array[], cmos_int32_T index)
{
    return tcb_list_array[index];
}

