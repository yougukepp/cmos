/********************************************************************************
*
* 文件名  ： misc.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： 杂项函数接口
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_LIB_MISC_H_
#define _CMOS_LIB_MISC_H_

/************************************ 头文件 ***********************************/
#include "string.h"
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/
/*
 * container_of - 通过结构体的一个成员获取容器结构体的指针
 *
 * @ptr:        指向成员的指针。
 * @type:       成员所嵌入的容器结构体类型。
 * @member:     结构体中的成员名。
 *
 * 备注:        参考Linux内核同名宏
 *
 * */
#define cmos_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define cmos_container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - cmos_offsetof(type,member) );})

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void assert_failed(uint8_t* file, uint32_t line);
void cmos_assert(cmos_bool_T flag, uint8_t* file, uint32_t line);
void err_loop(void);
void cmos_strcat(cmos_int8_T *dst, const cmos_int8_T *src, cmos_int32_T len);

#endif /* _CMOS_LIB_MISC_H_ */

