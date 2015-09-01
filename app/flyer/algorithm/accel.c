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
    float theta_a = 0.0f;
    float phi_a = 0.0f;
    float theta_g = 0.0f;
    float phi_g = 0.0f;
    float theta_r = 0.0f;
    float phi_r = 0.0f;
    float psi = 0.0f;
    float euler[ALGO_DIM] = {0.0f};

    for(int i=0;i<ALGO_DIM;i++)
    {
        a[i] = accel[i];
    }
    attidude_get_matrix_by_column(g, 3);

    /* 直接姿态 */
    /* theta */
    a[0] = 0; /* yoz 投影 x值设置为零 */
    g[0] = 0; /* yoz 投影 x值设置为零 */
    math_vetor_angle(&theta_a, a, g);
    /* phi */
    a[1] = 0; /* xoz 投影 y值设置为零 */
    g[1] = 0; /* xoz 投影 y值设置为零 */
    math_vetor_angle(&phi_a, a, g);

#if 1
    /* 获取积分姿态 */
    attidude_get_level(&theta_g, &phi_g);

    /* 姿态融合 */
    theta_r = theta_g + (theta_a - theta_g) * ALGO_ACCEL_PITCH_RATE;
    phi_r = phi_g + (phi_a - phi_g) * ALGO_ACCEL_ROLL_RATE;

    /* 转换为四元数 */
    attidude_get_psi(&psi);

    euler[0] = theta_r;
    euler[1] = phi_r;
    euler[2] = psi; 
    
    attidude_euler2quaternion(q, euler);
    attidude_set_quaternion(q);
#endif

    printf("%7.4f,%7.4f + ", math_arc2angle(theta_a), math_arc2angle(phi_a));
    printf("%7.4f,%7.4f => ", math_arc2angle(theta_g), math_arc2angle(phi_g));
    printf("%7.4f,%7.4f,%7.4f ", math_arc2angle(theta_r), math_arc2angle(phi_r), math_arc2angle(psi));
    printf("%7.4f,%7.4f,%7.4f,%7.4f\n", q[0],q[1],q[2],q[3]);

    return 0;
}

