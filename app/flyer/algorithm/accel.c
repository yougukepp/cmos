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
    float pitch = 0.0f;
    float roll = 0.0f;

    for(int i=0;i<ALGO_DIM;i++)
    {
        a[i] = accel[i];
    }
    attidude_get_matrix_by_column(g, 3);

    /* pitch */
    a[0] = 0; /* yoz 投影 x值设置为零 */
    g[0] = 0; /* yoz 投影 x值设置为零 */
    math_vetor_angle(&pitch, a, g);
    printf("accel:%7.4f,%7.4f,%7.4f\n", accel[0], accel[1], accel[2]);
    printf("gyro :%7.4f,%7.4f,%7.4f\n", g[0], g[1], g[2]);
    printf("pitch:%7.4f\n", math_arc2angle(pitch));
    printf("\n");

    /* roll */
    a[1] = 0; /* xoz 投影 y值设置为零 */
    g[1] = 0; /* xoz 投影 y值设置为零 */
    math_vetor_angle(&roll, a, g);

    //printf("pitch:%7.4f,roll:%7.4f\n", math_arc2angle(pitch), math_arc2angle(roll));

    return 0;
}

