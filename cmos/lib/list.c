/******************************************************************************
 *
 * 文件名  ： list.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030
 * 版本号  ： v1.0
 * 文件描述： 数据结构 链表的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "list.h"
#include "mem.h"
#include "console.h"

#ifdef _X64_PC_
#include "pc.h"
#endif

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_lib_list_node_T *cmos_lib_list_get_head(const cmos_lib_list_T *list);
static cmos_lib_list_node_T *cmos_lib_list_get_tail(const cmos_lib_list_T *list);
static cmos_status_T cmos_lib_list_add(cmos_lib_list_T **list, cmos_lib_list_node_T *node, cmos_int32_T flag);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_init
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: list指针的指针
 *
 * 输入参数: 头结点
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_lib_list_init(cmos_lib_list_T **list, cmos_lib_list_node_T *node)
{
    if((NULL == node)
    || (NULL == list))
    {
        CMOS_ERR_STR("NULL pointer");
        return;
    }

    *list = node;

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_destroy
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: list析构
 *
 * 输入参数: list指针
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_lib_list_destroy(cmos_lib_list_T *list)
{
    cmos_status_T status = cmos_ERR_E;
    if(NULL == list)
    {
        return cmos_NULL_E;
    }

    /* TODO:遍历树list每个结点 */
    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_node_malloc
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 新建list结点
 *
 * 输入参数: data 数据域指针
 * 输出参数: 无
 *
 * 返回值  : 新建的list结点指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_lib_list_node_T *cmos_lib_list_node_malloc(const void *data)
{
    cmos_lib_list_node_T *node = NULL;

    node = cmos_malloc(sizeof(cmos_lib_list_node_T));
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_list_node_malloc malloc failed.");
        return NULL;
    }

    node->prev = NULL;
    node->next = NULL;
    node->data = (void *)data;

    return node;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_node_free
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: 释放list结点
 *
 * 输入参数: node 结点指针
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_lib_list_node_free(cmos_lib_list_node_T *node)
{
    node->prev = NULL;
    node->next = NULL;
    node->data = NULL;

    cmos_free(node);
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_node_get_data
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 获取结点
 *
 * 输入参数: node 结点指针
 * 输出参数: 无
 *
 * 返回值  : 结点数据域指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void *cmos_lib_list_node_get_data(const cmos_lib_list_node_T *node)
{
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_list_node_get_data can not with null node pointer.");
        return NULL;
    }

    return node->data;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_add
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: list中加入结点
 *
 * 输入参数: list 链表指针的指针
 *           node 待增加的结点指针
 *           flag 0表示尾部加 1表示头部加
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T cmos_lib_list_add(cmos_lib_list_T **list, cmos_lib_list_node_T *node, cmos_int32_T flag)
{
    cmos_lib_list_node_T *head = NULL;
    cmos_lib_list_node_T *tail = NULL;

    if((NULL == list)
    || (NULL == *list)
    || (NULL == node)
    || (flag > 1)
    || (flag < 0))
    {
        CMOS_ERR_STR("cmos_lib_list_add with err para.");
        return cmos_PARA_E;
    }

    if(0 == flag) /* append */
    { 
        /* step1: 找链表尾 */ 
        tail = cmos_lib_list_get_tail(*list); 
        
        /* step2: 链入 */
        tail->next = node;
        node->prev = tail;
    }
    else /* insert */
    {
        /* step1: 找链表头 */ 
        head = cmos_lib_list_get_head(*list);

        /* step2: 链入 */
        node->next = head;
        head->prev = node;

        /* step3: 更换头结点 */
        *list = node;
    }

    return cmos_OK_E;

}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_append
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: list中尾加入结点
 *
 * 输入参数: list 链表指针的指针
 *           node 待增加的结点指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_lib_list_append(cmos_lib_list_T **list, cmos_lib_list_node_T *node)
{
    return cmos_lib_list_add(list, node, 0);
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_insert
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: list中头加入结点
 *
 * 输入参数: list 链表指针的指针
 *           node 待增加的结点指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_lib_list_insert(cmos_lib_list_T **list, cmos_lib_list_node_T *node)
{
    return cmos_lib_list_add(list, node, 1);
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_del
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: list中删除结点
 *
 * 输入参数: list 链表头
 *           data 数据域指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : TODO:其他地方释放
 *
 ******************************************************************************/
cmos_status_T cmos_lib_list_del(cmos_lib_list_T **list, const cmos_lib_list_node_T *node)
{
    cmos_lib_list_node_T *go_ptr = NULL;
    cmos_lib_list_node_T *prev = NULL; /* go_ptr 前驱 */
    cmos_lib_list_node_T *next = NULL; /* go_ptr 后继 */

    if((NULL == list)
    || (NULL == *list)
    || (NULL == node))
    {
        CMOS_ERR_STR("cmos_lib_list_get_tail can not with null pointer.");
        return cmos_NULL_E;
    } 
    
    go_ptr = (cmos_lib_list_node_T *)(*list);
    do
    { 
        if(go_ptr == node) /* 找到,处理go_ptr结点 */
        {
            prev = go_ptr->prev;
            next = go_ptr->next; 
            
            /* list只有一个结点 go_ptr */
            if((NULL == prev)
            && (NULL == next))
            { 
                *list = NULL;
                break;
            } 
            
            /* go_ptr是头结点 */
            if(NULL == prev)
            {
                *list = (cmos_lib_list_T *)next;
                next->prev = NULL;
                break;
            } 
            
            /* go_ptr是尾巴 */
            if(NULL == next)
            {
                prev->next = NULL;
                break;
            }

            /* go_ptr是中间结点 */
            prev->next = next;
            next->prev = prev; 
            break;
        }
        else /* 继续向后找 */
        { 
            go_ptr = go_ptr->next;
        }
    }while(NULL != go_ptr);

    if(NULL == go_ptr)
    { 
        cmos_debug_log("cmos_lib_list_del not find valid node.\n");
        return cmos_PARA_E;
    }
    else
    {
        /* 消除引用 */
        go_ptr->prev = NULL;
        go_ptr->next = NULL;
        return cmos_OK_E;
    }

}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_get_head
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 查找list中链表头结点
 *
 * 输入参数: list 链表
 * 输出参数: 无
 *
 * 返回值  : 链表头结点
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_lib_list_node_T *cmos_lib_list_get_head(const cmos_lib_list_T *list)
{
    return (cmos_lib_list_node_T *)list;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_get_tail
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 查找list中链表尾结点
 *
 * 输入参数: list 链表
 * 输出参数: 无
 *
 * 返回值  : 链表尾巴
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_lib_list_node_T *cmos_lib_list_get_tail(const cmos_lib_list_T *list)
{
    cmos_lib_list_node_T *go_ptr = NULL;
    if(NULL == list)
    {
        CMOS_ERR_STR("cmos_lib_list_get_tail can not with null list.");
        return NULL;
    } 
    
    go_ptr = (cmos_lib_list_node_T *)list;
    while(NULL != go_ptr->next)
    {
        go_ptr = go_ptr->next;
    }

    return go_ptr;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_search_by_data
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 查找list中数据域为data的结点
 *
 * 输入参数: list 链表
 *           data 数据域指针
 * 输出参数: 无
 *
 * 返回值  : 带查找的结点指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_lib_list_node_T *cmos_lib_list_search_by_data(const cmos_lib_list_T *list, const void *data)
{
    cmos_lib_list_node_T *go_ptr = NULL;

    go_ptr = (cmos_lib_list_node_T *)list;
    while(NULL != go_ptr)
    {
        if(data == go_ptr->data) /* 找到 */
        {
            return go_ptr;
        }
        go_ptr = go_ptr->next;
    }

    /* 未找到 */
    return NULL;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_walk
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: 遍历list
 *
 * 输入参数: list 链表头
 *           func 遍历函数
 *           para 遍历函数参数
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_lib_list_walk(cmos_lib_list_T *list, cmos_lib_list_walk_func_T func, void *para)
{
    cmos_lib_list_node_T *go_node = list;
    if((NULL == list)
    || (NULL == func))
    {
        CMOS_TRACE_STR("cmos_lib_list_walk with null pointer.");
        return;
    }

    while(NULL != go_node)
    {
        func(go_node, para); /* 处理本节点 */

        go_node = go_node->next; /* 下一结点 */
    }
}

