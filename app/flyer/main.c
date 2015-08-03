/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150629 
 * 版本号  ： 1.0
 * 文件描述： MPU9250传感器 I2C读取
 * 版权说明： Copyright (c) GNU
 * 其    他： TODO:研究清楚算法
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

/*----------------------------------- 声明区 ----------------------------------*/
#define MAIN_DIM        (3)
#define MAIN_YAW        (0)
#define MAIN_PITCH      (1)
#define MAIN_ROLL       (2)
#define MAIN_PI         (3.1415926)

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static int init(void);
static void run_self_test(void);
static void get_temperature(float *temperature, unsigned long *time_stamp);
static void get_gyro(float *gyro, unsigned long *time_stamp);
static void get_accel(float *accel, unsigned long *time_stamp);
static void get_compass(float *compass, unsigned long *time_stamp);

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
    unsigned long time_stamp = 0;   /* 时间 */
    float temperature = 0;          /* 温度 */
    float accel[MAIN_DIM] = {0};    /* 加速度 x y z*/
    float gyro[MAIN_DIM] = {0};     /* 陀螺仪 x y z*/
    float compass[MAIN_DIM] = {0};  /* 磁力计 x y z*/

    float ypr[MAIN_DIM] = {0};      /* yaw pitch roll */

    init();
    do{
        get_temperature(&temperature, &time_stamp);
        cmos_printf("attitude(%5.2fs,%5.2fC):%5.2f,%5.2f,%5.2f.\r\n",
                time_stamp / 1000.0, temperature, ypr[MAIN_YAW], ypr[MAIN_PITCH], ypr[MAIN_ROLL]);

        get_accel(accel, &time_stamp); 
        cmos_printf("accel(%5.2fs):     %5.2f,%5.2f,%5.2f.\r\n", accel[0], accel[1], accel[2]);

        get_gyro(gyro, &time_stamp); 
        cmos_printf("gyro(%5.2fs):      %5.2f,%5.2f,%5.2f.\r\n", gyro[0], gyro[1], gyro[2]);

        get_compass(compass, &time_stamp);
        cmos_printf("compass(%5.2f):    %5.2f,%5.2f,%5.2f.\r\n", compass[0], compass[1], compass[2]);
        cmos_printf("\r\n");

        mpu9250_delay_ms(1000); /* 1s 打印一次 */
    }while(TRUE);
}

/*******************************************************************************
*
* 函数名  : init
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
static int init(void)
{
    unsigned char dev_status = 0;

    /* 系统初始化 */
    cmos_init();
    cmos_i2c_init(MPU9250_I2C_INDEX, MPU9250_SPEED);

    /* invensence 初始化 */
    cmos_printf("初始化MPU...\r\n");
    if (mpu_init(NULL) != 0)
    {
        cmos_printf("初始化MPU失败!\r\n");
        return -1;
    }
    cmos_printf("打开传感器...\r\n");
    if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS)!=0)
    {
        cmos_printf("打开传感器失败.\r\n");
        return -1;
    }
    cmos_printf("设置陀螺仪量程...\r\n");
    if (mpu_set_gyro_fsr(2000)!=0)
    {
        cmos_printf("设置陀螺仪量程失败.\r\n");
        return -1;
    }
    cmos_printf("设置加速度计量程...\r\n");
    if (mpu_set_accel_fsr(2)!=0)
    {
        cmos_printf("设置加速度计量程失败.\r\n");
        return -1;
    }
    cmos_printf("MPU上电...\r\n");
    mpu_get_power_state(&dev_status);
    cmos_printf("MPU9250 上电%s", dev_status? "成功!\r\n" : "失败!\r\n");
    cmos_printf("设置MPU FIFO...\r\n");
    if (mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL)!=0)
    {
        cmos_printf("设置MPU FIFO失败.\r\n");
        return -1;
    }

    cmos_printf("复位FIFO队列...\r\n");
    if (mpu_reset_fifo()!=0)
    {
        cmos_printf("复位FIFO队列失败.\r\n");
        return -1;
    }

    cmos_printf("自检...\r\n"); 
    run_self_test();
    cmos_printf("初始化完成.\r\n");

    return 0;
}

/*******************************************************************************
*
* 函数名  : update
* 负责人  : 彭鹏
* 创建日期: 20150707
* 函数功能: 更新数据
*
* 输入参数: 无
*
* 输出参数: (unsigned long *time, float *temper, float *ypr, float *accel, float *gyro, float *compass)
*
* 返回值  : 0       成功
*           其他    失败
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void get_temperature(float *temperature, unsigned long *time_stamp)
{
    long temperature_l = 0;

    mpu_get_temperature(&temperature_l, time_stamp);
    *temperature = (float)temperature_l/65536L; 
}

static void get_gyro(float *gyro, unsigned long *time_stamp)
{ 
    short gyro_i[3] = {0};
    float sens = 0;

    mpu_get_gyro_reg(gyro_i, time_stamp);
    mpu_get_gyro_sens(&sens);
    for(int i=0;i<MAIN_DIM;i++)
    {
        gyro[i] = gyro_i[i] / sens;
    }
}

static void get_accel(float *accel, unsigned long *time_stamp)
{
    short accel_i[3] = {0};
    unsigned short sens = 0;

    mpu_get_accel_reg(accel_i, time_stamp);
    mpu_get_accel_sens(&sens);
    for(int i=0;i<MAIN_DIM;i++)
    {
        accel[i] = 1.0 * accel_i[i] / sens;
    }
}

static void get_compass(float *compass, unsigned long *time_stamp)
{ 
    short compass_i[3] = {0};

    mpu_get_compass_reg(compass_i, time_stamp); 

#if 0
    unsigned short fsr = 0;
    mpu_get_compass_fsr(&fsr);
    for(int i=0;i<MAIN_DIM;i++)
    {
        compass[i] = 1.0 * compass_i[i] / fsr;
    }
#else
    for(int i=0;i<MAIN_DIM;i++)
    {
        compass[i] = 1.0 * compass_i[i];
    }
#endif
}

/* TODO:补充注释 */
static void run_self_test(void)
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

        cmos_printf("测试通过,设置偏移.\r\n");
        cmos_printf("accel offset: %7.4f %7.4f %7.4f\r\n",
                accel[0]/65536.f,
                accel[1]/65536.f,
                accel[2]/65536.f);
        cmos_printf("gyro  offset: %7.4f %7.4f %7.4f\r\n",
                gyro[0]/65536.f,
                gyro[1]/65536.f,
                gyro[2]/65536.f);
    }
    else
    {
        if (!(result & 0x1))
        {
            cmos_printf("Gyro failed.\r\n");
        }
        if (!(result & 0x2))
        {
            cmos_printf("Accel failed.\r\n");
        }
        if (!(result & 0x4))
        {
            cmos_printf("Compass failed.\r\n");
        }
    }

    return;
}

