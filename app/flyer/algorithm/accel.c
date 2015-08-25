/******************************************************************************
 *
 * 文件名  ： accel.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150825 
 * 版本号  ： v1.0
 * 文件描述： 加计的姿态解算
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
#include "accel.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void *accel_loop(void *argv)
{
    float accel[ALGO_DIM] = {0.0f};

    while(1)
    {
        delay_ms(ALGO_ACCEL_PERIOD);
        ALGO_ACCEL_GET_DATA(accel);
        ALGO_ACCEL_FUSION(accel);
    }

    return NULL;
}

int accel_fusion(const float *accel)
{
    float a[ALGO_DIM] = {0.0f};
    float g[ALGO_DIM] = {0.0f};
    float q[ALGO_QUAD] = {0.0f};
    float pitch_a = 0.0f;
    float roll_a = 0.0f;
    float pitch_g = 0.0f;
    float roll_g = 0.0f;
    float pitch_r = 0.0f;
    float roll_r = 0.0f;
    float yaw = 0.0f;
    float euler[ALGO_DIM] = {0.0f};

    for(int i=0;i<ALGO_DIM;i++)
    {
        a[i] = accel[i];
    }
    attidude_get_matrix_by_column(g, 3);

    /* 直接姿态 */
    /* pitch */
    a[0] = 0; /* yoz 投影 x值设置为零 */
    g[0] = 0; /* yoz 投影 x值设置为零 */
    math_vetor_angle(&pitch_a, a, g);
    /* roll */
    a[1] = 0; /* xoz 投影 y值设置为零 */
    g[1] = 0; /* xoz 投影 y值设置为零 */
    math_vetor_angle(&roll_a, a, g);

#if 1
    /* 获取积分姿态 */
    attidude_get_level(&pitch_g, &roll_g);

    /* 姿态融合 */
    pitch_r = pitch_g + (pitch_a - pitch_g) * ALGO_ACCEL_PITCH_RATE;
    roll_r = roll_g + (roll_a - roll_g) * ALGO_ACCEL_ROLL_RATE;

    /* 转换为四元数 */
    attidude_get_yaw(&yaw);

    euler[0] = pitch_r;
    euler[1] = roll_r;
    euler[2] = yaw; 
    
    attidude_euler2quaternion(q, euler);
    attidude_set_quaternion(q);
#endif

    printf("%7.4f,%7.4f + ", math_arc2angle(pitch_a), math_arc2angle(roll_a));
    printf("%7.4f,%7.4f => ", math_arc2angle(pitch_g), math_arc2angle(roll_g));
    printf("%7.4f,%7.4f,%7.4f ", math_arc2angle(pitch_r), math_arc2angle(roll_r), math_arc2angle(yaw));
    printf("%7.4f,%7.4f,%7.4f,%7.4f\n", q[0],q[1],q[2],q[3]);

    return 0;
}

