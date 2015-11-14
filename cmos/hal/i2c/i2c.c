/******************************************************************************
 *
 * 文件名  ： i2c.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151101 
 * 版本号  ： 1.0
 * 文件描述： cmos i2c 驱动程序实现
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
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static void *i2c_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
static cmos_int32_T i2c_read(const void *dev_id, void *buf, cmos_int32_T n_bytes);
static cmos_int32_T i2c_write(const void *dev_id, const void *buf, cmos_int32_T n_bytes);
static cmos_status_T i2c_ioctl(const void *dev_id, cmos_uint32_T request, cmos_uint32_T mode);
static cmos_status_T i2c_close(const void *dev_id);

/* 驱动变量 加入到vfs */
const cmos_hal_driver_T g_i2c_driver = {
    .open = i2c_open,
    .read = i2c_read,
    .write = i2c_write,
    .ioctl = i2c_ioctl,
    .close = i2c_close
};

/* STM32F4Cube HAL驱动 */
I2C_HandleTypeDef s_i2c_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_hal_i2c_init
* 负责人  : 彭鹏
* 创建日期: 20151113
* 函数功能: cmos hal i2c初始化
*
* 输入参数: I2C总线号
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: hal_init中和其他硬件的初始化一起调
* 其 它   : 无
*
******************************************************************************/
void cmos_hal_i2c_init(void *para)
{
    const cmos_hal_i2c_init_para_T *init_para = para;

    if(HAL_I2C_STATE_RESET != HAL_I2C_GetState(&s_i2c_handle))
    {
        CMOS_ERR_STR("cmos_hal_i2c_init running with wrong state!")
        return;
    }

    /* 仅实现一个I2C总线 */
    switch(init_para->bus_num)
    {
        case 3:
            {
                s_i2c_handle.Instance = I2C3;
                break;
            }
        default:
            {
                cmos_err_log("mcu not support %d i2c bus.", init_para->bus_num);
                return;
            }
    } 
    
    s_i2c_handle.Init.ClockSpeed       = init_para->device_speed;
    s_i2c_handle.Init.DutyCycle        = I2C_DUTYCYCLE_2;
    s_i2c_handle.Init.OwnAddress1      = 0;
    s_i2c_handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    s_i2c_handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    s_i2c_handle.Init.OwnAddress2      = 0;
    s_i2c_handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    s_i2c_handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE; 
    
    if(HAL_OK != HAL_I2C_Init(&s_i2c_handle))
    {
        CMOS_ERR_STR("HAL_I2C_Init err");
    }

    return;
}

static void *i2c_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
    return NULL;
}

static cmos_int32_T i2c_read(const void *dev_id, void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

static cmos_int32_T i2c_write(const void *dev_id, const void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

static cmos_status_T i2c_ioctl(const void *dev_id, cmos_uint32_T request, cmos_uint32_T mode)
{
    return cmos_ERR_E;
}

static cmos_status_T i2c_close(const void *dev_id)
{
    return cmos_ERR_E;
}










/* 测试使用 */
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

