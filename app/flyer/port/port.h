/********************************************************************************
*
* 文件名  ： PORT.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150704
* 版本号  ： v1.0
* 文件描述： MPU9250官方库与cmos适配层头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PORT_H_
#define _PORT_H_

/************************************ 头文件 ***********************************/
#include "cmos_api.h"

/************************************ 宏定义 ***********************************/
#define MPU9250_SPEED               (100000)/* 100k */
#define MPU9250_I2C_INDEX           (3)     /* stm32f429 dis 板i2c3 */

/* mpu官方库 */
/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (20)
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define COMPASS_ON      (0x04)
#define PEDO_READ_MS    (1000)
#define TEMP_READ_MS    (500)
#define COMPASS_READ_MS (100)
/* Data read from MPL. */
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define PRINT_COMPASS   (0x08)
#define PRINT_EULER     (0x10)
#define PRINT_ROT_MAT   (0x20)
#define PRINT_HEADING   (0x40)
#define PRINT_PEDO      (0x80)
#define PRINT_LINEAR_ACCEL (0x100)
#define PRINT_GRAVITY_VECTOR (0x200)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void port_init(void);
int port_get_ms(unsigned long *count);
int port_read(unsigned char dev_addr,
        unsigned char reg_addr,
        unsigned short buf_len, 
        unsigned char *ptr_read_buf);
int port_write(unsigned char dev_addr,
        unsigned char reg_addr,
        unsigned short buf_len, 
        const unsigned char *ptr_write_buf);
int port_uart_send_char(char c);

#endif /* _PORT_H_ */

