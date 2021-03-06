/********************************************************************************
*
* 文件名  ： mem.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20151103
* 版本号  ： v1.0
* 文件描述： 动态分配内存
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_LIB_MEM_H_
#define _CMOS_LIB_MEM_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void *cmos_malloc(cmos_int32_T size);
void cmos_free(void *ptr);

#endif /* _CMOS_LIB_MEM_H_ */

