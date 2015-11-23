/******************************************************************************
 *
 * 文件名  ： 
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： 
 * 文件描述： 
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "list.h"
#include "tcb_list.h"
#include "console.h"


/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_list_malloc_node
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 新建tcb_list结点
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 新建的tcb_list结点指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_task_tcb_list_node_T *cmos_task_tcb_list_malloc_node(const cmos_task_tcb_T *tcb)
{
    cmos_lib_list_node_T *list_node = NULL;
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_malloc_node with null tcb pointer.");
        return NULL;
    }

    list_node = cmos_lib_list_node_malloc((const void *)tcb);
    if(NULL == list_node)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_malloc_node malloc cmos_task_tcb_list_node failed.");
        return NULL;
    }

    return list_node;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_list_add
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 将tcb加入tcb链表
 *
 * 输入参数: list 任务链表
 *           tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_tcb_list_add(cmos_task_tcb_list_T *list, const cmos_task_tcb_T *tcb)
{
    cmos_status_T status = cmos_ERR_E;

    if(NULL == list)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_add can not with null list.");
        return cmos_NULL_E;
    }

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_add can not with null tcb.");
        return cmos_NULL_E;
    }

    status = cmos_lib_list_add(list, tcb);
    if(cmos_OK_E != status)
    {
        return status;
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_list_del
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: 删除list中的tcb结点
 *
 * 输入参数: list 任务链表
 *           tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_tcb_list_del(cmos_task_tcb_list_T *list, const cmos_task_tcb_T *tcb)
{
    cmos_status_T status = cmos_ERR_E;

    if(NULL == list)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_add can not with null list.");
        return cmos_NULL_E;
    }

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_add can not with null tcb.");
        return cmos_NULL_E;
    }

    status = cmos_lib_list_del(list, tcb);
    if(cmos_OK_E != status)
    {
        return status;
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_list_get_head_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 获取tcb_list首结点的任务控制块指针
 *
 * 输入参数: head tcb_list链表
 * 输出参数: 无
 *
 * 返回值  : 任务控制块指针
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_task_tcb_T *cmos_task_tcb_list_get_head_tcb(const cmos_task_tcb_list_T *list)
{
    cmos_task_tcb_list_node_T *head_node = NULL;
    cmos_task_tcb_T *tcb = NULL;

    if(NULL == list)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_get_head_tcb can not with null list.");
        return NULL;
    }

    /* list指向头结点 */
    head_node = (cmos_task_tcb_list_node_T *)list;

    tcb = cmos_lib_list_node_get_data(head_node);

    return tcb;
}

