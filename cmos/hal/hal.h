/********************************************************************************
*
* 文件名  ： hal.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150615
* 版本号  ： v1.0
* 文件描述： cmos hal 模块头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_HAL_H_
#define _CMOS_HAL_HAL_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T hal_init(void);

#endif /* _CMOS_HAL_HAL_H_ */
