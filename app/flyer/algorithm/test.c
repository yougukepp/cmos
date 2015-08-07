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
#include <math.h>

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
    float gyro[3] = {0.0f};
    float attitude[3] = {0.0f}; 

    float max = 0;

    while(1)
    {
        gyro[0] = rand_range(-1.0f, 1.0f);
        gyro[1] = rand_range(-1.0f, 1.0f);
        gyro[2] = rand_range(-1.0f, 1.0f); 

        imu_update(gyro); 
        imu_get_attitude(attitude);
        printf("wx:%7.4f, wy:%7.4f, wz:%7.4f => ", 
                gyro[0], gyro[1], gyro[2]);
        printf("pitch:%7.4f, roll:%7.4f, yaw:%7.4f, max=%7.4f\n", 
                math_arc2angle(attitude[0]), math_arc2angle(attitude[1]), math_arc2angle(attitude[2]), max);
        usleep(ALGO_GYRO_PERIOD * 1000000);

        /* 找最大值 */
        if(max < fabs(math_arc2angle(attitude[0])))
        {
            max = fabs(math_arc2angle(attitude[0]));
        }
        if(max < fabs(math_arc2angle(attitude[1])))
        {
            max = fabs(math_arc2angle(attitude[1]));
        }
        if(max < fabs(math_arc2angle(attitude[2])))
        {
            max = fabs(math_arc2angle(attitude[2]));
        }
        i++;
    }

    printf("test done!\n");
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

