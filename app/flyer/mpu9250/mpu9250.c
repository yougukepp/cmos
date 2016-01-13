/******************************************************************************
 *
 * 文件名  ： mpu9250.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150703 
 * 版本号  ： 1.0
 * 文件描述： mpu9250驱动
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
/* 消除中文打印警告 */
#pragma  diag_suppress 870

/************************************ 头文件 ***********************************/
#include "inv_mpu.h"
#include "mpu9250.h"
#include "imu.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static unsigned char addr_convert(unsigned char addr);
static void run_self_test(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : mpu9250_read_buf
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: i2c读取
*
* 输入参数: dev_addr 设备地址
*           reg_addr 寄存器启始地址
*           buf_len 缓存大小
*
* 输出参数: ptr_read_buf 读取的缓存
*
* 返回值  : 0       成功
*           其他    失败
*
* 调用关系: 无
* 其 它   : buf_len过大(需要的数据过多会卡死)
*
******************************************************************************/
inline int mpu9250_read_buf(unsigned char dev_addr,
        unsigned char reg_addr,
        unsigned short buf_len, 
        unsigned char *buf)
{
    uint8_T dev_addr_real = addr_convert(dev_addr);
   
    imu_read(dev_addr_real, reg_addr, buf, buf_len);

    return 0;
}

/*******************************************************************************
*
* 函数名  : mpu9250_write_buf
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: i2c写入
*
* 输入参数: dev_addr 设备地址
*           reg_addr 寄存器启始地址
*           buf_len 缓存大小
*
* 输出参数: ptr_write_buf 写入的缓存
*
* 返回值  : 0       成功
*           其他    失败
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline int mpu9250_write_buf(unsigned char dev_addr,
        unsigned char reg_addr,
        unsigned short buf_len, 
        const unsigned char *buf)
{
    uint8_T dev_addr_real = addr_convert(dev_addr);

    imu_write(dev_addr_real, reg_addr, buf, buf_len);

    return 0;
}

/*******************************************************************************
*
* 函数名  : mpu9250_get_ms
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: 获取系统启动后的时间ms为单位
*
* 输入参数: 无
* 输出参数: count 当前ms值地址
* 返回值  : 0 正确
*           1 出错
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline int mpu9250_get_ms(unsigned long *count)
{
    *count = HAL_GetTick();
    return 0;
}

/*******************************************************************************
*
* 函数名  : mpu9250_delay_ms
* 负责人  : 彭鹏
* 创建日期: 20150707
* 函数功能: 延迟 ms作为单位
*
* 输入参数: 无
* 输出参数: count 当前ms值地址
* 返回值  : 0 正确
*           1 出错
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void mpu9250_delay_ms(unsigned int ms)
{
    HAL_Delay(ms);
}

/*******************************************************************************
*
* 函数名  : addr_convert
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: 地址转换
*           MPU9250 7bit地址 1101000
*           stm32f4 i2c接收的地址 1101000X (X 1读 x0 写)
*           故地址需要左移1bit
*
* 输入参数: addr 待转换的地址
* 输出参数: 无
* 返回值  : 转换后的地址
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline static unsigned char addr_convert(unsigned char addr)
{
    return (addr << 1);
}

void mpu9250_init(void)
{
    unsigned char dev_status = 0;

    /* invensence 初始化 */
    debug_log("初始化MPU...\r\n");
    if (mpu_init(NULL) != 0)
    {
        debug_log("初始化MPU失败!\r\n");
        return;
    }
    debug_log("打开传感器...\r\n");
    if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS)!=0)
    {
        debug_log("打开传感器失败.\r\n");
        return;
    }
    debug_log("设置陀螺仪量程...\r\n");
    if (mpu_set_gyro_fsr(2000)!=0)
    {
        debug_log("设置陀螺仪量程失败.\r\n");
        return;
    }
    debug_log("设置加速度计量程...\r\n");
    if (mpu_set_accel_fsr(2)!=0)
    {
        debug_log("设置加速度计量程失败.\r\n");
        return;
    }
    debug_log("MPU上电...\r\n");
    mpu_get_power_state(&dev_status);
    debug_log("MPU9250 上电%s", dev_status? "成功!\r\n" : "失败!\r\n");
    debug_log("设置MPU FIFO...\r\n");
    if (mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL)!=0)
    {
        debug_log("设置MPU FIFO失败.\r\n");
        return;
    }

    debug_log("复位FIFO队列...\r\n");
    if (mpu_reset_fifo()!=0)
    {
        debug_log("复位FIFO队列失败.\r\n");
        return;
    }

    debug_log("自检...\r\n"); 
    run_self_test();

    return;
}

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

        debug_log("测试通过,设置偏移.\r\n");
        debug_log("accel offset: %7.4f %7.4f %7.4f\r\n",
                accel[0]/65536.f,
                accel[1]/65536.f,
                accel[2]/65536.f);
        debug_log("gyro  offset: %7.4f %7.4f %7.4f\r\n",
                gyro[0]/65536.f,
                gyro[1]/65536.f,
                gyro[2]/65536.f);
    }
    else
    {
        if (!(result & 0x1))
        {
            debug_log("Gyro failed.\r\n");
        }
        if (!(result & 0x2))
        {
            debug_log("Accel failed.\r\n");
        }
        if (!(result & 0x4))
        {
            debug_log("Compass failed.\r\n");
        }
    }

    return;
}

