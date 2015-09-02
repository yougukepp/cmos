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
#include "mag.h"



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
void *mag_loop(void *argv)
{
    float mag[ALGO_DIM] = {0.0f};

    while(1)
    {
        delay_ms(ALGO_MAG_PERIOD);
        ALGO_MAG_GET_DATA(mag);
        ALGO_MAG_FUSION(mag);
    }

    return NULL;
}

int mag_fusion(const float *mag)
{
    float m[ALGO_DIM] = {0.0f};
    float theta = 0.0f;
    float phi = 0.0f;
    float psi = 0.0f;
    float psi_m = 0.0f;
    float psi_g = 0.0f;
    float euler[ALGO_DIM] = {0.0f};
    float q[ALGO_QUAD] = {0.0f};

    for(int i=0;i<ALGO_DIM;i++)
    {
        m[i] = mag[i];
    }

    /* 直接姿态 */
    /* psi */
    psi_m = atan2(m[0], m[1]);

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

    return 0;
}

