/******************************************************************************
 *
 * 文件名  ： pc.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150824 
 * 版本号  ： v1.0
 * 文件描述： PC模拟需要的功能
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： TODO: 三个传感器的仿真数据有一定的相关性,需要建模
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "algo.h"
#include "algo_math.h"



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static float s_attidude[ALGO_DIM] = {0.0f};

static float s_gyro[ALGO_DIM] = {0.0f};
static float s_accel[ALGO_DIM] = {0.0f};
static float s_mag[ALGO_DIM] = {0.0f};

/********************************** 函数声明区 *********************************/
static float rand_range(float min, float max);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/* 增加陀螺积分误差模型 */
/* 增加加计干扰误差模型 */
void *pc_loop(void *argv)
{
    float gyro_snr = 10.0f;

    while(1)
    { 
        /* TODO: */
        s_gyro[0] = rand_range(-1.0f, 1.0f)/gyro_snr;
        s_gyro[1] = rand_range(-1.0f, 1.0f)/gyro_snr;
        s_gyro[2] = rand_range(-1.0f, 1.0f)/gyro_snr;

        s_accel[0] = 0;

        s_mag[0] = 0;
        
        s_attidude[0] += s_gyro[0];

        //printf("%s,%d:pc_loop.\n", __FILE__, __LINE__);
        usleep(1000); /* 1ms 更新一次姿态 */
    }
    return NULL;
}

int pc_init(void)
{
    pthread_t tid;
    int rst = 0;
    rst = pthread_create(&tid, NULL, pc_loop, NULL);
    if(0 != rst)
    { 
        printf("%s,%d:pc_init\n", __FILE__, __LINE__);
        return -1;
    }

    return 0;
}

int pc_get_gyro(float *gyro)
{
    gyro[0] = rand_range(-1.0f, 1.0f);
    gyro[1] = rand_range(-1.0f, 1.0f);
    gyro[2] = rand_range(-1.0f, 1.0f);
    return 0;
}

int pc_get_accel(float *accel)
{
    float snr = 1000.0f;
    accel[0] = rand_range(-1.0f, 1.0f) / snr;
    accel[1] = rand_range(-1.0f, 1.0f) / snr;
    accel[2] = 1.0f + rand_range(-1.0f, 1.0f) / snr;

    return 0;
}

int pc_get_mag(float *mag)
{
    float theta = 30;
    float snr = 1000.0f;

    mag[0] = cos(math_angle2arc(theta)) + rand_range(-1.0f, 1.0f) / snr;
    mag[1] = rand_range(-1.0f, 1.0f) / snr;
    mag[2] = -sin(math_angle2arc(theta)) + rand_range(-1.0f, 1.0f) / snr;

    return 0;
}

static float rand_range(float min, float max)
{ 
    float value = 0.0f;
    float range = 0.0f;

    range = max - min;
    value = min + range * rand() / RAND_MAX;

    return value;
}

