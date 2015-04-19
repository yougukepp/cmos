/******************************************************************************
 *
 * 文件名  ： wait.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416 
 * 版本号  ： v1.0
 * 文件描述： 实现CMSIS中延迟(Generic Wait Functions)
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "wait.h"
#include "switch.h"
#include "tcb.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static cm_bool_t in_isr(void);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
cm_status_t syscall_delay(cm_uint32_t m_sec)
{
    cm_tcb_t *cur = NULL; 
    
    /* TODO: 判断是否在中断中 */
    if(in_isr())
    {
        return cm_Error_ISR;
    }

    /* 获取当前线程 */
    cur = switch_get_running_tcb();
    tcb_set_delay(cur, m_sec);

    /* 通知switch模块处理RUNNING线程 */
    switch_running_to_waiting();

    /* 调度 */
    switch_pend();

    return cm_OK;
}

static cm_bool_t in_isr(void)
{
    return FALSE;
}

