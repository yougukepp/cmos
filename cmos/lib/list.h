/******************************************************************************
 *
 * 文件名  ： list.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 数据结构 链表的接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_LIB_LIST_H_
#define _CMOS_LIB_LIST_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
/* 链表结点定义 */
typedef struct cmos_lib_list_node_T_tag cmos_lib_list_node_T;
struct cmos_lib_list_node_T_tag{
    cmos_lib_list_node_T *prev; /* 上一结点 */
    cmos_lib_list_node_T *next; /* 下一结点 */
    void *data;                 /* 结点数据指针 */
};

/* 树的定义 */
typedef cmos_lib_list_node_T cmos_lib_list_T;
/* 遍历时对于结点的回调函数 */
typedef void (*cmos_lib_list_walk_func_T)(cmos_lib_list_node_T *node, void *para);

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_lib_list_destroy(cmos_lib_list_T *list);

cmos_lib_list_node_T *cmos_lib_list_node_malloc(const void *data);
void cmos_lib_list_node_free(cmos_lib_list_node_T *node);
void *cmos_lib_list_node_get_data(const cmos_lib_list_node_T *node);

cmos_lib_list_node_T *cmos_lib_list_get_head(const cmos_lib_list_T *list);
cmos_lib_list_node_T *cmos_lib_list_get_tail(const cmos_lib_list_T *list);

cmos_status_T cmos_lib_list_append(cmos_lib_list_T **list, cmos_lib_list_node_T *node);
cmos_status_T cmos_lib_list_insert(cmos_lib_list_T **list, cmos_lib_list_node_T *node);
cmos_status_T cmos_lib_list_del(cmos_lib_list_T **list, const cmos_lib_list_node_T *node);

void cmos_lib_list_walk(cmos_lib_list_T *list, cmos_lib_list_walk_func_T func, void *para);

cmos_lib_list_node_T *cmos_lib_list_search_by_data(const cmos_lib_list_T *list, const void *data);

#endif /* #ifndef _CMOS_LIB_LIST_H_ */

