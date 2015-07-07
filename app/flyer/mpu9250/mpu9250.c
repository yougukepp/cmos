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
#include "mpu9250.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static unsigned char addr_convert(unsigned char addr);

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
        unsigned char *ptr_read_buf)
{
    unsigned int read_num = 0;

    dev_addr = addr_convert(dev_addr);
	
    read_num = cmos_i2c_read_buf(dev_addr, reg_addr, ptr_read_buf, buf_len);
    if(read_num != buf_len)
    {
        assert_failed(__FILE__, __LINE__);
    }
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
        const unsigned char *ptr_write_buf)
{
    unsigned int write_num = 0;

    dev_addr = addr_convert(dev_addr);

    write_num = cmos_i2c_write_buf(dev_addr, reg_addr, ptr_write_buf, buf_len);
    if(write_num != buf_len)
    {
        assert_failed(__FILE__, __LINE__);
    }
    return 0;
}

/*******************************************************************************
*
* 函数名  : mpu9250_read_byte
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: i2c读出字节
*
* 输入参数: dev_addr 设备地址
*           reg_addr 寄存器启始地址
*
* 输出参数: 无
* 返回值  : 待读出的数据
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline unsigned char mpu9250_read_byte(unsigned char dev_addr, unsigned char reg_addr)
{
    int rst = 0; 
    unsigned char val = 0xff;
    
    rst = mpu9250_read_buf(dev_addr, reg_addr, 1, &val);
    if(1 != rst)
    {
        assert_failed(__FILE__, __LINE__);
    }

    return val;
}

/*******************************************************************************
*
* 函数名  : mpu9250_write_byte
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: i2c写入字节
*
* 输入参数: dev_addr 设备地址
*           reg_addr 寄存器启始地址
*           write_byte 待写入的数据
*
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
inline void mpu9250_write_byte(unsigned char dev_addr, unsigned char reg_addr, unsigned char write_byte)
{
    int rst = 0;

    rst = mpu9250_write_buf(dev_addr, reg_addr, 1, &write_byte);
    if(1 != rst)
    {
        assert_failed(__FILE__, __LINE__);
    }
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
    unsigned long val = 0;
    val = cmos_get_ms();
    *count = val;
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
    cmos_delay_ms(ms);
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

