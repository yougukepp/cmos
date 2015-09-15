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
#include "algorithm.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static int init(void);
static int deinit(void);
static void run_self_test(void);
static void get_temperature(float *temperature, unsigned long *time_stamp);
static void get_gyro(float *gyro, unsigned long *time_stamp);
static void get_accel(float *accel, unsigned long *time_stamp);
static void get_compass(float *compass, unsigned long *time_stamp);

void temp_imu(float *pry, const float *gyro);

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

float angle = 0;
int main(void)
{ 

#if 0
    unsigned char val = 0;
    int i = 0;
    int iMax = 0;
    int read_num = 0;
    unsigned char bmp180_reg_addr[] = {
        0xAA, 0xAB,
        0xAC, 0xAD,
        0xAE, 0xAF,
        0xBA, 0xBB,
        0xBC, 0xBD,
        0xBE, 0xBF,
        0xD0, 0xE0, 0xF4, 0xF6, 0xF7, 0xF8};

    /* 初始化硬件 */
    cmos_init();
    cmos_i2c_init(MPU9250_I2C_INDEX, MPU9250_SPEED);

    cmos_printf("BMP180 寄存器值:\r\n");
    iMax = sizeof(bmp180_reg_addr) /sizeof(bmp180_reg_addr[0]);
    for(i=0;i<iMax;i++) 
    { 
        read_num = cmos_i2c_read_buf(0xEF, bmp180_reg_addr[i], &val, 1);
        if(read_num != 1)
        {
            assert_failed(__FILE__, __LINE__);
        }
        cmos_printf("0x%02x:0x%02x\r\n", bmp180_reg_addr[i], val);
    }
    while(TRUE);
#else
    unsigned long time_stamp = 0;   /* 时间 */
    float temperature = 0;          /* 温度 */
    float accel[MAIN_DIM] = {0};    /* 加速度 x y z*/
    float gyro[MAIN_DIM] = {0};     /* 陀螺仪 x y z*/
    float compass[MAIN_DIM] = {0};  /* 磁力计 x y z*/

    float pry[MAIN_DIM] = {0};      /* yaw pitch roll */

    angle = asin(0.5f) * ALGO_ARC2ANGLE_RATE;          // 30
		angle = atan2(1.0, 1.0) * ALGO_ARC2ANGLE_RATE;     // 45
    angle = atan2(1.0, -1.0) * ALGO_ARC2ANGLE_RATE;    // 135
    angle = atan2(-1.0, -1.0) * ALGO_ARC2ANGLE_RATE;   // -135
		angle = atan2(-1.0, 1.0) * ALGO_ARC2ANGLE_RATE;    // -45
		
    init();

    int times = 0;
    int times_print = 250;

		
    do{
        get_temperature(&temperature, &time_stamp); 
        get_gyro(gyro, &time_stamp); 
        temp_imu(pry, gyro);
        get_accel(accel, &time_stamp); 
        get_compass(compass, &time_stamp);

        if(times > times_print)
        {
            cmos_printf("%d,%lu:\r\n", times, time_stamp);
            cmos_printf("temperature:       %5.2fC.\r\n", temperature);
            cmos_printf("pry:               %5.2f,%5.2f,%5.2f.\r\n", pry[0], pry[1], pry[2]);
            cmos_printf("gyro:              %5.2f,%5.2f,%5.2f.\r\n", gyro[0], gyro[1], gyro[2]);
            cmos_printf("accel:             %5.2f,%5.2f,%5.2f.\r\n", accel[0], accel[1], accel[2]);
            cmos_printf("compass:           %5.2f,%5.2f,%5.2f.\r\n", compass[0], compass[1], compass[2]);
            cmos_printf("\r\n");
            times = 0;
        }

        mpu9250_delay_ms(1); /* 1ms 打印一次 */

        times++;
    }while(TRUE);

    deinit();
#endif
}

/* 临时使用 */
void temp_imu(float *pry, const float *gyro)
{
    imu_update(gyro); 
    imu_get_attitude(pry);

    for(int i=0;i<3;i++)
    {
        pry[i] = math_arc2angle(pry[i]);
    }
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

    cmos_printf("imu算法库初始化...\r\n"); 
    imu_init();

    cmos_printf("初始化完成.\r\n");

    return 0;
}

int deinit(void)
{
    imu_deinit();
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
* 输出参数: (unsigned long *time, float *temper, float *pry, float *accel, float *gyro, float *compass)
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

