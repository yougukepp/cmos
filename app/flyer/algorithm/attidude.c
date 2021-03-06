/******************************************************************************
 *
 * 文件名  ： attidude.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150824
 * 版本号  ： v1.0
 * 文件描述： 实现四元数欧拉角数据结构并完成两者的转换
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdio.h>
#include <math.h>

#include "algo.h"
#include "port.h"
#include "attidude.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static float s_quaternion[ALGO_QUAD] = {0.0f}; /* 四元数 q0 q1 q2 q3*/
MUTEX_T s_quaternion_mutex; /* 四元数锁 */

static int quaternion_lock(MUTEX_T *mutex);
static int quaternion_unlock(MUTEX_T *mutex);

/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : quaternion_lock
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 锁定
 *
 * 输入参数: 锁变量
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    可能会阻塞 避免获取四元数 时产生不一致
 *
 ******************************************************************************/
inline static int quaternion_lock(MUTEX_T *mutex)
{
    if (0 != mutex_lock(mutex))
    {
        algo_printf("%s,%d:mutex_lock error!\n", __FILE__,__LINE__);
    }

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : quaternion_unlock
 * 负责人  : 彭鹏
 * 创建日期: 20150729
 * 函数功能: 解除锁定
 *
 * 输入参数: 锁变量
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    与quaternion_lock成对使用
 *
 ******************************************************************************/
inline static int quaternion_unlock(MUTEX_T *mutex)
{
    if (0 != mutex_unlock(mutex))
    {
        algo_printf("%s,%d:mutex_unlock error!\n", __FILE__,__LINE__);
    }
    return 0;
}

/*******************************************************************************
 *
 * 函数名  : attidude_euler2quaternion
 * 负责人  : 彭鹏
 * 创建日期: 20150821
 * 函数功能: 欧拉角转四元数
 *
 * 输入参数: euler   欧拉角
 *           0 theta 俯仰角 x轴
 *           1 phi  翻滚角 y轴
 *           2 psi   偏航角 z轴
 * 输出参数: quaternion 姿态四元数
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int attidude_euler2quaternion(float *quaternion, const float *euler)
{
    float half_theta = 0.0f;
    float half_phi = 0.0f;
    float half_psi = 0.0f;

    half_theta = euler[0] / 2;
    half_phi = euler[1] / 2;
    half_psi = euler[2] / 2;

    quaternion[0] = cos(half_theta)*cos(half_phi)*cos(half_psi) + sin(half_theta)*sin(half_phi)*sin(half_psi);
    quaternion[1] = sin(half_theta)*cos(half_phi)*cos(half_psi) - cos(half_theta)*sin(half_phi)*sin(half_psi);
    quaternion[2] = cos(half_theta)*sin(half_phi)*cos(half_psi) + sin(half_theta)*cos(half_phi)*sin(half_psi);
    quaternion[3] = cos(half_theta)*cos(half_phi)*sin(half_psi) - sin(half_theta)*sin(half_phi)*cos(half_psi); 

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : attidude_quaternion2euler
 * 负责人  : 彭鹏
 * 创建日期: 20150821
 * 函数功能: 四元数转欧拉角
 *
 * 输入参数: quaternion 姿态四元数
 *
 * 输出参数: euler   欧拉角
 *           0 theta 俯仰角 x轴
 *           1 phi  翻滚角 y轴
 *           2 psi   偏航角 z轴
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    暂时未使用
 *
 ******************************************************************************/
int attidude_quaternion2euler(float *euler, const float *quaternion)
{
    float theta = 0.0f;
    float phi = 0.0f;
    float psi = 0.0f;

    float q[ALGO_QUAD] = {0.0f};

    attidude_get_quaternion(q);

    theta = atan2(q[2]*q[3] + q[0]*q[1], q[0]*q[0] + q[3]*q[3] - 0.5f);
    phi  = -asin(2*(q[1]*q[3] - q[0]*q[2]));
    psi   = atan2(q[1]*q[2] + q[0]*q[3], q[0]*q[0] + q[1]*q[1] - 0.5f);

    euler[0] = theta;
    euler[1] = phi;
    euler[2] = psi;

    /* FIXME: 是否是全姿态的，反三角函数计算出的角度是否需要修正? */

    return 0;
}

/* 提高效率 欧拉角 分为水平 和偏航 */
int attidude_get_level(float *theta, float *phi)
{
    float q[ALGO_QUAD] = {0.0f};

    attidude_get_quaternion(q);

    *theta = atan2(q[2]*q[3] + q[0]*q[1], q[0]*q[0] + q[3]*q[3] - 0.5f);
    *phi  = -asin(2*(q[1]*q[3] - q[0]*q[2]));

    return 0;
}

int attidude_get_psi(float *psi)
{
    float q[ALGO_QUAD] = {0.0f};

    attidude_get_quaternion(q);
    *psi   = atan2(q[1]*q[2] + q[0]*q[3], q[0]*q[0] + q[1]*q[1] - 0.5f);

    return 0;
}

/* 出于效率考虑,暂时未使用 */
int attidude_get_matrix(float *data)
{
    float q[ALGO_QUAD] = {0.0f};

    attidude_get_quaternion(q);

    /*
     * 如下排列:
     * 0 1 2
     * 3 4 5
     * 6 7 8
     * */
    data[0] = 2*(q[0]*q[0] + q[1]*q[1]) - 1;
    data[1] = 2*(q[1]*q[2] + q[0]*q[3]);
    data[2] = 2*(q[1]*q[3] - q[0]*q[2]);

    data[3] = 2*(q[1]*q[2] - q[0]*q[3]);
    data[4] = 2*(q[0]*q[0] + q[2]*q[2]) - 1;
    data[5] = 2*(q[2]*q[3] + q[0]*q[1]);

    data[6] = 2*(q[1]*q[3] + q[0]*q[2]);
    data[7] = 2*(q[2]*q[3] - q[0]*q[1]);
    data[8] = 2*(q[0]*q[0] + q[3]*q[3]) - 1;

    return 0;
}

/* 提高效率 欧拉角 转旋转矩阵 按照列 */
int attidude_get_matrix_by_column(float *data, int column)
{
    float q[ALGO_QUAD] = {0.0f};

    attidude_get_quaternion(q);

    if(2 == column)
    {
        data[0] = 2*(q[1]*q[2] + q[0]*q[3]);
        data[1] = 2*(q[0]*q[0] + q[2]*q[2]) - 1;
        data[2] = 2*(q[2]*q[3] - q[0]*q[1]);
    }
    else if(3 == column)
    {
        data[0] = 2*(q[1]*q[3] - q[0]*q[2]);
        data[1] = 2*(q[2]*q[3] + q[0]*q[1]); 
        data[2] = 2*(q[0]*q[0] + q[3]*q[3]) - 1;
    }
    else
    {
        data[0] = 0;
        data[0] = 0;
        data[0] = 0;
        return -1;
    }

    return 0;
}

int attidude_get_quaternion(float *quaternion)
{
    quaternion_lock(&s_quaternion_mutex); 
    quaternion[0] = s_quaternion[0];
    quaternion[1] = s_quaternion[1];
    quaternion[2] = s_quaternion[2];
    quaternion[3] = s_quaternion[3];
    quaternion_unlock(&s_quaternion_mutex);

    return 0;
}

int attidude_set_quaternion(const float *quaternion)
{
    quaternion_lock(&s_quaternion_mutex); 
    s_quaternion[0] = quaternion[0];
    s_quaternion[1] = quaternion[1];
    s_quaternion[2] = quaternion[2];
    s_quaternion[3] = quaternion[3];
    quaternion_unlock(&s_quaternion_mutex);

    return 0;
} 

int attidude_init(void)
{
    float euler[ALGO_DIM] = {0.0f}; /* 欧拉角 全零 */
    float q[ALGO_QUAD] = {0.0f}; /* 四元数 */

    if (0 != mutex_init(&s_quaternion_mutex))
    {
        algo_printf("%s,%d:pthread_mutex_init error!\n", __FILE__,__LINE__);
    }

    attidude_euler2quaternion(q, euler);
    attidude_set_quaternion(q);

#ifdef ALGO_TRACE
    algo_printf("初始化四元数为:\n");
    attidude_print_quaternion();
    algo_printf("\n");
#endif

    return 0;
}

int attidude_print_quaternion(void)
{
    float q[ALGO_QUAD] = {0.0f}; /* 四元数 */

    attidude_get_quaternion(q);

    algo_printf("%7.4f,%7.4f,%7.4f,%7.4f", q[0], q[1], q[2], q[3]);

    return 0;
}

