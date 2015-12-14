/******************************************************************************
 *
 * 文件名  ： blocked.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151211 
 * 版本号  ： v1.0
 * 文件描述： 阻塞任务实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 每个阻塞类型一个tcb链表
 *            注意:该模块运行域svc中断中 不需要锁
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "console.h"
#include "blocked.h"
#include "ready.h"
#include "list.h"
#include "list_array.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void dec_delay(cmos_task_tcb_T *tcb, void *para);

/********************************** 变量实现区 *********************************/
static cmos_lib_list_T *s_blocked_tcb[CMOS_TASK_POOL_BLOCKED_TYPE_MAX] = {NULL};

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_blocked_add
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: 将任务加入到阻塞链表
 *
 * 输入参数: tcb  任务控制块指针
 *           type 阻塞类型
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_pool_blocked_add(const cmos_task_tcb_T *tcb, cmos_blocked_type_T type)
{ 
    cmos_status_T status = cmos_ERR_E;
    cmos_lib_list_T *tcb_list = NULL;

    /* step1: 获取对应的阻塞链表 */
    tcb_list = cmos_task_pool_list_array_get(s_blocked_tcb, type);

    /* step2: 将tcb加入该链表 */
    status = cmos_lib_list_push_tail(&tcb_list, tcb); /* 加入到该优先级链表 */
    if(cmos_OK_E != status)
    {
        CMOS_ERR_STR("cmos_lib_list_push_tail failed.");
        return status;
    }

    /* 处理边界条件: tcb是该阻塞链表首个任务 */
    if(NULL == cmos_task_pool_list_array_get(s_blocked_tcb, type))
    { 
        cmos_task_pool_list_array_set(s_blocked_tcb, type, tcb_list);
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_blocked_add
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: tick定时到 更新相关数据结构
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_pool_blocked_update_tick(void)
{
    cmos_lib_list_T *delay_list = s_blocked_tcb[cmos_blocked_delay_E]; 
    cmos_lib_list_walk_func_T func = (cmos_lib_list_walk_func_T)dec_delay;
    
    /* 遍历延迟链表 自减tick 为零的加入就绪链表 */
    cmos_lib_list_walk(delay_list, func, NULL);
}

/*******************************************************************************
 *
 * 函数名  : dec_delay
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: 遍历链表的工具函数
 *
 * 输入参数: tcb    任务控制块
 *           para   额外参数(未使用)
 * 输出参数: 无
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void dec_delay(cmos_task_tcb_T *tcb, void *para)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_lib_list_T *delay_list = s_blocked_tcb[cmos_blocked_delay_E]; 

    /* 自减延迟值 */
    cmos_task_tcb_dec_delay_ms(tcb);

    /* 延迟结束移出阻塞表 移入就绪表 */
    if(cmos_task_tcb_zero_delay_ms(tcb))
    {
        status = cmos_lib_list_del_by_data(&delay_list, tcb);
        if(cmos_OK_E != status)
        {
            CMOS_ERR_STR("cmos_lib_list_del_by_data err.");
            return;
        }

        status = cmos_task_pool_ready_add(tcb);
        if(cmos_OK_E != status)
        {
            CMOS_ERR_STR("cmos_lib_list_del_by_data err.");
            return;
        }
    }
}

