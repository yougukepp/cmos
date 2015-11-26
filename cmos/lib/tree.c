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
* 其 它   : tree 需要使用二级指针
*
******************************************************************************/
void cmos_lib_tree_init(cmos_lib_tree_T **tree, cmos_lib_tree_node_T *node)
{
    CMOS_TRACE_FUNC_IN;
    if((NULL == node)
    || (NULL == tree))
    {
        CMOS_ERR_STR("NULL pointer");
        return;
    }
    *tree = node;

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
* 函数名  : cmos_lib_tree_root
* 负责人  : 彭鹏
* 创建日期: 20151105
* 函数功能: 获取树tree的根
*
* 输入参数: tree 树
* 输出参数: 无
*
* 返回值  : tree树的根
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_lib_tree_node_T *cmos_lib_tree_root(const cmos_lib_tree_T *tree)
{
    if(NULL == tree)
    {
        CMOS_ERR_STR("tree root must not be NULL.");
    }

    return (cmos_lib_tree_node_T *)tree;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_insert_child
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: 插入子树(或者插入结点)
*
* 输入参数: parent 待插入子节点的结点
*           order  插入parent的度
*           child  待插入的子树的根
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_lib_tree_insert_child(cmos_lib_tree_node_T *parent,
        cmos_int32_T order,
        cmos_lib_tree_node_T *child)
{
    CMOS_TRACE_FUNC_IN;

    if((NULL == parent)
    || (NULL == child))
    {
        CMOS_ERR_STR("cmos_lib_tree_insert_child para is null.");
        return;
    }
    if(0 != order) /* 目前仅插为首子结点 */
    {
        CMOS_ERR_STR("cmos_lib_tree_insert_child no accept 0 != order now.");
        return;
    }

    child->parent = parent;
    cmos_lib_tree_node_T *first_sun = cmos_lib_tree_node_first_sun(parent);
    if(NULL != first_sun)
    {
        child->next_brother = first_sun;
    } 
    parent->first_sun = child; 

    CMOS_TRACE_FUNC_OUT;
    return;
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
* 函数名  : cmos_lib_tree_node_parent
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的父结点
*
* 输入参数: node 待查结点
*
* 输出参数: 无
*
* 返回值  : NULL 无相应结点
*           其他 父结点指针
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_lib_tree_node_T *cmos_lib_tree_node_parent(const cmos_lib_tree_node_T *node)
{
    //CMOS_TRACE_FUNC_IN;
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_tree_node_parent should not with NULL");
        return NULL;
    }

    //CMOS_TRACE_FUNC_OUT;
    return node->parent;
}

/*******************************************************************************
*
*
* 函数名  : cmos_lib_tree_node_first_sun
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的首子结点
*
* 输入参数: node    待查结点
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
cmos_lib_tree_node_T *cmos_lib_tree_node_first_sun(const cmos_lib_tree_node_T *node)
{
    //CMOS_TRACE_FUNC_IN;
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_tree_node_first_sun should not with NULL");
        return NULL;
    }

    //CMOS_TRACE_FUNC_OUT;
    return node->first_sun;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_node_next_brother
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的下一兄弟结点
*
* 输入参数: node    待查结点
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
cmos_lib_tree_node_T *cmos_lib_tree_node_next_brother(const cmos_lib_tree_node_T *node)
{
    //CMOS_TRACE_FUNC_IN;
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_tree_node_next_brother should not with NULL");
        return NULL;
    }

    //CMOS_TRACE_FUNC_OUT;
    return node->next_brother;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_data
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 获取结点 node 的数据域指针
*
* 输入参数: node    待查结点
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
void *cmos_lib_tree_node_data(const cmos_lib_tree_node_T *node)
{
    CMOS_TRACE_FUNC_IN;
    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_tree_data should not with NULL");
        return NULL;
    }

    CMOS_TRACE_FUNC_OUT;
    return node->data;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_node_depth
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 获取树的深度
*
* 输入参数: tree    树
*
* 输出参数: 无
*
* 返回值  : 0    空树
*           其他 深度
*
* 调用关系: 无
* 其 它   : FIXME:递归算法 栈要求很高
*
******************************************************************************/
cmos_int32_T cmos_lib_tree_node_depth(const cmos_lib_tree_T *node)
{
    CMOS_TRACE_FUNC_IN;
    cmos_int32_T depth_self = 0;
    cmos_int32_T depth_sun = 0;
    cmos_int32_T depth_brother = 0;
    cmos_lib_tree_node_T *node_next_brother = NULL;
    cmos_lib_tree_node_T *node_first_sun = NULL;

    if(NULL == node)
    {
        return 0;
    }

    /* 1 子女 */
    node_first_sun = cmos_lib_tree_node_first_sun(node);
    depth_sun = 1 + cmos_lib_tree_node_depth(node_first_sun);

    /* 2 兄弟 */
    node_next_brother = cmos_lib_tree_node_next_brother(node);
    depth_brother = cmos_lib_tree_node_depth(node_next_brother);

    /* 2 自己 */
    depth_self = (depth_brother > depth_sun) ? depth_brother : depth_sun;

    CMOS_TRACE_FUNC_OUT;
    return depth_self;

}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_walk
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 遍历树
*
* 输入参数: tree    待遍历的树
*           func    对于每个结点的操作回调
*           para    func函数的第二参数
*
* 输出参数: 无
*
* 返回值  : NULL 无数据
*           其他 数据指针
*
* 调用关系: FIXME:递归算法 栈要求很高
* 其 它   : 
*           func 参数   
*                node 树结点指针
*                para 附加参数
*
******************************************************************************/
void cmos_lib_tree_walk(cmos_lib_tree_T *tree, cmos_lib_tree_walk_func_T func, void *para)
{
    CMOS_TRACE_FUNC_IN;
    cmos_lib_tree_node_T *node_self = tree;
    cmos_lib_tree_node_T *node_next_brother = NULL;
    cmos_lib_tree_node_T *node_first_sun = NULL;
    if(NULL == tree) /* 递归结束 */
    {
        CMOS_TRACE_STR("call myself over");
        return;
    }
    if(NULL == func)
    {
        CMOS_ERR_STR("cmos_lib_tree_walk with NULL function.\n");
        return;
    }

    /* 深度优先 */
    /* 1 自己 */
    func(node_self, para);

    /* 2 子女 */
    node_first_sun = cmos_lib_tree_node_first_sun(node_self);
    if(NULL != node_first_sun) /* 遍历完 */
    { 
        cmos_lib_tree_walk(node_first_sun, func, para);
    }

    /* 3 兄弟 */
    node_next_brother = cmos_lib_tree_node_next_brother(node_self);
    if(NULL != node_next_brother) /* 遍历完 */
    { 
        cmos_lib_tree_walk(node_next_brother, func, para);
    }

    CMOS_TRACE_FUNC_OUT;
    return;
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_node_show
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 打印结点指针
*
* 输入参数: node 树结点指针
*
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void cmos_lib_tree_node_show(const cmos_lib_tree_node_T *node)
{
    if(NULL == node)
    {
        cmos_console_printf("null node\n");
    }
    else
    {
        cmos_console_printf("addr:%p,data:%p,parent:%p,sun:%p,brother:%p\n", 
                node,
                node->data,
                node->parent,
                node->first_sun,
                node->next_brother);
    }
}

/*******************************************************************************
*
* 函数名  : cmos_lib_tree_node_distance
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 获取距离根的跳数 根为0 一级目录为1 以此类推
*
* 输入参数: node 结点
* 输出参数: 无
*
* 返回值  : 0  根
*           >0 结点
*           -1 出错
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_int32_T cmos_lib_tree_node_distance(cmos_lib_tree_node_T *node)
{
    cmos_int32_T distance = 0;
    cmos_lib_tree_node_T *go_node = node;

    if(NULL == node)
    {
        CMOS_ERR_STR("cmos_lib_tree_node_distance node should not be null.\n");
        return -1;
    }

    do
    {
        go_node = cmos_lib_tree_node_parent(go_node);
        if(NULL == go_node)
        {
            break;
        }
        distance++;
    }while(TRUE);

    return distance;
}
