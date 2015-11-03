/******************************************************************************
 *
 * 文件名  ： vfs.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030
 * 版本号  ： v1.0
 * 文件描述： 虚拟文件系统(vfs)的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

#include "lib.h"
#include "mem.h"
#include "tree.h"
#include "vfs.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 
 * vfs树
 * FIXME:所有操作加锁 
 * */
cmos_lib_tree_T s_vfs_root;

/********************************** 函数声明区 *********************************/
static vfs_node_T *vfs_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name);
static cmos_lib_tree_node_T * vfs_tree_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name);

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : vfs_init
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: cmos vfs初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T vfs_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* 构造 树根 */ 
    cmos_lib_tree_node_T *root_node = NULL; 
    root_node = vfs_tree_node_malloc(vfs_dir, CMOS_VFS_ROOT);

    /* 使用根结点初始化树 */ 
    cmos_lib_tree_init(&s_vfs_root, root_node); 

    /* 加入/proc目录 */
    cmos_lib_tree_node_T *proc_node = NULL; 
    root_node = vfs_tree_node_malloc(vfs_dir, CMOS_VFS_PROC_DIR);
    cmos_lib_tree_insert_child(&s_vfs_root, root_node, 0, proc_node);
    /* TODO:加入/proc/cpuinfo /proc/meminfo文件 */

    /* 加入/dev目录 为驱动加入到vfs做准备 */
    cmos_lib_tree_node_T *dev_node = NULL; 
    root_node = vfs_tree_node_malloc(vfs_dir, CMOS_VFS_DEV_DIR);
    cmos_lib_tree_insert_child(&s_vfs_root, root_node, 0, dev_node);

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
*
* 函数名  : vfs_destroy
* 负责人  : 彭鹏
* 创建日期: 20151101
* 函数功能: cmos vfs 解除初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T vfs_destroy(void)
{
    cmos_status_T status = cmos_ERR_E;

    status = cmos_lib_tree_destroy(&s_vfs_root);
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : vfs_node_malloc
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: cmos vfs 结点初始化
*
* 输入参数: type 结点类型
*           name 结点名字
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
static vfs_node_T *vfs_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name)
{
    vfs_node_T *data = NULL; 
    data = (vfs_node_T *)cmos_malloc(sizeof(vfs_node_T));
    if(NULL == data)
    {
        return NULL;
    }
    data->type = type;
    data->name = (cmos_uint8_T *)name;

    return data;
}

/*******************************************************************************
*
* 函数名  : vfs_tree_node_malloc
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: cmos vfs 树结点初始化
*
* 输入参数: type        结点类型
*           name        结点名字
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
static cmos_lib_tree_node_T *vfs_tree_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name)
{
    /* 数据 */
    vfs_node_T *data = NULL; 
    data = vfs_node_malloc(type, name);
    if(NULL == data)
    {
        return NULL;
    }

    /* 树结点 */
    cmos_lib_tree_node_T *node = NULL;
    node = cmos_lib_tree_node_malloc(data);
    if(NULL == node)
    {
        return NULL;
    }

    return node;
}

