/******************************************************************************
 *
 * 文件名  ： list.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030 
 * 版本号  ： v1.0
 * 文件描述： 数据结构 链表的接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 实现queue、stack等线性结构的底层操作
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

/* 链表的定义 */
typedef cmos_lib_list_node_T cmos_lib_list_T;

/* 遍历时对于结点的回调函数 */
typedef void (*cmos_lib_list_walk_func_T)(void *data, void *para);

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_lib_list_init(cmos_lib_list_T **list);           /* 初始化 */
cmos_status_T cmos_lib_list_destroy(cmos_lib_list_T **list);        /* 销毁 */
cmos_int32_T cmos_lib_list_length(cmos_lib_list_T *list);           /* 链表长度 节点数 */

/* 1+2 = stack
 * 1+3 = queue */
cmos_status_T cmos_lib_list_push_tail(cmos_lib_list_T **list, const void *data);/* 1 尾部加入 */
void *cmos_lib_list_pop_tail(cmos_lib_list_T **list);                           /* 2 尾部弹出 */
void *cmos_lib_list_pop_head(cmos_lib_list_T **list);                           /* 3 头部弹出 */

/* 单纯获取 获取头中数据域指针 不增删结点 */
void *cmos_lib_list_get_head_data(const cmos_lib_list_T *list);                 /* 获取头结点数据域指针 */
//cmos_lib_list_node_T *cmos_lib_list_get_tail_data(const cmos_lib_list_T *list); /* 获取尾结点数据 */

/* 遍历 */
void cmos_lib_list_walk(cmos_lib_list_T *list, cmos_lib_list_walk_func_T func, void *para);

/* 获取结点中数据域指针 */
void *cmos_lib_list_node_get_data(const cmos_lib_list_node_T *node);

/* 目前blocked中专用:删除链表中数据域为data的元素 
 * TODO: 考虑字典或者缓冲池数据结构 */
cmos_status_T cmos_lib_list_del_by_data(cmos_lib_list_T **list, const void *data); 

#endif /* #ifndef _CMOS_LIB_LIST_H_ */

