/******************************************************************************
 *
 * 文件名  ： thread_switch.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150411 
 * 版本号  ： v1.0
 * 文件描述： 线程切换算法的实现
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： TODO:算法描述
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
extern cm_tcb_t g_thread_cb[2];

/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void *thread_switch(const void *cur_stack)
{
    static cm_uint32_t next_id = 0;
    cm_uint32_t *next_psp = NULL; 
    
    next_id++;
    next_id %= 2; 
    
    next_psp = g_thread_cb[next_id].psp; 
    return next_psp;
}

