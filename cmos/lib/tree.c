/******************************************************************************
 *
 * 文件名  ： tree.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030
 * 版本号  ： v1.0
 * 文件描述： 数据结构 树的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "mem.h"
#include "tree.h"
#include "console.h"

#ifdef _X64_PC_
#include "pc.h"
#endif

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_lib_tree_init
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: tree 初始化
*
* 输入参数: tree 树
*           node 根结点
*           name 树名字
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_lib_tree_init(cmos_lib_tree_T *tree, cmos_lib_tree_node_T *node, const cmos_uint8_T *name)
{
    CMOS_TRACE_FUNC_IN;

    if((NULL == tree)
    || (NULL == node)
    || (NULL == name))
    {
        CMOS_ERR_STR("NULL pointer");
        return;
    }

    tree->root = node;
    tree->name = name;

    CMOS_TRACE_FUNC_OUT;
    return;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_destroy
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: tree 解除初始化
*
* 输入参数: tree 树
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T cmos_lib_tree_destroy(cmos_lib_tree_T *tree)
{
    CMOS_TRACE_FUNC_IN;

    cmos_status_T status = cmos_ERR_E;
    if(NULL == tree)
    {
        return cmos_NULL_E;
    }

    /* TODO:遍历树free每个结点 */

    status = cmos_OK_E;
    CMOS_TRACE_FUNC_OUT;
    return status;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_insert_child
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: 插入子树(或者插入结点)
*
* 输入参数: tree        待插入的树
*           modify_node 待插入子节点的结点
*           index       插入modify_node的度
*           child_root  待插入的子树的根
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T cmos_lib_tree_insert_child(cmos_lib_tree_T *tree, 
        cmos_lib_tree_node_T *modify_node,
        cmos_uint32_T index,
        cmos_lib_tree_node_T *child_root)
{
    CMOS_TRACE_FUNC_IN;

    cmos_status_T status = cmos_ERR_E;

    if((NULL == tree)
    || (NULL == modify_node)
    || (NULL == child_root))
    {
        return cmos_NULL_E;
    }

    status = cmos_OK_E;
    CMOS_TRACE_FUNC_OUT;
    return status;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_node_malloc
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: 分配树结点
*
* 输入参数: data        结点数据域指针
*
* 输出参数: 无
*
* 返回值  : NULL 内存分配失败
*           其他 结点指针
*
* 调用关系: 无
* 其 它   : TODO:需要对应free否则内存泄露
*
******************************************************************************/
cmos_lib_tree_node_T *cmos_lib_tree_node_malloc(const void *data)
{
    CMOS_TRACE_FUNC_IN;

    cmos_lib_tree_node_T *node = NULL;

    if(NULL == data)
    {
        CMOS_ERR_STR("NULL pointer");
        return NULL;
    }

    node = (cmos_lib_tree_node_T *)cmos_malloc(sizeof(cmos_lib_tree_node_T));
    if(NULL == node)
    {
        return NULL;
    }
    node->data = (void *)data;
    node->parent = NULL;
    node->first_sun = NULL;
    node->next_brother = NULL;

    CMOS_TRACE_FUNC_OUT;
    return node;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_get_first_sun
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的首子结点
*
* 输入参数: tree    待查找的树
*           node    待查结点
*
* 输出参数: 无
*
* 返回值  : NULL 无相应结点
*           其他 子结点指针
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_lib_tree_node_T *cmos_lib_tree_get_first_sun(const cmos_lib_tree_T *tree,
        const cmos_lib_tree_node_T *node)
{
    if((NULL == tree)
    || (NULL == node))
    {
        CMOS_ERR_STR("NULL pointer");
        return NULL;
    }

    return node->first_sun;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_get_next_brother
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的下一兄弟结点
*
* 输入参数: tree    待查找的树
*           node    待查结点
*
* 输出参数: 无
*
* 返回值  : NULL 无相应结点
*           其他 子结点指针
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_lib_tree_node_T *cmos_lib_tree_get_next_brother(const cmos_lib_tree_T *tree,
        const cmos_lib_tree_node_T *node)
{
    if((NULL == tree)
    || (NULL == node))
    {
        CMOS_ERR_STR("NULL pointer");
        return NULL;
    }

    return node->next_brother;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_get_data
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的数据域指针
*
* 输入参数: tree    待查找的树
*           node    待查结点
*
* 输出参数: 无
*
* 返回值  : NULL 无数据
*           其他 数据指针
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void *cmos_lib_tree_get_data(const cmos_lib_tree_T *tree,
        const cmos_lib_tree_node_T *node)
{
    if((NULL == tree)
    || (NULL == node))
    {
        CMOS_ERR_STR("NULL pointer");
        return NULL;
    }

    return node->data;
}

