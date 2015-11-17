/******************************************************************************
 *
 * 文件名  ： attidude.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150825 
 * 版本号  ： v1.0
 * 文件描述： 姿态数学表示接口
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _ATTIDUDE_H_
#define _ATTIDUDE_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
int attidude_init(void);
int attidude_set_quaternion(const float *quaternion);
int attidude_get_quaternion(float *quaternion);
int attidude_get_level(float *theta, float *phi);
int attidude_get_psi(float *psi);
int attidude_get_matrix_by_column(float *data, int column);
int attidude_euler2quaternion(float *quaternion, const float *euler);

int attidude_print_quaternion(void);

#endif // #ifndef _ATTIDUDE_H_

