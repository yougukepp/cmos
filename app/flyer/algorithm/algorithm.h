/******************************************************************************
 *
 * 文件名  ： algorithm.h
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
#ifndef _CMOS_ALGORITHM_H_
#define _CMOS_ALGORITHM_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/
#define CMOS_PI                 (3.1415926f)
#define CMOS_ANGLE_ARC_RATE     (CMOS_PI/180)
#define CMOS_ARC_ANGLE_RATE     (180/CMOS_PI)

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
float inv_sqrt(float x);
float angle2arc(float x);
float arc2angle(float x);

int imu_update(const float *gyro);
int fusion6axis(const float *gyro, const float *accel);
int fusion9axis(const float *gyro, const float *accel, const float *mag);
int get_attitude(float *attitude);

int pwm_pid(const float *expect_attitude);

/* 调试 */
void print_quaternion(void);

#endif // #ifndef _CMOS_ALGORITHM_H_

