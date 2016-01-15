/******************************************************************************
 *
 * 文件名  ： fusion
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20160116 
 * 版本号  ： v1.0
 * 文件描述： 融合算法
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "fusion.h"
#include "lib_math.h"
#include "imu.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static f32_T s_quaternion[MATH_QUAD] = {0.0f}; /* 四元数 q0 q1 q2 q3*/


/********************************** 函数声明区 *********************************/
static void lock(void);
static void unlock(void);

static void set_quaternion(f32_T *q);
static void get_quaternion(f32_T *q);

static void gyro_fusion(const f32_T *gyro, uint32_T time);

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void task_gyro(void)
{ 
    f32_T gyro[MATH_THREE];
    uint32_T time;
    if(imu_get_gyro(gyro, &time))
    {
        /* 3轴 融合 */
        gyro_fusion(gyro, time);
    }
}

static void gyro_fusion(const f32_T *gyro, uint32_T time)
{
    f32_T wx = gyro[0];
    f32_T wy = gyro[1];
    f32_T wz = gyro[2];

    f32_T q[MATH_QUAD] = {0.0f};
    f32_T q_norm[MATH_QUAD] = {0.0f};

    f32_T q0_diff = 0.0f;
    f32_T q1_diff = 0.0f;
    f32_T q2_diff = 0.0f;
    f32_T q3_diff = 0.0f;

    f32_T half_period = 0.0f;

    static uint32_T last_time = 0;
    uint32_T now_time = 0;

    /* 计算积分时间 */
    if(0 == last_time) /* 首次不积分 */
    {
        last_time = time;
        return;
    }
    now_time = time;

    half_period = HALF_PERIOD_CONST * (now_time - last_time);
    last_time = now_time; 

    /* 角度转弧度 */
    wx = math_angle2arc(wx);
    wy = math_angle2arc(wy);
    wz = math_angle2arc(wz); 
    
    get_quaternion(q);

    /* 微分 */
    q0_diff =  -half_period * ( q[1] * wx + q[2] * wy + q[3] * wz);
    q1_diff =   half_period * ( q[0] * wx - q[3] * wy + q[2] * wz);
    q2_diff =   half_period * ( q[3] * wx + q[0] * wy - q[1] * wz);
    q3_diff =   half_period * (-q[2] * wx + q[1] * wy + q[0] * wz);

#if 0
    debug_log("q:%7.5f,%7.5f,%7.5f,%7.5f.\n", q[0], q[1], q[2], q[3]);
    debug_log("w:%5.2f,%5.2f,%5.2f.\n", wx,wy,wz);
    debug_log("half_period:%5.2f.\n", half_period);
    debug_log("diff:%7.5f,%7.5f,%7.5f,%7.5f.\n", q0_diff, q1_diff, q2_diff, q3_diff);
    debug_log("\n");
#endif

    /* 积分 */
    q[0] += q0_diff;
    q[1] += q1_diff;
    q[2] += q2_diff;
    q[3] += q3_diff;

    /* 归一化 */ 
    math_norm(q_norm, q, 4);

    set_quaternion(q_norm);

    return;
}

/* FIXME:考虑是否有并发问题 */
static void get_quaternion(f32_T *q)
{ 
    lock();

    q[0] = s_quaternion[0];
    q[1] = s_quaternion[1];
    q[2] = s_quaternion[2];
    q[3] = s_quaternion[3];

    unlock();
}

static void set_quaternion(f32_T *q)
{
    lock();

    s_quaternion[0] = q[0];
    s_quaternion[1] = q[1];
    s_quaternion[2] = q[2];
    s_quaternion[3] = q[3];

    unlock();
}

inline static void lock(void)
{}

inline static void unlock(void)
{}

