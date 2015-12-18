/******************************************************************************
 *
 * 文件名  ： flyer.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151218 
 * 版本号  ： 1.1
 * 文件描述： 飞控办主控模块
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
/* 消除中文打印警告 */
#pragma  diag_suppress 870

/************************************ 头文件 ***********************************/
#include <math.h>
#include "cmos_config.h"
#include "cmos_api.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "algorithm.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static int mpu9250_init(void);
static void mpu9250_self_test(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: MPU9250 测试主函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 主程序永不返回
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
int main(void)
{ 
    cmos_init();
    cmos_debug_log("cmos初始化完成.\r\n");

    init();

    /* 起姿态检测算法 */
    cmos_debug_log("imu算法库初始化...\r\n"); 

    /* 起pid控制算法 */
    cmos_debug_log("pid算法库初始化...\r\n"); 

    /* 调度启动 */
    cmos_start();
}

/*******************************************************************************
*
* 函数名  : mpu9250_init
* 负责人  : 彭鹏
* 创建日期: 20150707
* 函数功能: 主初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 0       成功
*           其他    失败
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static int mpu9250_init(void)
{
    unsigned char dev_status = 0;

    /* invensence 初始化 */
    cmos_debug_log("初始化MPU...\r\n");
    if (mpu_init(NULL) != 0)
    {
        cmos_debug_log("初始化MPU失败!\r\n");
        return -1;
    }
    cmos_debug_log("打开传感器...\r\n");
    if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS)!=0)
    {
        cmos_debug_log("打开传感器失败.\r\n");
        return -1;
    }
    cmos_debug_log("设置陀螺仪量程...\r\n");
    if (mpu_set_gyro_fsr(2000)!=0)
    {
        cmos_debug_log("设置陀螺仪量程失败.\r\n");
        return -1;
    }
    cmos_debug_log("设置加速度计量程...\r\n");
    if (mpu_set_accel_fsr(2)!=0)
    {
        cmos_debug_log("设置加速度计量程失败.\r\n");
        return -1;
    }
    cmos_debug_log("MPU上电...\r\n");
    mpu_get_power_state(&dev_status);
    cmos_debug_log("MPU9250 上电%s", dev_status? "成功!\r\n" : "失败!\r\n");
    cmos_debug_log("设置MPU FIFO...\r\n");
    if (mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL)!=0)
    {
        cmos_debug_log("设置MPU FIFO失败.\r\n");
        return -1;
    }

    cmos_debug_log("复位FIFO队列...\r\n");
    if (mpu_reset_fifo()!=0)
    {
        cmos_debug_log("复位FIFO队列失败.\r\n");
        return -1;
    }

    cmos_debug_log("自检...\r\n"); 
    run_self_test();

    return 0;
}

/*******************************************************************************
*
* 函数名  : mpu9250_self_test
* 负责人  : 彭鹏
* 创建日期: 20150707
* 函数功能: 自检并设置校正值
*
* 输入参数: 无
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void mpu9250_self_test(void)
{
    int result = 0;
    long gyro[3] = {0};
    long accel[3] = {0};

    result = mpu_run_6500_self_test(gyro, accel, 0);
    if (result == 0x7)
    {
        /* 自检测试通过 我们需要更新校准数据 与offset寄存器 */
        unsigned char i = 0;
        for(i = 0; i<3; i++)
        {
            gyro[i] = (long)(gyro[i] * 32.8f); /* 运用量程 +-1000dps */
            accel[i] *= 2048.f; /* 运用量程convert to +-16G */
            accel[i] = accel[i] >> 16;
            gyro[i] = (long)(gyro[i] >> 16);
        }

        mpu_set_gyro_bias_reg(gyro);
        mpu_set_accel_bias_6500_reg(accel);

        cmos_debug_log("测试通过,设置偏移.\r\n");
        cmos_debug_log("accel offset: %7.4f %7.4f %7.4f\r\n",
                accel[0]/65536.f,
                accel[1]/65536.f,
                accel[2]/65536.f);
        cmos_debug_log("gyro  offset: %7.4f %7.4f %7.4f\r\n",
                gyro[0]/65536.f,
                gyro[1]/65536.f,
                gyro[2]/65536.f);
    }
    else
    {
        if (!(result & 0x1))
        {
            cmos_debug_log("Gyro failed.\r\n");
        }
        if (!(result & 0x2))
        {
            cmos_debug_log("Accel failed.\r\n");
        }
        if (!(result & 0x4))
        {
            cmos_debug_log("Compass failed.\r\n");
        }
    }

    return;
}

