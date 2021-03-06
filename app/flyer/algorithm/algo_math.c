/******************************************************************************
 *
 * 文件名  ： algo_math.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150824 
 * 版本号  ： v1.0
 * 文件描述： 数学库
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <float.h>
#include <math.h>
#include "algo.h"
#include "algo_math.h"
#include "port.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : math_inv_sqrt
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 求平方根倒数 
 *           快速算法
 *
 * 输入参数: 待求的值
 *
 * 输出参数: 无
 *
 * 返回值:   x 的平方根倒数
 *          -1 出错
 * 调用关系: 无
 * 其 它:    http://www.matrix67.com/data/InvSqrt.pdf
 *           参数不得为零
 *
 ******************************************************************************/
inline float math_inv_sqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x;          // get bits for floating value
    i =  0x5f375a86 - (i>>1);   // gives initial guess
    x = *(float*)&i;            // convert bits back to float
    x = x * (1.5f - xhalf*x*x); // Newton step
    return x;
}

/* 归一化 */
inline int math_norm(float *dst, const float *src, int dim)
{
    float square_sum = 0.0f; 
    float norm = 0.0f; 
    
    for(int i=0;i<dim;i++)
    {
        square_sum += (src[i] * src[i]);
    } 
    
    norm = math_inv_sqrt(square_sum);
    if(0 == norm)
    {
        algo_printf("%s,%d:math_norm error\n!", __FILE__, __LINE__);
        return -1;
    }

    for(int i=0;i<dim;i++)
    {
        dst[i] = src[i] * norm;
    } 

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : math_angle2arc
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 角度转弧度
 *
 * 输入参数: 待转的角度
 * 输出参数: 无
 *
 * 返回值:   弧度
 * 调用关系: 无
 * 其 它:    x 为 0 ~ 180
 *
 ******************************************************************************/
inline float math_angle2arc(float x)
{
    return ALGO_ANGLE2ARC_RATE * x;
}

/*******************************************************************************
 *
 * 函数名  : math_arc2angle
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 弧度转角度
 *
 * 输入参数: 待转的弧度
 * 输出参数: 无
 *
 * 返回值:   角度
 * 调用关系: 无
 * 其 它:    x 为 -pi ~ pi
 *
 ******************************************************************************/
inline float math_arc2angle(float x)
{
    return ALGO_ARC2ANGLE_RATE * x;
}

/*******************************************************************************
 *
 * 函数名  : math_vector3_cross_product
 * 负责人  : 彭鹏
 * 创建日期: 20150731
 * 函数功能: 向量叉乘
 *
 * 输入参数: a 向量a
 *           b 向量b
 *
 * 输出参数: product 叉乘值
 *
 * 返回值:    0 成功
 *           -1 失败
 * 调用关系: 无
 * 其 它:    顺序不可反
 *
 ******************************************************************************/
inline int math_vector3_cross_product(float *product, const float *a, const float *b)
{

    /*
     * 行列式定义
     * |   i     j    k  |
     * | a[0]  a[1] a[2] |
     * | b[0]  b[1] b[2] |
     *
     *
     * = a[1]*b[2]*i + a[2]*b[0]*j + a[0]*b[1]*k
     *  -a[2]*b[1]*i - a[0]*b[2]*j - a[1]*b[0]*k
     *
     */
    product[0] = a[1] * b[2] - a[2] * b[1];
    product[1] = a[2] * b[0] - a[0] * b[2];
    product[2] = a[0] * b[1] - a[1] * b[0];

    return 0;
}

/* 点乘值 */
inline int math_vector3_dot_product(float *product, const float *a, const float *b)
{
    *product = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];

    return 0;
}

/* 求向量夹角 */
int math_vetor_angle(float *angle, const float *a, const float *b)
{
    float a_norm[ALGO_DIM] = {0.0f};
    float b_norm[ALGO_DIM] = {0.0f};
    float cross_product[ALGO_DIM] = {0.0f};
    float dot_product = 0.0f;
    float rst_angle = 0.0f;

    math_norm(a_norm, a, 3);
    math_norm(b_norm, b, 3);
    math_vector3_dot_product(&dot_product, a_norm, b_norm); 
    math_vector3_cross_product(cross_product, a_norm, b_norm); 
    
    rst_angle = acosf(dot_product);
    if(cross_product[2] < 0.0f) /* 外积决定符号 */
    {
        rst_angle = -rst_angle;
    }

    *angle = rst_angle;

    return 0;
}

