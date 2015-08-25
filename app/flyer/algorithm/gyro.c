/******************************************************************************
 *
 * 文件名  ： 
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： 
 * 文件描述： 
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "algo.h"
#include "port.h"
#include "attidude.h"
#include "algo_math.h"
#include "gyro.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void *gyro_loop(void *argv)
{
    float gyro[ALGO_DIM] = {0.0f};

    while(1)
    {
        delay_ms(ALGO_GYRO_PERIOD);
        ALGO_GYRO_GET_DATA(gyro);
        ALGO_GYRO_FUSION(gyro);
    }

    return NULL;
}

/*******************************************************************************
 *
 * 函数名  : gyro_fusion
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
int gyro_fusion(const float *gyro)
{
    float wx = gyro[0];
    float wy = gyro[1];
    float wz = gyro[2];

    float q[ALGO_QUAD] = {0.0f};
    float q_norm[ALGO_QUAD] = {0.0f};

    float q0_diff = 0.0f;
    float q1_diff = 0.0f;
    float q2_diff = 0.0f;
    float q3_diff = 0.0f;

    float half_period = 0.0f;

    static unsigned long last_ms = 0;
    unsigned long now_ms = 0;

    /* 计算积分时间 */
    if(0 == last_ms) /* 首次不积分 */
    {
        get_ms(&last_ms);
        return 0;
    }
    get_ms(&now_ms);
    half_period = 0.0005f * (now_ms - last_ms); /* ms => s 为单位 half 所以除以1000再除以2 */
    last_ms = now_ms; 

    /* 角度转弧度 */
    wx = math_angle2arc(wx);
    wy = math_angle2arc(wy);
    wz = math_angle2arc(wz); 
    
    attidude_get_quaternion(q);

    /* 微分 */
    q0_diff =  -half_period * (q[1] * wx + q[2] * wy + q[3] * wz);
    q1_diff =   half_period * (q[0] * wx + q[2] * wz - q[3] * wy);
    q2_diff =   half_period * (q[0] * wy - q[1] * wz + q[3] * wx);
    q3_diff =   half_period * (q[0] * wz + q[1] * wy - q[2] * wx);

#if 0
    algo_printf("q:%7.5f,%7.5f,%7.5f,%7.5f.\n", q[0], q[1], q[2], q[3]);
    algo_printf("w:%5.2f,%5.2f,%5.2f.\n", wx,wy,wz);
    algo_printf("half_period:%5.2f.\n", half_period);
    algo_printf("diff:%7.5f,%7.5f,%7.5f,%7.5f.\n", q0_diff, q1_diff, q2_diff, q3_diff);
    algo_printf("\n");
#endif

    /* 积分 */
    q[0] += q0_diff;
    q[1] += q1_diff;
    q[2] += q2_diff;
    q[3] += q3_diff;

    /* 归一化 */ 
    math_norm(q_norm, q, 4);

    attidude_set_quaternion(q_norm);

    return 0;
}

