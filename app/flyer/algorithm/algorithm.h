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

/*********************************** 类型定义 **********************************/


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
float inv_sqrt(float x);

int imu_update(float *quaternion, const float *gyro);
int fusion6axis(float *quaternion, const float *gyro, const float *accel);
int fusion9axis(float *quaternion, const float *gyro, const float *accel, const float *mag);
int get_attitude(float *attitude);

int pwm_pid(const float *expect_attitude);

#endif // #ifndef _CMOS_ALGORITHM_H_

