/******************************************************************************
 *
 * 文件名  ： i2c.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150703 
 * 版本号  ： 1.0
 * 文件描述： i2c cmos hal
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "i2c.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f429idiscovery_hardware.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
I2C_HandleTypeDef s_i2c_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_i2c_init
* 负责人  : 彭鹏
* 创建日期: 20150703
* 函数功能: i2c中间层初始化
*
* 输入参数: uart_index 硬件i2c编号
*           baud_rate  i2c速度 < 400k
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 
*           TODO:本模块内部管理多i2c的共享问题
*                实现解初始化
*
******************************************************************************/
cmos_status_T cmos_i2c_init(cmos_uint32_T i2c_index, cmos_int32_T bit_rate)
{
    if(HAL_I2C_GetState(&s_i2c_handle) == HAL_I2C_STATE_RESET)
    {
        /* 由索引赋值i2c基地址 */
        switch(i2c_index)
        {
            case 3:
                {
                    s_i2c_handle.Instance = IMU_I2C;
                    break;
                }
            default:
                {
                    assert_failed(__FILE__, __LINE__);
                    break;
                }

        }
        s_i2c_handle.Init.ClockSpeed       = bit_rate;
        s_i2c_handle.Init.DutyCycle        = I2C_DUTYCYCLE_2;
        s_i2c_handle.Init.OwnAddress1      = 0;
        s_i2c_handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
        s_i2c_handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
        s_i2c_handle.Init.OwnAddress2      = 0;
        s_i2c_handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
        s_i2c_handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE; 

        if(HAL_OK != HAL_I2C_Init(&s_i2c_handle))
        {
            assert_failed(__FILE__, __LINE__);
        }
        return cmos_OK_E;
    }
    else
    {
        return cmos_DEV_BUSY_E;
    }
}

/*******************************************************************************
*
* 函数名  : cmos_i2c_read_buf
* 负责人  : 彭鹏
* 创建日期: 20150704
* 函数功能: i2c 读取多字节
*
* 输入参数: dev_addr 设备地址
*           reg_addr 寄存器启始地址
*           ptr_read_buf 读取的缓存
*           buf_len 缓存大小
*
* 输出参数: 无
*
* 返回值  : 读取的字节数
*           0    无数据读出
*           其他 读取的字节数
*
* 调用关系: 无
* 其 它   : buf_len过大(需要的数据过多会卡死)
*
******************************************************************************/
cmos_uint32_T cmos_i2c_read_buf(cmos_uint8_T dev_addr, cmos_uint16_T reg_addr,
        cmos_uint8_T *ptr_read_buf, cmos_uint32_T buf_len)
{
  if(HAL_OK != HAL_I2C_Mem_Read(&s_i2c_handle, dev_addr, reg_addr,
              I2C_MEMADD_SIZE_8BIT, ptr_read_buf, (cmos_uint16_T)(buf_len), HAL_MAX_DELAY))
  {
      assert_failed(__FILE__, __LINE__);
  }

  return buf_len;
}

/*******************************************************************************
*
* 函数名  : cmos_i2c_write_buf
* 负责人  : 彭鹏
* 创建日期: 20150704
* 函数功能: i2c 读取多字节
*
* 输入参数: dev_addr 设备地址
*           reg_addr 寄存器启始地址
*           ptr_read_buf 写入的缓存
*           buf_len 缓存大小
*
* 输出参数: 无
*
* 返回值  : 写入的字节数
*
* 调用关系: 无
* 其 它   : buf_len过大(写入的数据过多会卡死)
*
******************************************************************************/
cmos_uint32_T cmos_i2c_write_buf(cmos_uint8_T dev_addr, cmos_uint8_T reg_addr,
        const cmos_uint8_T *ptr_write_buf, cmos_uint32_T buf_len)
{
  if(HAL_OK != HAL_I2C_Mem_Write(&s_i2c_handle, dev_addr, reg_addr,
              I2C_MEMADD_SIZE_8BIT, (cmos_uint8_T *)ptr_write_buf, (cmos_uint16_T)(buf_len), HAL_MAX_DELAY))
  {
      assert_failed(__FILE__, __LINE__);
  }

  return buf_len;
}

