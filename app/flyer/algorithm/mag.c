/******************************************************************************
 *
 * 文件名  ： mag.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150825 
 * 版本号  ： v1.0
 * 文件描述： 磁计的姿态解算
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <math.h>

#include "algo.h"
#include "port.h"
#include "attidude.h"
#include "algo_math.h"
#include "accel.h"
#include "mag.h"



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void *mag_loop(void *argv)
{
    while(1)
    {
        delay_ms(ALGO_MAG_PERIOD);
        ALGO_MAG_FUSION();
    }

    return NULL;
}

int mag_fusion()
{
    float e[ALGO_DIM] = {0.0f};
    float accel[ALGO_DIM] = {0.0f};
    float mag[ALGO_DIM] = {0.0f};
    float theta = 0.0f;
    float phi = 0.0f;
    float psi = 0.0f;
    float psi_m = 0.0f;
    float psi_g = 0.0f;
    float euler[ALGO_DIM] = {0.0f};
    float q[ALGO_QUAD] = {0.0f};

    /* 求指东针 */
    ALGO_ACCEL_GET_DATA(accel);
    ALGO_MAG_GET_DATA(mag); 
    math_vector3_cross_product(e, accel, mag);

#if 0
    printf("a:%7.4f,%7.4f,%7.4f + ", accel[0], accel[1], accel[2]);
    printf("m:%7.4f,%7.4f,%7.4f => ", mag[0], mag[1], mag[2]);
    printf("e:%7.4f,%7.4f,%7.4f\n", e[0], e[1], e[2]);
#endif

    /* 直接姿态 */
    /* psi */
    psi_m = atan2(e[0], e[1]);

    /* 获取积分姿态 */
    attidude_get_psi(&psi_g);

    /* 姿态融合 */
    psi = psi_g + (psi_m - psi_g) * ALGO_MAG_PSI_RATE;

    /* 转换为四元数 */
    attidude_get_level(&theta, &phi);

    euler[0] = theta;
    euler[1] = phi;
    euler[2] = psi; 
    
    attidude_euler2quaternion(q, euler);
    attidude_set_quaternion(q);

#if 0
    static int pp = 0;
    printf("%d:\n", pp);
    printf("%7.4f + %7.4f => %7.4f,", math_arc2angle(psi_g), math_arc2angle(psi_m), math_arc2angle(psi));
    printf("%7.4f,%7.4f,%7.4f ", math_arc2angle(theta), math_arc2angle(phi), math_arc2angle(psi));
    printf("%7.4f,%7.4f,%7.4f,%7.4f\n", q[0],q[1],q[2],q[3]);
    printf("\n");
    if(10000 == pp)
    {
        while(1);
    }

    pp++;
#endif

    return 0;
}

