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
#include "driver.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 
 * vfs树
 * FIXME:所有操作加锁 
 * */
cmos_lib_tree_T *s_vfs_tree;
static vfs_fd_item_T s_vfs_fd_list[CMOS_VFS_FD_MAX] = {{0}}; /* fd 列表 */
static cmos_int32_T s_vfs_fd_list_index = 0;            /* fd 列表索引 */

/********************************** 函数声明区 *********************************/
static cmos_lib_tree_node_T *vfs_tree_node_malloc(vfs_node_type_E type, const cmos_uint8_T *name, const void *driver);
static void node_print(cmos_lib_tree_node_T *node, void *para);
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
    root_node = vfs_tree_node_malloc(vfs_dir, CMOS_VFS_ROOT, NULL);
    if(NULL == root_node)
    {
        return cmos_MEM_LACK_E;
    }
    /* 使用根结点初始化树 */ 
    cmos_lib_tree_init(&s_vfs_tree, root_node);

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

    CMOS_TRACE_FUNC_OUT;
    return cmos_OK_E;
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

    status = cmos_lib_tree_destroy(s_vfs_tree);
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
    cmos_lib_tree_insert_child(parent_node, 0, node);


    CMOS_TRACE_FUNC_OUT;
    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : vfs_get_tree_node
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 通过路径查找对应的结点
*
* 输入参数: path 路径
* 输出参数: 无
*
* 返回值  : NULL 失败
*           其他 结点指针
*
* 调用关系: 无
* 其 它   : 无
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
    const cmos_uint8_T *go_path = path;
    cmos_lib_tree_node_T *last_node = NULL;
    cmos_lib_tree_node_T *now_node = NULL;
    cmos_int32_T i = 0;
    cmos_int32_T depth = 0;
    cmos_uint8_T name[CMOS_VFS_NAME_MAX] = {0};

    /* 初始为根结点 */
    last_node = cmos_lib_tree_root(s_vfs_tree);
    
    /* 路径对应根结点 */
    if(CMOS_VFS_ROOT_LEN == strlen((const char *)go_path))
    { 
        now_node = last_node;
        goto found;
    } 

    /* step1: 求取path的深度 */
    depth = vfs_path_depth(go_path);

    /* step2: 逐级比对 */
    for(i = 0; i < depth; i++)
    {
        /* step2.1: 获取i级path name TODO: 实现 */
        status = vfs_path_sub_name(name, CMOS_VFS_NAME_MAX, go_path, i);
        if(cmos_OK_E != status)
        {
            CMOS_ERR_STR("vfs_path_sub_name err.");
            return NULL;
        }

        /* step2.2: 比对name与本级vfs所有名字 返回匹配的node */
        now_node = vfs_name_compare(last_node, name);

        /* step2.3: 比对成功 continue */
        if(NULL != now_node)
        {
            /* step2.3: 更新下一级vfs名字 */
            last_node = cmos_lib_tree_node_first_sun(now_node);
        }
        /* step2.4: 否则 return NULL */
        else
        {
            return NULL;
        }
    }

found:
    CMOS_TRACE_FUNC_OUT;
    return now_node;
}

/*******************************************************************************
*
* 函数名  : vfs_node_name
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 获取结点名
*
* 输入参数: 无
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
const cmos_uint8_T *vfs_node_name(vfs_node_T *node)
{
    CMOS_TRACE_FUNC_IN;

    if(NULL == node) 
    { 
        CMOS_ERR_STR("NULL vfs node have no name.");
        return NULL;
    } 

    CMOS_TRACE_FUNC_OUT;
    return node->name;
}

/*******************************************************************************
*
* 函数名  : vfs_print
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 打印vfs
*
* 输入参数: 无
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void vfs_print(void)
{ 
    cmos_lib_tree_walk_func_T print_node = (cmos_lib_tree_walk_func_T)node_print;
    cmos_lib_tree_walk(s_vfs_tree, print_node, NULL);
}

/*******************************************************************************
*
* 函数名  : node_print
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 打树结点
*
* 输入参数: para 
*             node         待打印的结点
*             get_data_str 结点数据域的提供的获取其标识字符串的回调函数
*
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 为了便于使用cmos_lib_tree_walk函数参数使用指针
*
******************************************************************************/
static void node_print(cmos_lib_tree_node_T *node, void *para)
{
    if(NULL == node)
    {
        CMOS_ERR_STR("node_print node should not be null.\n");
        return;
    }

    void *data = NULL;
    char *data_str = NULL;
    cmos_int32_T i = 0;
    cmos_lib_tree_node_T *go_node = node;
    cmos_int32_T distance = 0;
    cmos_int32_T space_num = 0; /* 凹入表示法 深度越深的结点 空格越少 根最靠前 */

    distance = cmos_lib_tree_node_distance(go_node);
    if(distance>0) /* 非根结点 后移 */
    {
        space_num = 2 * distance;
    }

    data = node->data;
    data_str = (char *)vfs_node_name(data);

    for(i = 0; i < space_num; i++)
    {
        cmos_console_printf(" ");
    }

    if(0 == distance)
    {
        cmos_console_printf("|");
    }
    else
    {
        cmos_console_printf("|-");
    }
    cmos_console_printf(data_str);
    cmos_console_printf("\r\n");

    return;
}

/*******************************************************************************
*
* 函数名  : vfs_name
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 比对name与now_node同级vfs的所有名字 返回匹配的兄弟node
*
* 输入参数: tree_node 该级首子节点
*           name      结点名
* 输出参数: 无
*
* 返回值  : NULL 无匹配的结点
*           其他 tree_node兄弟结点中与name匹配的结点
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_lib_tree_node_T *vfs_name_compare(const cmos_lib_tree_node_T *tree_node, const cmos_uint8_T *name)
{
    if((NULL == tree_node) 
    || (NULL == name))
    { 
        CMOS_ERR_STR("vfs_name_compare should not with null para.");
        return NULL;
    } 
    vfs_node_T *vfs_node = NULL;
    cmos_lib_tree_node_T *go_node = (cmos_lib_tree_node_T *)tree_node;

    do
    {
        vfs_node = cmos_lib_tree_node_data(go_node); 
        if(0 == strcmp((const char *)name, (const char *)vfs_node->name)) /* 找到 */ 
        {
            break;
        }
        else
        {
            go_node = cmos_lib_tree_node_next_brother(go_node);
        }

    }while(NULL != go_node);

    return go_node;
}

/*******************************************************************************
*
* 函数名  : vfs_open
* 负责人  : 彭鹏
* 创建日期: 20151110
* 函数功能: 打开path指示的文件
*
* 输入参数: path 文件路径
*           flag 打开flag
*           mode 模式
*           参数规则参考Linux open
* 输出参数: 无
*
* 返回值  : 文件句柄
*
* 调用关系: 无
* 其 它   : TODO: 实现多设备管理
*
******************************************************************************/
cmos_int32_T vfs_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
    /* step1: 找到对应驱动 */
    cmos_lib_tree_node_T *tree_node = NULL;
    vfs_node_T *vfs_node = NULL;
    cmos_hal_driver_T *driver = NULL;
    void *driver_id = NULL;

    /* vfs fd 已满 */
    if(CMOS_VFS_FD_MAX <= s_vfs_fd_list_index)
    {
        CMOS_ERR_STR("vfs fd list is full.");
        goto err;
    }
    if(NULL == path)
    {
        CMOS_ERR_STR("vfs_open should not have a null path.");
        goto err;
    }

    tree_node = vfs_get_tree_node(path);
    vfs_node = cmos_lib_tree_node_data(tree_node);
    driver = vfs_node->driver;

    /* step2: 执行驱动对应open函数 获取驱动相关句柄 */
    if(NULL != driver)
    {
        driver_id = driver->open(path, flag, mode);
    }
    else
    {
        cmos_err_log("%s driver is null.", path);
        goto err;
    }

    /* step3: 保存底层句柄 */
    s_vfs_fd_list[s_vfs_fd_list_index].driver = driver;
    s_vfs_fd_list[s_vfs_fd_list_index].driver_id = driver_id;
    s_vfs_fd_list_index++;

    /* 当前fd为自加之前的 */
    return s_vfs_fd_list_index - 1;

err:
    return -1;
}

/*******************************************************************************
*
* 函数名  : vfs_write
* 负责人  : 彭鹏
* 创建日期: 20151110
* 函数功能: 写入fd指示的文件
*
* 输入参数: fd      文件句柄
*           buf     缓冲
*           n_bytes 缓冲大小
*           参数规则见Linux write
* 输出参数: 无
*
* 返回值  : 写入字节数
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_int32_T vfs_write(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes)
{
    cmos_int32_T write_bytes = 0;
    /* step1: 找到对应驱动及stm32hal底层句柄 */
    vfs_fd_item_T *fd_item = NULL;
    cmos_hal_driver_T *driver = NULL;
    void *driver_id = NULL;

    fd_item = &s_vfs_fd_list[fd];
    driver = fd_item->driver;
    driver_id = fd_item->driver_id;         /* 底层驱动需要的指针 */

    /* step2: 执行驱动对应write函数 */
    if(NULL != driver)
    { 
        write_bytes = driver->write(driver_id, buf, n_bytes);
    }
    else
    {
        cmos_err_log("file %d driver is null.", fd);
        write_bytes = 0;
    }

    return write_bytes;
}

/*******************************************************************************
*
* 函数名  : vfs_ioctl
* 负责人  : 彭鹏
* 创建日期: 20151117
* 函数功能: 控制fd指示的文件
*
* 输入参数: fd      文件句柄
*           request 请求号码定义于cmos_api.h
*           mode    控制模式(可选参数)
*           参数规则见Linux ioctl
* 输出参数: 无
* 返回值  : 执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T vfs_ioctl(cmos_int32_T fd, cmos_uint32_T request, cmos_uint32_T mode)
{
    ;
}

