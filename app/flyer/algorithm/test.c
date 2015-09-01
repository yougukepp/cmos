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
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "algo.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


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
    float theta = 0;
    float phi = 0;
    float psi = 0;

    float max = 0.0f;

    algo_init();
    algo_start();

    usleep(10000); /* 睡眠10ms 使打印好看 */
    printf("\n");
    while(1)
    {
        algo_get_level(&theta, &phi);
        printf("theta:%7.4f,", theta);
        printf("phi:%7.4f,", phi);
        algo_get_psi(&psi);
        printf("psi:%7.4f <===", psi); 
        
        algo_print_quaternion();
        if(max < fabs(theta))
        {
            max = fabs(theta);
        }
        if(max < fabs(phi))
        {
            max = fabs(phi);
        }
        if(max < fabs(psi))
        {
            max = fabs(psi);
        }

        printf("\tmax:%7.4f\n", max); 
        usleep(1000000);
    }

    printf("test done!\n");
    return 0;
}

