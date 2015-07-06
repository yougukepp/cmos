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
/*********************************** 板级定义 **********************************/
#define MPU9250_SPEED                       (100000)/* 100k */
#define MPU9250_I2C_INDEX                   (3)     /* stm32f429 dis 板i2c3 */
#define MPU9250_DEFAULT_HZ                  (20)    /* MPU9250采样率 */
#define AK8963_DEFAULT_HZ                   (10)    /* AK8963 */
/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
#define mpu9250_log_i   cmos_printf
#define mpu9250_log_e   cmos_err_log
int mpu9250_read_buf(unsigned char dev_addr, unsigned char reg_addr,
        unsigned short buf_len, unsigned char *ptr_read_buf);
int mpu9250_write_buf(unsigned char dev_addr, unsigned char reg_addr, 
        unsigned short buf_len, const unsigned char *ptr_write_buf);
int mpu9250_get_ms(unsigned long *count);
void mpu9250_delay_ms(unsigned int ms);



/* FIXME:仅仅方便 可以不使用 */
unsigned char mpu9250_read_byte(unsigned char dev_addr, unsigned char reg_addr);
void mpu9250_write_byte(unsigned char dev_addr, unsigned char reg_addr, unsigned char write_byte);

#endif /* _MPU9250_H_ */

