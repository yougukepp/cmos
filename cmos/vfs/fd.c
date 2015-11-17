/******************************************************************************
 *
 * 文件名  ： fd.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 实现文件描述符列表
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tree.h"
#include "path.h"
#include "vfs.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static cmos_status_T vfs_fd_get_fd_list_item(cmos_hal_driver_T **driver, void **driver_id, cmos_int32_T index);


/********************************** 变量实现区 *********************************/
/*
 * FIXME:所有操作加锁 
 * */
static vfs_fd_item_T s_vfs_fd_list[CMOS_VFS_FD_MAX] = {{0}};    /* fd 列表 */
static cmos_int32_T s_vfs_fd_list_index = 0;                    /* fd 列表索引 */

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : vfs_fd_open
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
cmos_int32_T vfs_fd_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
    /* step1: 找到对应驱动 */
    cmos_lib_tree_node_T *tree_node = NULL;
    vfs_node_T *vfs_node = NULL;
    cmos_hal_driver_T *driver = NULL;
    void *driver_id = NULL;

    if(NULL == path)
    {
        CMOS_ERR_STR("vfs_open with null path.");
        goto err;
    }

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

    /* TODO:封装文件描述符列表操作 */
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
* 函数名  : vfs_fd_read
* 负责人  : 彭鹏
* 创建日期: 20151117
* 函数功能: 读取fd指示的文件
*
* 输入参数: fd      文件句柄
*           buf     缓冲
*           n_bytes 缓冲大小
*           参数规则见Linux read
* 输出参数: 无
*
* 返回值  : 读取字节数
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_int32_T vfs_fd_read(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes)
{
    if((fd < 0)
    || (fd >= CMOS_VFS_FD_MAX))
    {
        CMOS_ERR_STR("vfs_fd_read with invalid fd.");
        return 0;
    }
    if((NULL == buf)
    || (n_bytes < 0))
    {
        CMOS_ERR_STR("vfs_fd_read with invalid buf.");
        return 0;
    }

    return n_bytes;
}

/*******************************************************************************
*
* 函数名  : vfs_fd_write
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
cmos_int32_T vfs_fd_write(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_int32_T write_bytes = 0;
    cmos_hal_driver_T *driver = NULL;
    void *driver_id = NULL;

    if((fd < 0)
    || (fd >= CMOS_VFS_FD_MAX))
    {
        CMOS_ERR_STR("vfs_write with invalid fd.");
        return 0;
    }
    if((NULL == buf)
    || (n_bytes < 0))
    {
        CMOS_ERR_STR("vfs_write with invalid buf.");
        return 0;
    }

    /* step1: 找到对应驱动及cmos hal底层句柄 */
    status = vfs_fd_get_fd_list_item(&driver, &driver_id, fd);
    if(cmos_OK_E != status)
    {
        return 0;
    }

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
* 函数名  : vfs_fd_ioctl
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
cmos_status_T vfs_fd_ioctl(cmos_int32_T fd, cmos_uint32_T request, cmos_uint32_T mode)
{
    if((fd < 0)
    || (fd >= CMOS_VFS_FD_MAX))
    {
        CMOS_ERR_STR("vfs_write with invalid fd.");
        return cmos_PARA_E;
    }

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : vfs_fd_close
* 负责人  : 彭鹏
* 创建日期: 20151117
* 函数功能: 关闭fd指示的文件
*
* 输入参数: fd      文件句柄
* 输出参数: 无
* 返回值  : 执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T vfs_fd_close(cmos_int32_T fd)
{
    if((fd < 0)
    || (fd >= CMOS_VFS_FD_MAX))
    {
        CMOS_ERR_STR("vfs_write with invalid fd.");
        return cmos_PARA_E;
    }

    /* TODO:调用驱动close函数 */

    /* TODO:删除文件描述符表项 */

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : vfs_fd_get_fd_list_item
* 负责人  : 彭鹏
* 创建日期: 20151117
* 函数功能: 获取文件描述符列表中第fd项的driver及driver_id
*
* 输入参数: fd 文件描述符列表项数
* 输出参数: 无
* 返回值  : 执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static cmos_status_T vfs_fd_get_fd_list_item(cmos_hal_driver_T **driver, void **driver_id, cmos_int32_T index)
{
    vfs_fd_item_T *fd_item = NULL;

    if((index < 0)
    || (index >= CMOS_VFS_FD_MAX))
    {
        CMOS_ERR_STR("vfs_fd_get_fd_list_item with invalid fd.");
        return cmos_PARA_E;
    }

    fd_item = s_vfs_fd_list + index;
    *driver = fd_item->driver;
    *driver_id = fd_item->driver_id;         /* 底层驱动需要的指针 */

    return cmos_OK_E;
}