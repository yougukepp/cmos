/********************************************************************************
*
* 文件名  ： i2c.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20151101
* 版本号  ： v1.0
* 文件描述： cmos i2c 驱动头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_I2C_I2C_H_
#define _CMOS_HAL_I2C_I2C_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "driver.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef struct cmos_hal_i2c_init_para_T_tag{
    cmos_int32_T bus_num;
    cmos_int32_T device_speed;
}cmos_hal_i2c_init_para_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/
extern const cmos_hal_driver_T g_i2c_driver; /* 驱动变量由vfs指示 */

/*********************************** 接口函数 **********************************/
void cmos_hal_i2c_init(void *para);

#endif /* _CMOS_HAL_I2C_I2C_H_ */

