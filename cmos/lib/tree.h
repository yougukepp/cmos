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
typedef struct cmos_lib_tree_node_T_tag cmos_lib_tree_node_T;
struct cmos_lib_tree_node_T_tag{
    void *data;                         /* 结点数据指针 */
    cmos_lib_tree_node_T *parent;       /* 父结点 */
    cmos_lib_tree_node_T *first_sun;    /* 首孩子结点 */
    cmos_lib_tree_node_T *next_brother; /* 下一兄弟结点 */
};

/* 树的定义 */
typedef cmos_lib_tree_node_T cmos_lib_tree_T;
/* 遍历时对于结点的回调函数 */
typedef cmos_status_T (*cmos_lib_tree_node_func_T)(void *data);

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void cmos_lib_tree_init(cmos_lib_tree_T **tree, cmos_lib_tree_node_T *node);
cmos_status_T cmos_lib_tree_destroy(cmos_lib_tree_T *tree);
cmos_lib_tree_node_T *cmos_lib_tree_root(const cmos_lib_tree_T *tree);
cmos_status_T cmos_lib_tree_insert_child(cmos_lib_tree_T *tree,
        cmos_lib_tree_node_T *modify_node,
        cmos_uint32_T index,
        cmos_lib_tree_node_T *child_root);
cmos_status_T cmos_lib_tree_walk(cmos_lib_tree_T *tree, cmos_lib_tree_node_func_T func);

cmos_lib_tree_node_T *cmos_lib_tree_get_first_sun(const cmos_lib_tree_T *tree,
        const cmos_lib_tree_node_T *node);
cmos_lib_tree_node_T *cmos_lib_tree_get_next_brother(const cmos_lib_tree_T *tree,
        const cmos_lib_tree_node_T *node);
void *cmos_lib_tree_get_data(const cmos_lib_tree_T *tree,
        const cmos_lib_tree_node_T *node);
cmos_lib_tree_node_T *cmos_lib_tree_node_malloc(const void *data);


#endif /* #ifndef _CMOS_LIB_TREE_H_ */

