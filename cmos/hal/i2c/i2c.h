/********************************************************************************
*
* 文件名  ： i2c.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150703
* 版本号  ： v1.0
* 文件描述： cmos hal i2c模块头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_HAL_I2C_H_
#define _CMOS_HAL_I2C_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
cmos_status_T cmos_i2c_init(cmos_uint32_T i2c_index, cmos_int32_T bit_rate);
cmos_status_T cmos_i2c_read_byte(cmos_uint8_T dev_addr, cmos_uint16_T reg_addr, cmos_uint8_T *ptr_read_byte);
cmos_status_T cmos_i2c_write_byte(cmos_uint8_T dev_addr, cmos_uint8_T reg_addr, cmos_uint8_T write_byte);

cmos_uint32_T cmos_i2c_read_buf(cmos_uint8_T dev_addr, cmos_uint16_T reg_addr,
        cmos_uint8_T *ptr_read_buf, cmos_uint32_T buf_len);
cmos_uint32_T cmos_i2c_write_buf(cmos_uint8_T dev_addr, cmos_uint8_T reg_addr,
        const cmos_uint8_T *ptr_write_buf, cmos_uint32_T buf_len);

#endif /* _CMOS_HAL_I2C_H_ */

