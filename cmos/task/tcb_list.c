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
static void cmos_task_tcb_list_lib_list_walk_func(cmos_lib_list_node_T *lib_list_node,
        cmos_task_tcb_list_walk_func_para_T *para);


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
cmos_status_T cmos_task_tcb_list_add(cmos_task_tcb_list_T **list, const cmos_task_tcb_T *tcb)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_lib_list_node_T *node = NULL;

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

    node = cmos_task_tcb_list_malloc_node(tcb);
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_add malloc node failed.");
        return cmos_PARA_E;
    }

    status = cmos_lib_list_append(list, node);
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
cmos_status_T cmos_task_tcb_list_del(cmos_task_tcb_list_T **list, const cmos_task_tcb_T *tcb)
{
    cmos_status_T status = cmos_ERR_E; 
    cmos_lib_list_node_T *node = NULL;

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

    node = cmos_lib_list_search_by_data(*list, tcb);
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_del search node failed.");
        return cmos_NULL_E;
    }

    status = cmos_lib_list_del(list, node);
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

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_list_walk
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: 遍历tcb_list链表
 *
 * 输入参数: tcb_list 链表
 *           func     遍历函数
 *           para     遍历函数参数
 * 输出参数: 无
 *
 * 返回值  : 任务控制块指针
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_tcb_list_walk(cmos_task_tcb_list_T *tcb_list, cmos_task_tcb_list_walk_func_T func, void *para)
{ 
    if((NULL == tcb_list)
    || (NULL == func))
    {
        CMOS_ERR_STR("cmos_task_tcb_list_walk can not with null pointer.");
        return;
    }

    cmos_task_tcb_list_walk_func_para_T func_and_para =
    {
        .func = func,
        .para = para
    };

    cmos_lib_list_walk((cmos_lib_list_T *)tcb_list,
            (cmos_lib_list_walk_func_T)cmos_task_tcb_list_lib_list_walk_func,
            &func_and_para);
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_list_walk
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: 实现tcb_list到lib_list间walk函数的适配
 *
 * 输入参数: lib_list_node 链表结点
 *           对于tcb的操作函数及其参数
 * 输出参数: 无
 *
 * 返回值  : 任务控制块指针
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static void cmos_task_tcb_list_lib_list_walk_func(cmos_lib_list_node_T *lib_list_node,
        cmos_task_tcb_list_walk_func_para_T *para)
{ 
    cmos_task_tcb_T *tcb = NULL;

    if((NULL == lib_list_node)
    || (NULL == para))
    {
        CMOS_ERR_STR("cmos_task_tcb_list_lib_list_walk_func can not with null pointer.");
        return;
    }

    tcb = cmos_lib_list_node_get_data(lib_list_node);
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_lib_list_walk_func can not with null tcb pointer.");
        return;
    }

    para->func(tcb, para->para);
}

