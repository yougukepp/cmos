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
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include "algorithm.h"



/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static float rand_range(float min, float max);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : main
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 主函数
 *
 * 输入参数: argc - 参数个数
 *           argv - 命令行参数数组
 *
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int main(int argc, char *argv[])
{
    int i = 0;
#if 1
    float gyro[3] = {0.0f};
    float attitude[3] = {0.0f};

    while(1)
    {
        gyro[0] = rand_range(-1.0f, 1.0f);
        gyro[1] = rand_range(-1.0f, 1.0f);
        gyro[2] = rand_range(-1.0f, 1.0f); 

        if(i>10)
        {
            gyro[0] = 0;
            gyro[1] = 0;
            gyro[2] = 0;
        }

        imu_update(gyro); 
        get_attitude(attitude);
        printf("wx:%7.4f, wy:%7.4f, wz:%7.4f => ", 
                gyro[0], gyro[1], gyro[2]);
        printf("pitch:%7.4f, roll:%7.4f, yaw:%7.4f\n", 
                arc2angle(attitude[0]), arc2angle(attitude[1]), arc2angle(attitude[2]));
        sleep(1);
        i++;
    }

    printf("test done!\n");
#else
    for(i=0;i<=360;i+=10)
    {
        float arc = angle2arc(i);
        printf("%d=>%7.4f,", i, arc);
        printf("%7.4f=>%7.4f\n", arc, arc2angle(arc));
    }

    for(i=0;i<20;i++)
    {
        printf("%d:rand_range(-1,1) = %7.4f\n", i, rand_range(-1,1));
    }
#endif
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

