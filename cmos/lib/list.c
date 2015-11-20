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
static cmos_lib_list_node_T *cmos_lib_list_get_tail(const cmos_lib_list_T *head);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
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
 * 输入参数: head 链表头
 *           data 数据域指针
 * 输出参数: 无
 *
 * 返回值  : 新建的list结点指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_lib_list_add(cmos_lib_list_T *head, const void *data)
{
    cmos_lib_list_node_T *node = NULL;
    cmos_lib_list_node_T *tail = NULL;

    if(NULL == head)
    {
        CMOS_ERR_STR("cmos_lib_list_add can not with null list.");
        return cmos_NULL_E;
    }

    /* step1: 分配结点 */ 
    node = cmos_lib_list_node_malloc(data);

    /* step2: 找链表尾 */ 
    tail = cmos_lib_list_get_tail(head);

    /* step3: 链入 */
    tail->next = node;
    node->prev = tail;

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_get_tail
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 查找list中链表尾巴
 *
 * 输入参数: head 链表头
 * 输出参数: 无
 *
 * 返回值  : 链表尾巴
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_lib_list_node_T *cmos_lib_list_get_tail(const cmos_lib_list_T *head)
{
    cmos_lib_list_node_T *go_ptr = NULL;
    cmos_lib_list_node_T *next_ptr = NULL;
    if(NULL == head)
    {
        CMOS_ERR_STR("cmos_lib_list_get_tail can not with null list.");
        return NULL;
    } 
    
    go_ptr = (cmos_lib_list_node_T *)head;
    next_ptr = go_ptr->next;
    while(NULL != next_ptr)
    {
        go_ptr = next_ptr;
        next_ptr = go_ptr->next;
    }

    return go_ptr;
}

