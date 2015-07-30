/******************************************************************************
 *
 * 文件名  ： imu.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： v1.0
 * 文件描述： 姿态融合算法
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <math.h>
#include <stdio.h>
#include "algorithm.h"



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/*
 * 欧拉角度转四元数
 * 欧拉角度 旋转顺序 z(a) - y(b) - x(c)
 *
 * q0 = cos(c/2)cos(b/2)cos(a/2) + sin(c/2)sin(b/2)sin(a/2)
 * q1 = sin(c/2)cos(b/2)cos(a/2) - cos(c/2)sin(b/2)sin(a/2)
 * q2 = cos(c/2)sin(b/2)cos(a/2) + sin(c/2)cos(b/2)sin(a/2)
 * q3 = cos(c/2)cos(b/2)sin(a/2) + sin(c/2)sin(b/2)cos(a/2)
 *
 * 初始化时 a b c 都为0带入上公式 所以初始化为 s_quaternion[4] = {1,0,0,0}
 * */
static float s_quaternion[4] = {1.0f, 0.0f, 0.0f, 0.0f}; /* 四元数 q0 q1 q2 q3*/

/********************************** 函数声明区 *********************************/
static int quaternion_lock(void);
static int quaternion_unlock(void);

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : imu_update
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 短期融合 3轴融合
 *
 * 输入参数: gyro  陀螺仪输出
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    短期融合仅仅使用gyro输出的角速度积分获取当前姿态
 *
 ******************************************************************************/
int imu_update(const float *gyro)
{
    float wx = gyro[0];
    float wy = gyro[1];
    float wz = gyro[2];

    float q0_last = 0.0f;
    float q1_last = 0.0f;
    float q2_last = 0.0f;
    float q3_last = 0.0f;

    float q_norm = 0.0f;

    /* 角度转弧度 */
    wx = angle2arc(wx);
    wy = angle2arc(wy);
    wz = angle2arc(wz);

    quaternion_lock();

    q0_last = s_quaternion[0];
    q1_last = s_quaternion[1];
    q2_last = s_quaternion[2];
    q3_last = s_quaternion[3];

    /* 更新四元数,四元数微分方程如下：
     *  q0       |  0 -wx -wy -wz |   | q0_last |
     *  q1       | wx   0  wz -wy |   | q1_last |
     *     = 0.5 |                | * |         |
     *  q2       | wy -wz   0  wx |   | q2_last |
     *  q3       | wz  wy -wx   0 |   | q3_last | */
    s_quaternion[0] += (-0.5 * (q1_last * wx + q2_last * wy + q3_last * wz));
    s_quaternion[1] += ( 0.5 * (q0_last * wx + q2_last * wz - q3_last * wy));
    s_quaternion[2] += ( 0.5 * (q0_last * wy - q1_last * wz + q3_last * wx));
    s_quaternion[3] += ( 0.5 * (q0_last * wz + q1_last * wy - q2_last * wx));

#if 0
    printf("%7.4f,%7.4f,%7.4f,%7.4f => %7.4f,%7.4f,%7.4f,%7.4f\n", 
            q0_last, q1_last, q2_last, q3_last,
            s_quaternion[0], s_quaternion[1], s_quaternion[2], s_quaternion[3]);
#endif

    /* 归1化 只旋转不拉伸 */
    q_norm = inv_sqrt( s_quaternion[0] * s_quaternion[0]
                     + s_quaternion[1] * s_quaternion[1]
                     + s_quaternion[2] * s_quaternion[2]
                     + s_quaternion[3] * s_quaternion[3]);

    s_quaternion[0] *= q_norm;
    s_quaternion[1] *= q_norm;
    s_quaternion[2] *= q_norm;
    s_quaternion[3] *= q_norm;

    quaternion_unlock();

#if 0
    printf("%7.4f,%7.4f,%7.4f,%7.4f => %7.4f,%7.4f,%7.4f,%7.4f\n", 
            q0_last, q1_last, q2_last, q3_last,
            s_quaternion[0], s_quaternion[1], s_quaternion[2], s_quaternion[3]);
#endif

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : fusion6aq1is
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 6轴融合
 *
 * 输入参数: gyro  陀螺仪输出
 *           accel 加速度计输出
 *
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    6轴融合使用加速度计获取重力场修正陀螺仪积分误差(无法确定偏航角)
 *
 ******************************************************************************/
int fusion6aq1is(const float *gyro, const float *accel)
{
    return 0;
}

/*******************************************************************************
 *
 * 函数名  : fusion9aq1is
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 9轴融合
 *
 * 输入参数: gyro  陀螺仪输出
 *           accel 加速度计输出
 *           mag   磁力计输出
 *
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    9轴融合使用加速度计获取重力场、地磁场修正陀螺仪积分误差(可以纠正偏航角)
 *
 ******************************************************************************/
int fusion9aq1is(const float *gyro, const float *accel, const float *mag)
{
    return 0;
}

/*******************************************************************************
 *
 * 函数名  : quaternion_lock
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 锁定
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    可能会阻塞 避免获取四元数 时产生不一致
 *
 ******************************************************************************/
inline static int quaternion_lock(void)
{
    return 0;
}

/*******************************************************************************
 *
 * 函数名  : quaternion_lock
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 解除锁定
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    与quaternion_lock成对使用
 *
 ******************************************************************************/
inline static int quaternion_unlock(void)
{
    return 0;
}

/*******************************************************************************
 *
 * 函数名  : get_attitude
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 获取当前针对参考坐标系的姿态
 *           0 pitch 俯仰角
 *           1 roll  翻滚角
 *           2 yaw   偏航角
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    求证公式出处(欧拉角与旋转顺序有关)
 *
 ******************************************************************************/
int get_attitude(float *attitude)
{
    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;

    float q0 = 0.0f;
    float q1 = 0.0f;
    float q2 = 0.0f;
    float q3 = 0.0f;

    quaternion_lock(); 
    q0 = s_quaternion[0];
    q1 = s_quaternion[1];
    q2 = s_quaternion[2];
    q3 = s_quaternion[3];
    quaternion_unlock();

    roll = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q3 * q3 + q1 * q1));
    pitch = asin(2 * (q0 * q1 - q2 * q3));
    yaw = atan2(2 * (q0 * q2 + q3 * q1), 1 - 2 * (q1 * q1 + q2 * q2));

    attitude[0] = pitch;
    attitude[1] = roll;
    attitude[2] = yaw;

    return 0;
}

