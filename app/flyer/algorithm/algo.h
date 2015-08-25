/******************************************************************************
 *
 * 文件名  ： algo.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150729 
 * 版本号  ： v1.0
 * 文件描述： 算法库
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _ALGO_H_
#define _ALGO_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/
/* 算法库参数 */
#define ALGO_GYRO_ENABLE        (1)
#define ALGO_ACCEL_ENABLE       (0)
#define ALGO_MAG_ENABLE         (0)
/* 单位 ms */
#define ALGO_GYRO_PERIOD        (5)
#define ALGO_ACCEL_PERIOD       (10)
#define ALGO_MAG_PERIOD         (50)

/* 常用常量 */
#define ALGO_PI                 (3.1415926f)
#define ALGO_ANGLE2ARC_RATE     (ALGO_PI/180)
#define ALGO_ARC2ANGLE_RATE     (180/ALGO_PI)
#define ALGO_QUAD               (4)
#define ALGO_DIM                (3)
#define ALGO_PITCH              (0)
#define ALGO_ROLL               (1)
#define ALGO_YAW                (2)

#ifdef X86_64
#include <stdio.h>
#define DEBUG_P                 printf
#else
#define DEBUG_P                 cmos_printf
#endif

/*********************************** 类型定义 **********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
int algo_init(void);
int algo_start(void);
int algo_get_level(float *pitch, float *roll);
int algo_get_yaw(float *yaw);

int algo_print_quaternion(void);

#endif // #ifndef _ALGO_H_

