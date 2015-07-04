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
static unsigned char mpu9250_read_byte(unsigned char dev_addr, unsigned char reg_addr);
static void mpu9250_write_byte(unsigned char dev_addr, unsigned char reg_addr, unsigned char write_byte);
static int mpu9250_read(unsigned char dev_addr, unsigned char reg_addr,
        unsigned short buf_len, unsigned char *ptr_read_buf);
static int mpu9250_write(unsigned char dev_addr, unsigned char reg_addr, 
        unsigned short buf_len, const unsigned char *ptr_write_buf);
				
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
    int i = 0;
    int i_max = 0;
    unsigned char val = 0;

    unsigned char mpu9250_init_data[][2] = {
        {0x80, MPU9250_PWR_MGMT_1_REG},     // Reset Device
        {0x00, MPU9250_PWR_MGMT_1_REG},     // Clock Source
        {0x00, MPU9250_PWR_MGMT_2_REG},     // Enable Acc & Gyro
				{0x07, MPU9250_SMPLRT_DIV_REG},     //
        {0x06, MPU9250_CONFIG_REG},         // 
        {0x18, MPU9250_GYRO_CONFIG_REG},    // +-2000dps
        {0x08, MPU9250_ACCEL_CONFIG_REG},   // +-4G
       
        /* 将磁力计I2C挂接与STM32F4 的I2C总线 */
        {0x02, MPU9250_INT_PIN_CFG_REG},
    };

    /* I2C 初始化 */
    cmos_i2c_init(MPU9250_I2C_INDEX, MPU9250_SPEED); 

    /* 保证I2C正常工作 */
    do{
        val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_WHO_AM_I_REG);
        cmos_printf("mpu9250 id:0x%02x", val);
        cmos_delay_ms(100);
    }while(MPU9250_DEV_ID != val);

    /* MPU9250 初始化 */
    i_max = sizeof(mpu9250_init_data) / 2;
    for(i=0; i<i_max; i++)
    {
        mpu9250_write_byte(MPU9250_DEV_ADDR, mpu9250_init_data[i][1], mpu9250_init_data[i][0]);
        cmos_delay_ms(1);
    }

    /* 确认磁力计正常工作 */
    do{
        val = mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_WIA_REG);
        cmos_delay_ms(100);
    }while(AK8963_DEV_ID != val);

}

/*******************************************************************************
*
* 函数名  : mpu9250_read
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
* 返回值  : 0       无数据读出
*           其他    读出数据个数
*
* 调用关系: 无
* 其 它   : buf_len过大(需要的数据过多会卡死)
*
******************************************************************************/
static int mpu9250_read(unsigned char dev_addr,
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
    return read_num;
}

/*******************************************************************************
*
* 函数名  : mpu9250_write
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
* 返回值  : 0       无数据写入
*           其他    写入数据个数
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static int mpu9250_write(unsigned char dev_addr,
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
    return write_num;
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
static unsigned char mpu9250_read_byte(unsigned char dev_addr, unsigned char reg_addr)
{
    int rst = 0; 
    unsigned char val = 0xff;
    
    rst = mpu9250_read(dev_addr, reg_addr, 1, &val);
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
static void mpu9250_write_byte(unsigned char dev_addr, unsigned char reg_addr, unsigned char write_byte)
{
    int rst = 0;

    rst = mpu9250_write(dev_addr, reg_addr, 1, &write_byte);
    if(1 != rst)
    {
        assert_failed(__FILE__, __LINE__);
    }
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

/* TODO:优化性能 mpu9250_read读 */
void mpu9250_read_accel(accel_T *ptr_accel)
{
    unsigned short val = 0xffff;

    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_ACCEL_XOUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_ACCEL_XOUT_L_REG); 
    ptr_accel->x = val;

    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_ACCEL_YOUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_ACCEL_YOUT_L_REG); 
    ptr_accel->y = val;

    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_ACCEL_ZOUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_ACCEL_ZOUT_L_REG); 
    ptr_accel->z = val;
}

void mpu9250_read_gyro(gyro_T *ptr_gyro)
{
    unsigned short val = 0xffff;

    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_GYRO_XOUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_GYRO_XOUT_L_REG); 
    ptr_gyro->x = val;

    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_GYRO_YOUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_GYRO_YOUT_L_REG); 
    ptr_gyro->y = val;

    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_GYRO_ZOUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_GYRO_ZOUT_L_REG); 
    ptr_gyro->z = val;
}

void mpu9250_read_mag(mag_T *ptr_mag)
{
    unsigned short val = 0xffff;

    /* TODO:优化性能 */
    mpu9250_write_byte(AK8963_I2C_ADDR, AK8963_CNTL1_REG, 0x02);
    cmos_delay_ms(10);

    val = mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_HXH_REG);
    val <<= 8;
    val |= mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_HXL_REG); 
    ptr_mag->x = val;

    val = mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_HYH_REG);
    val <<= 8;
    val |= mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_HYL_REG); 
    ptr_mag->y = val;

    val = mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_HZH_REG);
    val <<= 8;
    val |= mpu9250_read_byte(AK8963_I2C_ADDR, AK8963_HZL_REG); 
    ptr_mag->z = val;
}

void mpu9250_read_tem(unsigned short *ptr_temp)
{
    unsigned short val = 0xffff;
    val = mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_TEMP_OUT_H_REG);
    val <<= 8;
    val |= mpu9250_read_byte(MPU9250_DEV_ADDR, MPU9250_TEMP_OUT_L_REG); 
    *ptr_temp = val;
}

