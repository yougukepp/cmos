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
static float s_quaternion[4] = {1.0f, 0.0f, 0.0f, 0.0f}; /* 四元数 q0 q1 q2 q3*/
static float s_eg_integration[3] = {0.0f}; /* 加计误差积分 */

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

    float q0_diff = 0.0f;
    float q1_diff = 0.0f;
    float q2_diff = 0.0f;
    float q3_diff = 0.0f;

    float q_norm = 0.0f;
    float half_period = 0.5f * ALGO_GYRO_PERIOD;

    /* 角度转弧度 */
    wx = math_angle2arc(wx);
    wy = math_angle2arc(wy);
    wz = math_angle2arc(wz);

    quaternion_lock();

    /* 微分 */
    q0_diff =  -half_period * (s_quaternion[1] * wx + s_quaternion[2] * wy + s_quaternion[3] * wz);
    q1_diff =   half_period * (s_quaternion[0] * wx + s_quaternion[2] * wz - s_quaternion[3] * wy);
    q2_diff =   half_period * (s_quaternion[0] * wy - s_quaternion[1] * wz + s_quaternion[3] * wx);
    q3_diff =   half_period * (s_quaternion[0] * wz + s_quaternion[1] * wy - s_quaternion[2] * wx);

    /* 积分 */
    s_quaternion[0] += q0_diff;
    s_quaternion[1] += q1_diff;
    s_quaternion[2] += q2_diff;
    s_quaternion[3] += q3_diff;

#if 0
    printf("%7.4f,%7.4f,%7.4f,%7.4f => %7.4f,%7.4f,%7.4f,%7.4f\n", 
            q0_last, q1_last, q2_last, q3_last,
            s_quaternion[0], s_quaternion[1], s_quaternion[2], s_quaternion[3]);
#endif

    /* 四元数归一化 */
    q_norm = math_inv_sqrt( s_quaternion[0] * s_quaternion[0]
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
 * 函数名  : imu_fusion6axis
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
int imu_fusion6axis(const float *gyro, const float *accel)
{
    float q0 = 0.0f;
    float q1 = 0.0f;
    float q2 = 0.0f;
    float q3 = 0.0f;
    float sg[3] = {0.0f}; /* 加计(重力)向量在s系的值 */
    float eg[3] = {0.0f}; /* 加计误差 */
    float est_gyro[3] = {0.0f}; /* 陀螺仪校正后的值 */

    quaternion_lock();
    q0 = s_quaternion[0];
    q1 = s_quaternion[1];
    q2 = s_quaternion[2];
    q3 = s_quaternion[3];
    quaternion_unlock();

    /* n系到s系的方向余弦的第三列即为 加计(重力)向量 在s系的值 */
    sg[0] = 2*(q1*q3 - q0*q2);
    sg[1] = 2*(q0*q1 + q2*q3);
    sg[2] = 2*(q0*q0 + q3*q3) - 1 ;

    /* 叉积表示误差 */
    math_vector_product(eg, accel, sg);

    /* 对误差进行积分 */
    s_eg_integration[0] += eg[0] * ALGO_ACCEL_KI;
    s_eg_integration[0] += eg[1] * ALGO_ACCEL_KI;
    s_eg_integration[0] += eg[2] * ALGO_ACCEL_KI;

    /* 修正陀螺仪器 */
    est_gyro[0] = gyro[0] + ALGO_ACCEL_KP*eg[0] + s_eg_integration[0];
    est_gyro[1] = gyro[1] + ALGO_ACCEL_KP*eg[1] + s_eg_integration[1];
    est_gyro[2] = gyro[2] + ALGO_ACCEL_KP*eg[2] + s_eg_integration[2];

    /* 使用修正值计算姿态 */
    imu_update(est_gyro);

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : imu_fusion9axis
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
int imu_fusion9axis(const float *gyro, const float *accel, const float *mag)
{
    /* 重力场叉乘地磁场 求解正东单位向量在载体坐标系的值 
     *
     * 重力场 叉 地磁场 = 正东
     * 地磁场 叉 重力场 = 正西
     * 此处使用正东
     *
     * */ 
    float product[3] = {0.0f};
    math_vector_product(product, accel, mag);

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
 * 函数名  : imu_get_attitude
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
int imu_get_attitude(float *attitude)
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

    /* TODO: 文档或者公式有错 */
    yaw   = atan2(2*(q1*q2 - q0*q3), 2*(q0*q0 + q1*q1) - 1);
    roll  = -asin(2*(q1*q3 + q0*q2));
    pitch = atan2(2*(q2*q3 - q0*q1), 2*(q0*q0 + q3*q3) - 1);

    attitude[0] = yaw;
    attitude[1] = pitch;
    attitude[2] = roll;

    return 0;
}

