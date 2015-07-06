/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150629 
 * 版本号  ： 1.0
 * 文件描述： MPU9250传感器 I2C读取
 * 版权说明： Copyright (c) GNU
 * 其    他： 无 
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

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
    struct int_param_s int_param;
    short data[3] = {0}; /* 0x, 1y, 2z*/
    long temperature = 0; /* 温度 */
    unsigned long time_ms = 0;

    unsigned char accel_fsr = 0;
    unsigned short accel_sens = 0;

    unsigned short gyro_rate = 0;
    unsigned short gyro_fsr = 0;
    float gyro_sens = 0;

    unsigned short compass_fsr = 0;

    cmos_init();
    cmos_i2c_init(MPU9250_I2C_INDEX, MPU9250_SPEED);

    mpu_init(&int_param);
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
    mpu_set_sample_rate(MPU9250_DEFAULT_HZ);
    mpu_set_compass_sample_rate(AK8963_DEFAULT_HZ);

    mpu_get_sample_rate(&gyro_rate);

    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_gyro_sens(&gyro_sens);
    mpu_get_accel_fsr(&accel_fsr);
    mpu_get_accel_sens(&accel_sens);
    mpu_get_compass_fsr(&compass_fsr);

    while(TRUE)
    {
        mpu_get_gyro_reg(&data[0], &time_ms);
        cmos_printf("time:%5.2fms: gyro(+-%d):   %5.2f(0x%04x),%5.2f(0x%04x),%5.2f(0x%04x)\r\n",
                time_ms/1000.0, gyro_fsr,
                data[0]/gyro_sens, data[0], data[1]/gyro_sens, data[1], data[2]/gyro_sens, data[2]);

        mpu_get_accel_reg(&data[0], &time_ms);
        cmos_printf("time:%5.2fms: accel(+-%d):     %5.2f(0x%04x),%5.2f(0x%04x),%5.2f(0x%04x)\r\n",
                time_ms/1000.0, accel_fsr,
                data[0]/accel_sens, data[0], data[1]/accel_sens, data[1], data[2]/accel_sens, data[2]);

        mpu_get_compass_reg(&data[0], &time_ms);
        cmos_printf("time:%5.2fms: cmopass(+-%d): %d(0x%04x), %d(0x%04x),%d(0x%04x)\r\n",
                time_ms/1000.0, compass_fsr,
                data[0], data[0], data[1], data[1], data[2], data[2]);

        mpu_get_temperature(&temperature, &time_ms);
        cmos_printf("time:%5.2fms: tempera:        %5.2f(0x%04x)\r\n\r\n",
                time_ms/1000.0, temperature, temperature);

			cmos_delay_ms(1000);
    }
#if 0
    mpu9250_init();

    while(TRUE)
    {
        now = cmos_get_ms();
        cmos_printf("%5.2fs:\r\n",   now / 1000.0);

        mpu9250_read_accel(&s_accel);
        cmos_printf("accel(+-4g):    %5.2f(0x%04x),%5.2f(0x%04x),%5.2f(0x%04x)\r\n",
                s_accel.x/8192.0, s_accel.x, s_accel.y/8192.0, s_accel.y, s_accel.z/8192.0, s_accel.z);

        mpu9250_read_gyro(&s_gyro);
        cmos_printf("gyro(+-2000dps):%5.2f(0x%04x),%5.2f(0x%04x),%5.2f(0x%04x)\r\n",
                s_gyro.x/16.4, s_gyro.x, s_gyro.y/16.4, s_gyro.y, s_gyro.z/16.4, s_gyro.z);

        mpu9250_read_mag(&s_mag);
        cmos_printf("mag(0.15):      %5.2f(0x%04x),%5.2f(0x%04x),%5.2f(0x%04x)\r\n",
                s_mag.x*0.15, s_mag.x, s_mag.y*0.15, s_mag.y, s_mag.z*0.15, s_mag.z);

        mpu9250_read_tem(&temp);
        cmos_printf("temp:           %5.2f(0x%04x)\r\n\r\n", 21 + temp/338.3, temp);

        cmos_delay_ms(1000);
    }
#endif
}

