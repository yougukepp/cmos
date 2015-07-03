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

/************************************ 头文件 ***********************************/
#include "inv_mpu.h"
#include "mltypes.h"
#include "mpu9250.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : mpu9250_init
* 负责人  : 彭鹏
* 创建日期: 20150603
* 函数功能: mpu9250初始化
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void mpu9250_init(void)
{
    inv_error_t result;
    unsigned char accel_fsr,  new_temp = 0;
    unsigned short gyro_rate, gyro_fsr;
    unsigned long timestamp;
    struct int_param_s int_param;

#ifdef COMPASS_ENABLED
    unsigned char new_compass = 0;
    unsigned short compass_fsr;
#endif

	
	
    /* I2C 初始化 */
    cmos_i2c_init(MPU9250_I2C_INDEX, MPU9250_SPEED);
    
    result = mpu_init(&int_param);
    if(result) 
		{
        console_printf("Could not initialize gyro.\n");
    }
	
}

/*******************************************************************************
*
* 函数名  : mpu9250_read
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: mpu9250读取字节
*
* 输入参数: reg_addr mpu9250寄存器地址
* 输出参数: 无
* 返回值  : 读出的值
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
unsigned char mpu9250_read(unsigned char reg_addr)
{ 
    unsigned char val = 0xff;
    cmos_i2c_read_byte(MPU9250_DEV_ADDR, reg_addr, &val);
    return val;
}

/*******************************************************************************
*
* 函数名  : mpu9250_write
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: mpu9250写入字节
*
* 输入参数: reg_addr mpu9250寄存器地址
*           data 待写入的值
* 输出参数: 无
* 返回值  : 读出的值
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void mpu9250_write(unsigned char reg_addr, unsigned char data)
{ 
    cmos_i2c_write_byte(MPU9250_DEV_ADDR, reg_addr, data);
}

/* TODO: 完善注释 */
int Sensors_I2C_WriteRegister(unsigned char slave_addr,
        unsigned char reg_addr,
        unsigned short len, 
        const unsigned char *ptr_data)
{
	  slave_addr <<= 1;
    if((1 != len)
     ||(slave_addr != MPU9250_DEV_ADDR))
    {
      assert_failed(__FILE__, __LINE__);
    }

    mpu9250_write(reg_addr, *ptr_data);

    return 0;
}

int Sensors_I2C_ReadRegister(unsigned char slave_addr,
        unsigned char reg_addr,
        unsigned short len, 
        unsigned char *ptr_data)
{
		slave_addr <<= 1;
    if((1 != len)
     ||(slave_addr != MPU9250_DEV_ADDR))
    {
      assert_failed(__FILE__, __LINE__);
    }

    *ptr_data = mpu9250_read(reg_addr);

    return 0;
}

