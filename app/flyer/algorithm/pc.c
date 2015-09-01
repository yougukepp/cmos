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
#include <stdlib.h>



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static s_attidude[ALGO_DIM] = {0.0f};

/********************************** 函数声明区 *********************************/
static float rand_range(float min, float max);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
int pc_get_gyro(float *gyro)
{
    gyro[0] = rand_range(-1.0f, 1.0f);
    gyro[1] = rand_range(-1.0f, 1.0f);
    gyro[2] = rand_range(-1.0f, 1.0f);
    return 0;
}

int pc_get_accel(float *accel)
{
    float snr = 100.0f;
    accel[0] = rand_range(-1.0f, 1.0f) / snr;
    accel[1] = rand_range(-1.0f, 1.0f) / snr;
    accel[2] = 1.0f + rand_range(-1.0f, 1.0f) / snr;

    return 0;
}

int pc_get_mag(float *mag)
{
    mag[0] = rand_range(-1.0f, 1.0f);
    mag[1] = rand_range(-1.0f, 1.0f);
    mag[2] = rand_range(-1.0f, 1.0f);

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

