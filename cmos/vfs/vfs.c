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

#include <string.h>

#include "lib.h"
#include "mem.h"
#include "tree.h"
#include "path.h"
#include "vfs.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 
 * vfs树
 * FIXME:所有操作加锁 
 * */
cmos_lib_tree_T s_vfs_tree;

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
    CMOS_TRACE_FUNC_IN;

    cmos_status_T status = cmos_ERR_E; 

    /* 构造 树根 */ 
    cmos_lib_tree_node_T *root_node = NULL; 
    root_node = vfs_tree_node_malloc(vfs_dir, (const cmos_uint8_T *)CMOS_VFS_ROOT, NULL);
    if(NULL == root_node)
    {
        return cmos_MEM_LACK_E;
    }
    /* 使用根结点初始化树 */ 
    cmos_lib_tree_init(&s_vfs_tree, root_node, (const cmos_uint8_T *)CMOS_VFS_NAME); 

    /* 加入/proc目录 */
    /* TODO:加入/proc/cpuinfo /proc/meminfo文件 */
    status = vfs_node_add((const cmos_uint8_T *)CMOS_VFS_ROOT, (const cmos_uint8_T *)CMOS_VFS_DEV, vfs_dir, NULL);
    if(cmos_OK_E != status)
    {
        return status;
    }

    /* 加入/dev目录 为驱动加入到vfs做准备 */
    status = vfs_node_add((const cmos_uint8_T *)CMOS_VFS_ROOT, (const cmos_uint8_T *)CMOS_VFS_PROC, vfs_dir, NULL);
    if(cmos_OK_E != status)
    {
        return status;
    }

    status = cmos_OK_E;

    CMOS_TRACE_FUNC_OUT;
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
    CMOS_TRACE_FUNC_IN;

    cmos_status_T status = cmos_ERR_E;

    status = cmos_lib_tree_destroy(&s_vfs_tree);
    if(cmos_OK_E != status)
    {
        assert_failed((cmos_uint8_T *)__FILE__, __LINE__);
    }

    CMOS_TRACE_FUNC_OUT;
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
    CMOS_TRACE_FUNC_IN;

    if((vfs_dev == type)
    && (NULL == driver))
    {
        CMOS_ERR_STR("vfs_dev must have driver.");
        return NULL;
    }
    if(NULL == name)
    {
        CMOS_ERR_STR("vfs name must not be NULL.");
        return NULL;
    }
    cmos_int32_T name_len = 0;
    name_len = strlen((const char *)name);
    if((0 == name_len)
    || (CMOS_VFS_NAME_MAX <= name_len)) /* 注意边间 NUL */
    {
        CMOS_ERR_STR("vfs_name must be in (0, CMOS_VFS_NAME_MAX).");
        return NULL;
    }

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

    CMOS_TRACE_FUNC_OUT;
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
cmos_status_T vfs_node_add(const cmos_uint8_T *dir_path,
        const cmos_uint8_T *name, 
        vfs_node_type_E type,
        const void *driver)
{
    CMOS_TRACE_FUNC_IN;
    if(NULL == dir_path) 
    { 
        CMOS_ERR_STR("dir_path must not be NULL.");
        return cmos_NULL_E;
    }
    if((vfs_dev == type)
    && (NULL == driver))
    {
        cmos_err_log("%s,%d:vfs_dev %s must have driver.", __FILE__, __LINE__, name);
        return cmos_NULL_E;
    }
    if(NULL == name)
    {
        CMOS_ERR_STR("vfs name must not be NULL.");
        return cmos_NULL_E;
    }
    cmos_int32_T name_len = 0;
    name_len = strlen((const char *)name);
    if((0 == name_len)
    || (CMOS_VFS_NAME_MAX <= name_len)) /* 注意边间 NUL */
    {
        CMOS_ERR_STR("vfs_name must be in (0, CMOS_VFS_NAME_MAX).");
        return cmos_PARA_E;
    }

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
    cmos_lib_tree_insert_child(&s_vfs_tree, parent_node, 0, node);

    status = cmos_OK_E;

    CMOS_TRACE_FUNC_OUT;
    return status;
}

/*******************************************************************************
*
* 函数名  : vfs_get_tree_node
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 通过路径查找对应的结点
*
* 输入参数: path    父路径
* 输出参数: 无
*
* 返回值  : NULL 失败
*           其他 结点指针
*
* 调用关系: 无
* 其 它   : TODO:是否可以复用树的遍历?
*
******************************************************************************/
static cmos_lib_tree_node_T *vfs_get_tree_node(const cmos_uint8_T *path)
{
    CMOS_TRACE_FUNC_IN;
    if(NULL == path) 
    { 
        CMOS_ERR_STR("path must not be NULL.");
        return NULL;
    } 
    if(!vfs_path_is_valid(path))
    {
        return NULL;
    }

    cmos_status_T status = cmos_ERR_E;
    cmos_lib_tree_node_T *go_node = NULL;
    const cmos_uint8_T *go_path = path;
    cmos_uint8_T name[CMOS_VFS_NAME_MAX] = {0};

    /* 算好备用 */
    go_node = s_vfs_tree.root; 
    
    /* 路径对应根结点 */
    if(CMOS_VFS_ROOT_LEN == strlen((const char *)go_path))
    {
        goto found;
    } 
    
    CMOS_TRACE_STR(go_path);
    /* TODO: 以下功能使用链表 */
    do{
        /* 找出一级目录 */
        status = vfs_path_head_pop(name, CMOS_VFS_NAME_MAX, go_path);
        if(cmos_OK_E != status) /* 没有有效的目录了 */
        {
            break;
        }
        go_path += strlen((const char *)name) + 1; /* 移除一级子目录 */
        CMOS_TRACE_STR(name);

        /* 查找对应结点 */
        /* 1、获取首子结点 并更新go_node */
        /* 2、比较go_node名字是否与name匹配 */
        /* 3、若是则找到break */
        /* 4、若否则获取go_node右兄弟并更新go_node */
        /* 5、返回2迭代直到 NULL == go_node 匹配失败 */
    }while(TRUE); 

found:
    CMOS_TRACE_FUNC_OUT;
    return go_node;
}

