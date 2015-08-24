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
#include <string.h>
#include "algorithm.h"

#ifdef X86_64
#include <sys/time.h>
#endif



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static float s_quaternion[ALGO_QUAD] = {0.0f}; /* 四元数 q0 q1 q2 q3*/
//static float s_eg_integration[ALGO_DIM] = {0.0f}; /* 加计误差积分 */
ALGO_IMU_PARA_T s_imu_para = {0};

/********************************** 函数声明区 *********************************/
static int quaternion_lock(void);
static int quaternion_unlock(void);
static int euler2quaternion(float *quaternion, const float *euler);
static int quaternion2euler(float *euler, const float *quaternion);
static int get_ms(unsigned long *ms);

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : imu_set
 * 负责人  : 彭鹏
 * 创建日期: 20150821
 * 函数功能: 融合算法参数设置
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int imu_set(ALGO_IMU_PARA_T *imu_para)
{
    float euler[3] = {0.0f}; /* 欧拉角 全零 */

    /* 由欧拉角球四元数初始值 */
    euler2quaternion(s_quaternion, euler);
    /* 初始化工作模式 */
    memcpy(&s_imu_para, imu_para, sizeof(ALGO_IMU_PARA_T));

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : imu_start
 * 负责人  : 彭鹏
 * 创建日期: 20150824
 * 函数功能: 启动融合算法线程
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int imu_start(void)
{
#ifdef X86_64
#else
#endif
    return 0;
}

/*******************************************************************************
 *
 * 函数名  : imu_fusion3axis
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
 * 其 它:    使用gyro输出的角速度积分获取当前姿态
 *
 ******************************************************************************/
static int imu_fusion3axis(const float *gyro)
{
    float wx = gyro[0];
    float wy = gyro[1];
    float wz = gyro[2];

    float q0_diff = 0.0f;
    float q1_diff = 0.0f;
    float q2_diff = 0.0f;
    float q3_diff = 0.0f;

    float q_norm = 0.0f;
    float half_period = 0.0f;

    static unsigned long last_ms = 0;
    unsigned long now_ms = 0;

    /* 计算积分时间 */
    get_ms(&now_ms);
    half_period = 0.5f * (now_ms - last_ms);
    last_ms = now_ms;

    /* 角度转弧度 */
    wx = math_angle2arc(wx);
    wy = math_angle2arc(wy);
    wz = math_angle2arc(wz);

    quaternion_lock();

    /* DEBUG_P("%f,%f,%f,%f\n", s_quaternion[0], s_quaternion[1], s_quaternion[2], s_quaternion[3]);*/

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
    DEBUG_P("%7.4f,%7.4f,%7.4f,%7.4f => %7.4f,%7.4f,%7.4f,%7.4f\n", 
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
    DEBUG_P("%7.4f,%7.4f,%7.4f,%7.4f => %7.4f,%7.4f,%7.4f,%7.4f\n", 
            q0_last, q1_last, q2_last, q3_last,
            s_quaternion[0], s_quaternion[1], s_quaternion[2], s_quaternion[3]);
#endif

    return 0;
}




/*******************************************************************************
 *
 * 函数名  : euler2quaternion
 * 负责人  : 彭鹏
 * 创建日期: 20150821
 * 函数功能: 欧拉角转四元数
 *
 * 输入参数: euler   欧拉角
 *           0 pitch 俯仰角 x轴
 *           1 roll  翻滚角 y轴
 *           2 yaw   偏航角 z轴
 * 输出参数: quaternion 姿态四元数
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
static int euler2quaternion(float *quaternion, const float *euler)
{
    float half_pitch = 0.0f;
    float half_roll = 0.0f;
    float half_yaw = 0.0f;

    half_pitch = euler[0] / 2;
    half_roll = euler[1] / 2;
    half_yaw = euler[2] / 2;

    quaternion[0] = cos(half_pitch)*cos(half_roll)*cos(half_yaw) + sin(half_pitch)*sin(half_roll)*sin(half_yaw);
    quaternion[1] = sin(half_pitch)*cos(half_roll)*cos(half_yaw) - cos(half_pitch)*sin(half_roll)*sin(half_yaw);
    quaternion[2] = cos(half_pitch)*sin(half_roll)*cos(half_yaw) + sin(half_pitch)*cos(half_roll)*sin(half_yaw);
    quaternion[3] = cos(half_pitch)*cos(half_roll)*sin(half_yaw) - sin(half_pitch)*sin(half_roll)*cos(half_yaw);

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : quaternion2euler
 * 负责人  : 彭鹏
 * 创建日期: 20150821
 * 函数功能: 四元数转欧拉角
 *
 * 输入参数: quaternion 姿态四元数
 *
 * 输出参数: euler   欧拉角
 *           0 pitch 俯仰角 x轴
 *           1 roll  翻滚角 y轴
 *           2 yaw   偏航角 z轴
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
static int quaternion2euler(float *euler, const float *quaternion)
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


    pitch = atan2(q2*q3 + q0*q1, q0*q0 + q3*q3 - 0.5f);
    roll  = -asin(2*(q1*q3 - q0*q2));
    yaw   = atan2(q1*q2 + q0*q3, q0*q0 + q1*q1 - 0.5f);

    euler[0] = pitch;
    euler[1] = roll;
    euler[2] = yaw;

    /* FIXME: 是否是全姿态的，反三角函数计算出的角度是否需要修正? */

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
 * 函数名  : get_ms
 * 负责人  : 彭鹏
 * 创建日期: 20150824
 * 函数功能: 获取当前进程的ms值
 *
 * 输入参数: 无
 * 输出参数: ms    毫秒值
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
static int get_ms(unsigned long *ms)
{
#ifdef X86_64
    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    /* FIXME: 溢出 */
    *ms = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
#else

#endif
    return 0;
}

