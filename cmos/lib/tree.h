/******************************************************************************
 *
 * 文件名  ： tree.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 数据结构 树的接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_LIB_TREE_H_
#define _CMOS_LIB_TREE_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
/* 树结点定义 */
typedef struct cmos_lib_tree_node_T_tag{
    void *data;                                           /* 结点数据指针 */
    struct cmos_lib_tree_node_T_tag *parent;              /* 父结点 */
    struct cmos_lib_tree_node_T_tag *first_sun;           /* 首孩子结点 */
    struct cmos_lib_tree_node_T_tag *next_brother;        /* 下一兄弟结点 */
}cmos_lib_tree_node_T;

/* 树的定义恰巧与树的结点相同 */
typedef cmos_lib_tree_node_T cmos_lib_tree_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void cmos_lib_tree_init(cmos_lib_tree_T *root);
cmos_status_T cmos_lib_tree_destroy(cmos_lib_tree_T *root);
cmos_status_T cmos_lib_tree_insert_child(cmos_lib_tree_T *root, cmos_lib_tree_node_T *modify_node, cmos_uint32_T index, cmos_lib_tree_T *child_root);

#endif /* #ifndef _CMOS_LIB_TREE_H_ */

