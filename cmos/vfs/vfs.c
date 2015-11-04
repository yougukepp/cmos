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
static cmos_lib_tree_node_T *vfs_tree_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name, const void *driver);
static cmos_lib_tree_node_T *vfs_get_tree_node(const cmos_uint8_T *path);

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
    root_node = vfs_tree_node_malloc(vfs_dir, CMOS_VFS_ROOT, NULL);
    if(NULL == root_node)
    {
        return cmos_MEM_LACK_E;
    }
    /* 使用根结点初始化树 */ 
    cmos_lib_tree_init(&s_vfs_root, root_node, CMOS_VFS_NAME); 

    /* 加入/proc目录 */
    /* TODO:加入/proc/cpuinfo /proc/meminfo文件 */
    status = vfs_node_add(CMOS_VFS_ROOT, CMOS_VFS_DEV, vfs_dir, NULL);
    if(cmos_OK_E != status)
    {
        return status;
    }

    /* 加入/dev目录 为驱动加入到vfs做准备 */
    status = vfs_node_add(CMOS_VFS_ROOT, CMOS_VFS_PROC, vfs_dir, NULL);
    if(cmos_OK_E != status)
    {
        return status;
    }

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
static cmos_lib_tree_node_T *vfs_tree_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name, const void *driver)
{
    /* 数据 */
    vfs_node_T *data = NULL; 
    data = (vfs_node_T *)cmos_malloc(sizeof(vfs_node_T));
    if(NULL == data)
    {
        return NULL;
    }
    data->type = type;
    data->name = (cmos_uint8_T *)name;
    data->driver = (void *)driver;

    /* 树结点 */
    cmos_lib_tree_node_T *node = NULL;
    node = cmos_lib_tree_node_malloc(data);
    if(NULL == node)
    {
        return NULL;
    }

    return node;
}

/*******************************************************************************
*
* 函数名  : vfs_node_add
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: cmos vfs 树结点初始化
*
* 输入参数: dir_path    父路径
*           name        结点名
*           type        结点类型
*           dirver      结点驱动
*
* 输出参数: 无
*
* 返回值  : cmos_OK_E  成功
*           其他       失败
*
* 调用关系: 无
* 其 它   : TODO:需要对应free否则内存泄露
*
******************************************************************************/
cmos_status_T vfs_node_add(const cmos_uint8_T *dir_path, const cmos_uint8_T *name, vfs_node_type_E type, const void *driver)
{
    cmos_status_T status = cmos_ERR_E;

    cmos_lib_tree_node_T *node = NULL; 
    node = vfs_tree_node_malloc(type, name, driver);
    if(NULL == node)
    {
        return cmos_MEM_LACK_E;
    }

    /* 根据dir_path查找父节点 */
    cmos_lib_tree_node_T *parent_node = NULL; 
    parent_node = vfs_get_tree_node(dir_path);
    if(NULL == parent_node)
    {
        return cmos_PARA_E;
    }

    /* 插入到父节点 */
    cmos_lib_tree_insert_child(&s_vfs_root, parent_node, 0, node);

    status = cmos_OK_E;
    return status;
}

static cmos_lib_tree_node_T *vfs_get_tree_node(const cmos_uint8_T *path)
{
#if 0
    cmos_lib_tree_node_T *go_node = NULL;
    cmos_uint8_T *go_path = NULL;
    cmos_int32_T name_count = 0;
    cmos_int32_T i = 0;
    cmos_uint8_T name[CMOS_VFS_NAME_MAX] = {0};
    cmos_int32_T root_len = 0;

    root_len = strlen(CMOS_VFS_ROOT);
    go_path = path;
    go_node = s_vfs_root;
    if(0 != strcmp(*go_path, CMOS_VFS_ROOT, root_len)) /* 不以根打头 路径错误 */
    {
        return cmos_PARA_E;
    }

    for(go_path += root_len; (NULL != go_path) && (NUL != *go_path); go_path++)
    {
        if(0 != strcmp(go_path, CMOS_VFS_SEPARATOR, root_len)) /* 非分隔符 */
        {
            name[i++] = *go_path;
            if(CMOS_VFS_NAME_MAX <= i) /* 名字过长 防止溢出 */
            {
                return cmos_PARA_E;
            }
        }
        else
        {
            //s_vfs_root->
            /* 以name 查找是否为该结点 */
            i = 0;
        }
    }
#endif

    return NULL;
}

