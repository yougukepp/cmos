/******************************************************************************
 *
 * 文件名  ： tree.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 硬件设备树头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_HARDWARE_TREE_H_
#define _CMOS_HAL_HARDWARE_TREE_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/
/* 设备树 根 */
#define CMOS_HAL_HARDWARE_TREE_ROOT_NAME        ("/dev/")
/* 设备树名字 */
#define CMOS_HAL_HARDWARE_TREE_NAME_SIZE        (16)
/* 设备树分隔符 */
#define CMOS_HAL_HARDWARE_TREE_SPLIT_CHAR       ('/')

/*********************************** 类型定义 **********************************/
typedef struct cmos_hal_hardware_tree_node_T_tag{
    cmos_uint8_T *ptr_name;
    cmos_int32_T *ptr_value;
    struct cmos_hal_hardware_tree_node_T_tag *ptr_first_sun;
    struct cmos_hal_hardware_tree_node_T_tag *ptr_next_brother;
}cmos_hal_hardware_tree_node_T;

/* 对结点的操作函数 */
typedef cmos_status_T (*cmos_hal_hardware_tree_node_func_T)(cmos_hal_hardware_tree_node_T *ptr_node);

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_hal_hardware_tree_init(void);
cmos_status_T cmos_hal_hardware_tree_add(const cmos_uint8_T *path);
cmos_status_T cmos_hal_hardware_tree_del(const cmos_uint8_T *path);
cmos_status_T cmos_hal_hardware_tree_print(void);

#endif /* #ifndef _CMOS_HAL_HARDWARE_TREE_H_ */

