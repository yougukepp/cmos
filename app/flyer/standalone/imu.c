/******************************************************************************
 *
 * 文件名  ： imu.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20160112 
 * 版本号  ： 1.0
 * 文件描述： imu i2c 驱动程序实现
 * 版权说明： Copyright (c) GNU
 * 其    他： 实现非阻塞
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "imu.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f429idiscovery_hardware.h"
#include "misc.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* STM32F4Cube HAL驱动 */
static I2C_HandleTypeDef s_imu_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
void imu_init(void)
{
    if(HAL_I2C_STATE_RESET != HAL_I2C_GetState(&s_imu_handle))
    {
        assert_failed(__FILE__, __LINE__);
    }

    s_imu_handle.Instance              = IMU_I2C;
    s_imu_handle.Init.ClockSpeed       = IMU_RATE;
    s_imu_handle.Init.DutyCycle        = I2C_DUTYCYCLE_2;
    s_imu_handle.Init.OwnAddress1      = 0;
    s_imu_handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    s_imu_handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    s_imu_handle.Init.OwnAddress2      = 0;
    s_imu_handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    s_imu_handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE; 
    
    if(HAL_OK != HAL_I2C_Init(&s_imu_handle))
    {
        assert_failed(__FILE__, __LINE__);
    }

    return;
}

void imu_read_poll(uint8_T dev_addr, uint16_T reg_addr, uint8_T *buf, uint32_T n)
{
    if(HAL_OK != HAL_I2C_Mem_Read(&s_imu_handle, dev_addr, reg_addr,
                I2C_MEMADD_SIZE_8BIT, buf, (uint16_T)(n), HAL_MAX_DELAY))
    {
        assert_failed(__FILE__, __LINE__);
    }

    return;
}

void imu_write_poll(uint8_T dev_addr, uint16_T reg_addr, const uint8_T *buf, uint32_T n)
{ 
    if(HAL_OK != HAL_I2C_Mem_Write(&s_imu_handle, dev_addr, reg_addr,
                I2C_MEMADD_SIZE_8BIT, (uint8_T *)buf, (uint16_T)(n), HAL_MAX_DELAY))
    {
        assert_failed(__FILE__, __LINE__);
    } 

    return;
}

/* 以下中断 读 作为线程实体 */
static uint8_T s_dev_addr = 0;
static uint16_T s_reg_addr = 0;
static uint8_T *s_buf = NULL;
static uint32_T s_n = 0;

static uint32_T s_start = 0;
static uint32_T s_max = 0;

void imu_read(uint8_T dev_addr, uint16_T reg_addr, uint8_T *buf, uint32_T n)
{ 
    s_start = HAL_GetTick();

    s_dev_addr = dev_addr;
    s_reg_addr = reg_addr;
    s_buf = buf;
    s_n = n;

    /* 中断 读取 */
    /* TODO:优化性能 不做冗余处理 仅启动中断读 */
    if(HAL_OK != HAL_I2C_Mem_Read_IT(&s_imu_handle, dev_addr, reg_addr,
                I2C_MEMADD_SIZE_8BIT, buf, (uint16_T)(n)))
    {
        assert_failed(__FILE__, __LINE__);
    }

    return;
}

/* 中断相关代码作为前台 移入main */
void I2C3_EV_IRQHandler(void)
{
    /* TODO:优化性能 不做冗余处理 仅处理读完成标记 */
    HAL_I2C_EV_IRQHandler(&s_imu_handle);
}

/* I2C读取完成回调 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    uint32_T max_new = HAL_GetTick() - s_start;
    if(s_max < max_new)
    {
        s_max = max_new;
    }

    if(s_n > 10)
    {
        s_buf = s_buf + 0;
    }
    else
    {
        s_buf = s_buf + 0;
    }


    /* 重新开启读 */
    imu_read(s_dev_addr, s_reg_addr, s_buf, s_n);
}

inline uint32_T get_imu_read_max(void)
{
    return s_max;
}

