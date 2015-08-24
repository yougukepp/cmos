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
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

/************************************ 头文件 ***********************************/

/************************************ 宏定义 ***********************************/
/* 加计修正参数 */
#define ALGO_ACCEL_KI           (0.008f)
#define ALGO_ACCEL_KP           (10.0f)

/* 陀螺仪 3轴姿态更新周期 s为单位 */
#define ALGO_GYRO_PERIOD        (1.0f / 1000)
#define ALGO_PI                 (3.1415926f)
#define ALGO_ANGLE2ARC_RATE     (ALGO_PI/180)
#define ALGO_ARC2ANGLE_RATE     (180/ALGO_PI)
#define ALGO_QUAD               (4)
#define ALGO_DIM                (3)
#define ALGO_PITCH              (0)
#define ALGO_ROLL               (1)
#define ALGO_YAW                (2)

#ifdef X86_64
#define DEBUG_P                 printf
#else
#define DEBUG_P                 cmos_printf
#endif

/*********************************** 类型定义 **********************************/
typedef struct ALGO_IMU_PARA_TAG{
    unsigned int feature;
    unsigned int gyro_period;
    unsigned int accel_period;
    unsigned int mag_period;
}ALGO_IMU_PARA_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
int imu_set(ALGO_IMU_PARA_T *imu_para);
int imu_start(void);
int imu_get_pitch(float *pitch);
int imu_get_roll(float *roll);
int imu_get_yaw(float *yaw);

float math_inv_sqrt(float x);
float math_angle2arc(float x);
float math_arc2angle(float x);
int math_vector_product(float *product, const float *a, const float *b);


#endif // #ifndef _CMOS_ALGORITHM_H_

