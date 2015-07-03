/********************************************************************************
*
* 文件名  ： mpu9250.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： mpu9250驱动头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _MPU9250_H_
#define _MPU9250_H_

/************************************ 头文件 ***********************************/
#include "cmos_api.h"

/************************************ 宏定义 ***********************************/
#define MPU9250_DEV_ADDR            (0xD0)  /* b1101000x */
#define MPU9250_Device_ID           (0x71)  /* MPU9250 id */

#define MPU9250_SPEED               (100000)/* 100k */
#define MPU9250_I2C_INDEX           (3)     /* stm32f429 dis 板i2c3 */

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void mpu9250_init(void);
unsigned char mpu9250_read(unsigned char reg_addr);
void mpu9250_write(unsigned char reg_addr, unsigned char data);


int Sensors_I2C_WriteRegister(unsigned char slave_addr,
        unsigned char reg_addr,
        unsigned short len, 
        const unsigned char *data_ptr);
int Sensors_I2C_ReadRegister(unsigned char slave_addr,
        unsigned char reg_addr,
        unsigned short len, 
        unsigned char *data_ptr);
#endif /* _MPU9250_H_ */

