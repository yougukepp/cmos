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
/* 算法库参数 */
/*
 * bit0   gyro
 * bit1   accel
 * bit2   mag
 *
 * */
#define ALGO_NONE               (0x0000)
#define ALGO_GYRO               (0x0001)
#define ALGO_ACCEL              (0x0002)
#define ALGO_MAG                (0x0004)
#define ALGO_GYRO_START(v)      ((ALGO_GYRO) & (v))
#define ALGO_ACCEL_START(v)     (((ALGO_ACCEL) & (v)) >> 1)
#define ALGO_MAG_START(v)       (((ALGO_MAG) & (v)) >> 2)

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
#define DEBUG_P                 printf
#else
#define DEBUG_P                 cmos_printf
#endif

/*********************************** 类型定义 **********************************/
typedef struct ALGO_IMU_PARA_TAG{
    unsigned int features;
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

int attidude_init(void);
int attidude_get_quaternion(float *quaternion);
int attidude_set_quaternion(const float *quaternion);
int attidude_euler2quaternion(float *quaternion, const float *euler);
int attidude_quaternion2euler(float *euler, const float *quaternion);
int attidude_print_quaternion(void);

float math_inv_sqrt(float x);
float math_angle2arc(float x);
float math_arc2angle(float x);
int math_vector_product(float *product, const float *a, const float *b);

#ifdef X86_64
int data_get_gyro(float *gyro);
#else
#endif

#endif // #ifndef _CMOS_ALGORITHM_H_

