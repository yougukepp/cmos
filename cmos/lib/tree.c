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
* 输入参数: root 树根
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_lib_tree_init(cmos_lib_tree_T *root, const cmos_lib_tree_node_T *node)
{
    root->data = NULL;
    root->parent = NULL;
    root->first_sun = NULL;
    root->next_brother = NULL;

    return;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_destroy
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: tree 解除初始化
*
* 输入参数: root 树根
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T cmos_lib_tree_destroy(cmos_lib_tree_T *root)
{
    cmos_status_T status = cmos_ERR_E;
    /* TODO:遍历树free每个结点 */

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_insert_child
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: 插入子树(或者插入结点)
*
* 输入参数: root        待插入的树的根
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
cmos_status_T cmos_lib_tree_insert_child(cmos_lib_tree_T *root,
        cmos_lib_tree_node_T *modify_node,
        cmos_uint32_T index,
        const cmos_lib_tree_T *child_root)
{
    cmos_status_T status = cmos_ERR_E;

    status = cmos_OK_E;
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
    cmos_lib_tree_node_T *node = NULL;

    node = (cmos_lib_tree_node_T *)cmos_malloc(sizeof(cmos_lib_tree_node_T));
    if(NULL == node)
    {
        return NULL;
    }
    node->data = (void *)data;
    node->parent = NULL;
    node->first_sun = NULL;
    node->next_brother = NULL;

    return NULL;
}
