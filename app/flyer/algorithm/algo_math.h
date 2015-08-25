/******************************************************************************
 *
 * 文件名  ： algo_math.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150824 
 * 版本号  ： v1.0
 * 文件描述： 数学库头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _ALGO_MATH_H_
#define _ALGO_MATH_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
float math_inv_sqrt(float x);
int math_norm(float *dst, const float *src, int dim);
int math_vetor_angle(float *angle, const float *a, const float *b);
float math_angle2arc(float x);
float math_arc2angle(float x);
int math_vector3_cross_product(float *product, const float *a, const float *b);
int math_vector3_dot_product(float *product, const float *a, const float *b);

#endif // #ifndef _ALGO_MATH_H_

