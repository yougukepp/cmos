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
#include "misc.h"
#include "console.h"

#ifdef _X64_PC_
#include "pc.h"
#endif

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_lib_list_node_T *node_malloc(const void *data);
static void node_free(cmos_lib_list_node_T *node);
static cmos_lib_list_node_T *list_get_tail(const cmos_lib_list_T *list);
static cmos_lib_list_node_T *list_get_head(const cmos_lib_list_T *list);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_init
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: list指针的指针
 *
 * 输入参数: 链表双重指针
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_lib_list_init(cmos_lib_list_T **list)
{
    if(NULL == list)
    {
        CMOS_ERR_STR("cmos_lib_list_init NULL pointer");
        return;
    }

    *list = NULL;

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_destroy
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: list析构
 *
 * 输入参数: list双重指针
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_lib_list_destroy(cmos_lib_list_T **list)
{
    cmos_assert((NULL != list) && (NULL != *list), (uint8_t *)__FILE__, __LINE__);

    /* TODO:遍历list每个结点 后 */
    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_length
 * 负责人  : 彭鹏
 * 创建日期：20151211 
 * 函数功能: 求list结点个数
 *
 * 输入参数: list指针
 * 输出参数: 无
 *
 * 返回值  : list个数
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_int32_T cmos_lib_list_length(cmos_lib_list_T *list)
{
    cmos_int32_T length = 0;
    if(NULL == list) /* 空表长度为0 */
    {
        return 0;
    }

    cmos_lib_list_node_T *go_ptr = NULL;
    go_ptr = list_get_head(list);
    while(NULL != go_ptr)
    {
        go_ptr = go_ptr->next;
        length++;
    }

    /* 未找到 */
    return length;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_push_tail
 * 负责人  : 彭鹏
 * 创建日期：20151211 
 * 函数功能: 尾部加入
 *
 * 输入参数: list 双重指针
 *           data 待加入结点的数据域
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_lib_list_push_tail(cmos_lib_list_T **list, const void *data)
{
    cmos_lib_list_node_T *node = NULL;
    cmos_lib_list_node_T *tail = NULL;

    cmos_assert(NULL != list, (uint8_t *)__FILE__, __LINE__);

    /* step1: malloc结点 */ 
    node = node_malloc(data);
    cmos_assert(NULL != node, (uint8_t *)__FILE__, __LINE__);

    /* step2: 找到尾部 */ 
    tail = list_get_tail(*list);
    if(NULL == tail) /* 空表 */
    {
        *list = node;
        return;
    }

    /* step3: 操作指针 */ 
    /* node->next = NULL malloc后已经为空 */
    node->prev = tail;
    tail->next = node;
    /* tail->prev 保持不变 */

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_pop_tail
 * 负责人  : 彭鹏
 * 创建日期：20151211 
 * 函数功能: 尾部弹出
 *
 * 输入参数: list 双重指针
 * 输出参数: 无
 *
 * 返回值  : 弹出结点的数据域指针的指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void *cmos_lib_list_pop_tail(cmos_lib_list_T **list)
{
    void *data = NULL;
    cmos_lib_list_node_T *tail = NULL;
    cmos_lib_list_node_T *new_tail = NULL;

    cmos_assert((NULL != list) && (NULL != *list), (uint8_t *)__FILE__, __LINE__);

    /* step1: 找到尾部 */ 
    tail = list_get_tail(*list);

    /* step2: 找到新尾部 */ 
    new_tail = tail->prev;

    /* step3: 操作指针 */ 
    if(NULL == new_tail) /* tail是链表仅有的结点删除该结点链表变空 */
    {
        *list = NULL;
    }
    else
    {
        new_tail->next = NULL;
        /* new_tail->prev; 不变 */
    }

    /* step4: 数据域赋值 */ 
    data = cmos_lib_list_node_get_data(tail);

    /* step5: 释放tail */ 
    node_free(tail);
    tail = NULL;

    return data;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_pop_head
 * 负责人  : 彭鹏
 * 创建日期：20151211 
 * 函数功能: 头部弹出
 *
 * 输入参数: list 双重指针
 *           data 待弹出结点的数据域指针的指针
 * 输出参数: 无
 *
 * 返回值  : 弹出结点的数据域指针的指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void *cmos_lib_list_pop_head(cmos_lib_list_T **list)
{
    void *data = NULL;
    cmos_lib_list_node_T *head = NULL;
    cmos_lib_list_node_T *new_head = NULL;

    cmos_assert((NULL != list) && (NULL != *list), (uint8_t *)__FILE__, __LINE__);

    /* step1: 找到头部 */ 
    head = list_get_head(*list);

    /* step2: 找到新头部 */ 
    new_head = head->next;

    /* step3: 操作指针 */ 
    if(NULL == new_head) /* head是链表仅有的结点删除该结点链表变空 */
    {
        *list = NULL;
    }
    else
    {
        new_head->prev= NULL;
        /* new_head->next; 不变 */
        /* 链表新指针 */
        *list = new_head;
    }

    /* step4: 数据域赋值 */ 
    data = cmos_lib_list_node_get_data(head);

    /* step5: 释放head */ 
    node_free(head);
    head = NULL;

    return data;
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_get_head_data
 * 负责人  : 彭鹏
 * 创建日期：20151213 
 * 函数功能: 获取头结点数据
 *
 * 输入参数: list 链表
 * 输出参数: 无
 *
 * 返回值  : 头结点数据域指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void *cmos_lib_list_get_head_data(const cmos_lib_list_T *list)
{
    cmos_assert(NULL != list, (uint8_t *)__FILE__, __LINE__);

    cmos_lib_list_node_T *go_node = list_get_head(list);

    return cmos_lib_list_node_get_data(go_node);
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
    cmos_assert(NULL != func, (uint8_t *)__FILE__, __LINE__);

    cmos_lib_list_node_T *next = NULL;
    cmos_lib_list_node_T *data = NULL;
    
    if(NULL == list) /* NULL == list 该函数啥也不干 */
    {
        return;
    }

    cmos_lib_list_node_T *go_node = list_get_head(list);
    while(NULL != go_node)
    { 
        /* 注意: 此处需要在调用func前获取next指针 否则func中若删除go_node则逻辑出错 */
        next = go_node->next; /* 下一结点 */
        data = cmos_lib_list_node_get_data(go_node);

        func(data, para); /* 处理本节点数据域 */

        go_node = next;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_lib_list_del_by_data
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: 删除list 数据域为data的结点
 *
 * 输入参数: list 链表指针的指针
 *           data 数据域指针
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 未找到则不删除任何结点
 *
 ******************************************************************************/
void cmos_lib_list_del_by_data(cmos_lib_list_T **list, const void *data)
{
    cmos_lib_list_node_T *prev = NULL;
    cmos_lib_list_node_T *next = NULL;
    void *node_data = NULL;

    cmos_assert((NULL != list), (uint8_t *)__FILE__, __LINE__);

    if((NULL == *list) || (NULL == data))
    {
        return;
    }

    cmos_lib_list_node_T *go_node = list_get_head(*list);
    while(NULL != go_node)
    { 
        node_data = cmos_lib_list_node_get_data(go_node);
        if(node_data == data) /* 找到 */
        {
            prev = go_node->prev;
            next = go_node->next;

            if((NULL == prev) && (NULL == next)) /* list仅有go_node结点 */
            { 
                *list = NULL; /* 链表置空 */
            }
            else if(NULL == next) /* 尾结点 */
            {
                prev->next = NULL; /* 次尾结点边尾结点 */
            }
            else if(NULL == prev) /* 头结点 */
            {
                *list = next; /* 头结点变为下一结点 */
                next->prev = NULL;
            }
            else /* 中间结点 */
            { 
                /* 跳过 go_node */
                prev->next = next;
                next->prev = prev;
            } 

            /* 释放内存 */
            node_free(go_node);
            go_node = NULL; 
            
            return;
        }

        go_node = go_node->next;
    } 

    cmos_assert(FALSE, (uint8_t *)__FILE__, __LINE__);
    return;
}

/*******************************************************************************
 *
 * 函数名  : node_malloc
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
static cmos_lib_list_node_T *node_malloc(const void *data)
{
    cmos_lib_list_node_T *node = NULL;

    node = cmos_malloc(sizeof(cmos_lib_list_node_T));
    cmos_assert(NULL != node, (uint8_t *)__FILE__, __LINE__);

    node->prev = NULL;
    node->next = NULL;
    node->data = (void *)data;

    return node;
}

/*******************************************************************************
 *
 * 函数名  : node_free
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
inline static void node_free(cmos_lib_list_node_T *node)
{
    node->prev = NULL;
    node->next = NULL;
    /* 仅仅释放 指针域 数据域不变 */
    /* node->data = NULL; */
    cmos_free(node);
}

/*******************************************************************************
 *
 * 函数名  : list_get_head
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
inline static cmos_lib_list_node_T *list_get_head(const cmos_lib_list_T *list)
{
    return (cmos_lib_list_node_T *)list;
}

/*******************************************************************************
 *
 * 函数名  : list_get_tail
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
static cmos_lib_list_node_T *list_get_tail(const cmos_lib_list_T *list)
{
    cmos_lib_list_node_T *go_ptr = NULL;
    if(NULL == list) /* 空表尾结点为空 */
    {
        return NULL;
    } 
    
    go_ptr = list_get_head(list);
    while(NULL != go_ptr->next)
    {
        go_ptr = go_ptr->next;
    }

    return go_ptr;
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
inline void *cmos_lib_list_node_get_data(const cmos_lib_list_node_T *node)
{
    cmos_assert(NULL != node, (uint8_t *)__FILE__, __LINE__);

    return node->data;
}

