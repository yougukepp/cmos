/******************************************************************************
 *
 * 文件名  ： driver.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 虚拟文件系统(vfs)的接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_VFS_VFS_H_
#define _CMOS_VFS_VFS_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "driver.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef enum{
    vfs_file = 0x00,
    vfs_dir = 0x01,
    vfs_dev = 0x02,
    vfs_link = 0x03,

    vfs_err_type = 0xff,
}vfs_node_type_E;

/* vfs结点data域值定义 */
typedef struct vfs_node_data_T_tag{
    vfs_node_type_E type;
    cmos_int8_T     *name;
    cmos_hal_driver_T *driver; /* 驱动指针 */
}vfs_node_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T vfs_init(void);
cmos_status_T vfs_destroy(void);
cmos_hal_driver_T *vfs_get_driver_by_path(const cmos_int8_T *path);
void vfs_print(void);
cmos_status_T vfs_node_add(const cmos_int8_T *dir_path,
        const cmos_int8_T *name, vfs_node_type_E type, const void *dirver);

#endif /* #ifndef _CMOS_VFS_VFS_H_ */

