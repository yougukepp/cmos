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
#include "attidude.h"
#include "gyro.h"

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
    unsigned int gyro_period = 0;

    if(ALGO_GYRO_ENABLE && ALGO_GYRO_PERIOD)
    {
        gyro_period = ALGO_GYRO_PERIOD;
        gyro_tid = thread_create(fusion_gyro_loop, &gyro_period);

#ifdef ALGO_TRACE
        algo_printf("陀螺仪线程id:0x%08x.\n", (int)gyro_tid);
#endif
    }

    return 0;
}

int algo_get_level(float *pitch, float *roll)
{
    return attidude_get_level(pitch, roll);
}

int algo_get_yaw(float *yaw)
{
    return attidude_get_yaw(yaw);
}

int algo_print_quaternion(void)
{
    return attidude_print_quaternion();
}

