/******************************************************************************
 *
 * 文件名  ： algo.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150821 
 * 版本号  ： v1.0
 * 文件描述： 姿态融合算法
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "algo.h"
#include "port.h"
#include "algo_math.h"
#include "attidude.h"
#include "gyro.h"
#include "accel.h"
#include "mag.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : algo_set
 * 负责人  : 彭鹏
 * 创建日期: 20150821
 * 函数功能: 融合算法参数设置
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int algo_init(void)
{
    /* 姿态初始化 */
    attidude_init();

#ifdef ALGO_TRACE
    algo_printf("算法库配置为:\n");
    algo_printf("陀螺仪:%s, 周期:%4ums\n",
            ALGO_GYRO_ENABLE ? ("打开") : ("关闭"), ALGO_GYRO_PERIOD);

    algo_printf("加计  :%s, 周期:%4ums\n", 
            ALGO_ACCEL_ENABLE? ("打开") : ("关闭"), ALGO_ACCEL_PERIOD);

    algo_printf("磁力计:%s, 周期:%4ums\n", 
            ALGO_MAG_ENABLE? ("打开") : ("关闭"), ALGO_MAG_PERIOD);
#endif

    return 0;
}

/*******************************************************************************
 *
 * 函数名  : algo_start
 * 负责人  : 彭鹏
 * 创建日期: 20150824
 * 函数功能: 启动融合算法线程
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int algo_start(void)
{
    thread_id_T gyro_tid;
    thread_id_T accel_tid;
    thread_id_T mag_tid;

    gyro_tid = 0;
    accel_tid = 0;
    mag_tid = 0;

    if(ALGO_GYRO_ENABLE && ALGO_GYRO_PERIOD)
    {
        gyro_tid = thread_create(gyro_loop, NULL);

        if(0 == gyro_tid)
        {
            algo_printf("陀螺仪线程启动失败.\n");
        }

#ifdef ALGO_TRACE
        algo_printf("陀螺仪线程id:%p.\n", (void *)gyro_tid);
#endif
    }

    if(ALGO_ACCEL_ENABLE && ALGO_ACCEL_PERIOD)
    {
        accel_tid= thread_create(accel_loop, NULL);

        if(0 == accel_tid)
        {
            algo_printf("加计线程启动失败.\n");
        }

#ifdef ALGO_TRACE
        algo_printf("加计线程id:%p.\n", (void *)accel_tid);
#endif
    }

    if(ALGO_MAG_ENABLE && ALGO_MAG_PERIOD)
    {
        mag_tid= thread_create(mag_loop, NULL);

        if(0 == mag_tid)
        {
            algo_printf("磁计线程启动失败.\n");
        }

#ifdef ALGO_TRACE
        algo_printf("磁计线程id:%p.\n", (void *)mag_tid);
#endif
    }

    return 0;
}

int algo_get_level(float *theta, float *phi)
{
    attidude_get_level(theta, phi);
    *theta = math_arc2angle(*theta);
    *phi = math_arc2angle(*phi);
    return 0;
}

int algo_get_psi(float *psi)
{
    attidude_get_psi(psi);
    *psi = math_arc2angle(*psi);
    return 0;
}

int algo_print_quaternion(void)
{
    return attidude_print_quaternion();
}

