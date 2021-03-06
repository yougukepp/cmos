/******************************************************************************
 *
 * 文件名  ： i2c.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151101 
 * 版本号  ： 1.0
 * 文件描述： cmos i2c 驱动程序实现
 * 版权说明： Copyright (c) GNU
 * 其    他： 实现非阻塞
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "i2c.h"
#include "cmos_api.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f429idiscovery_hardware.h"
#include "misc.h"
#include "console.h"
#include "task.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static void *i2c_open(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
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
static I2C_HandleTypeDef s_i2c_handle;

/* 从设备地址 */
static cmos_i2c_addr_T s_i2c_addr =
{
    .dev_addr = 0xff,
    .reg_offset = 0xff
};

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

/* TODO:
   1 实现 参数 管理 */
static void *i2c_open(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
    /* 由path返回相应的 s_uart_handle */
    return (void *)(&s_i2c_handle);
}

static cmos_int32_T i2c_read(const void *dev_id, void *buf, cmos_int32_T n_bytes)
{
    cmos_assert(NULL != dev_id, __FILE__, __LINE__);
    cmos_assert((NULL != buf) && (n_bytes > 0), __FILE__, __LINE__);
    cmos_assert(((0xff != s_i2c_addr.dev_addr) && (0xff != s_i2c_addr.reg_offset)), __FILE__, __LINE__); 

    /* 中断 读取 */
    if(HAL_OK != HAL_I2C_Mem_Read_IT((I2C_HandleTypeDef *)dev_id, s_i2c_addr.dev_addr, s_i2c_addr.reg_offset,
                I2C_MEMADD_SIZE_8BIT, buf, (cmos_uint16_T)(n_bytes)))
    {
        cmos_assert(FALSE, __FILE__, __LINE__);
    }

    return 0;
}

#if 0
static cmos_int32_T i2c_read_poll(const void *dev_id, void *buf, cmos_int32_T n_bytes)
{ 
    cmos_assert(NULL != dev_id, __FILE__, __LINE__);
    cmos_assert((NULL != buf) && (n_bytes > 0), __FILE__, __LINE__);
    cmos_assert(((0xff != s_i2c_addr.dev_addr) && (0xff != s_i2c_addr.reg_offset)), __FILE__, __LINE__);

    if(HAL_OK != HAL_I2C_Mem_Read((I2C_HandleTypeDef *)dev_id, s_i2c_addr.dev_addr, s_i2c_addr.reg_offset,
                I2C_MEMADD_SIZE_8BIT, buf, (cmos_uint16_T)(n_bytes), HAL_MAX_DELAY))
    {
        cmos_assert(FALSE, __FILE__, __LINE__);
    }

    return n_bytes;
}
#endif

/* 未使用 暂不实现 */
static cmos_int32_T i2c_write(const void *dev_id, const void *buf, cmos_int32_T n_bytes)
{ 
    cmos_assert(FALSE, __FILE__, __LINE__);
    return 0;
}

#if 0
static cmos_int32_T i2c_write_poll(const void *dev_id, const void *buf, cmos_int32_T n_bytes)
{ 
    cmos_assert(NULL != dev_id, __FILE__, __LINE__);
    cmos_assert((NULL != buf) && (n_bytes > 0), __FILE__, __LINE__);
    cmos_assert(((0xff != s_i2c_addr.dev_addr) && (0xff != s_i2c_addr.reg_offset)), __FILE__, __LINE__);

    if(HAL_OK != HAL_I2C_Mem_Write(&s_i2c_handle, s_i2c_addr.dev_addr, s_i2c_addr.reg_offset,
                I2C_MEMADD_SIZE_8BIT, (cmos_uint8_T *)buf, (cmos_uint16_T)(n_bytes), HAL_MAX_DELAY))
    {
        cmos_assert(FALSE, __FILE__, __LINE__);
    } 

    return n_bytes;
}
#endif

static cmos_status_T i2c_ioctl(const void *dev_id, cmos_uint32_T request, cmos_uint32_T mode)
{
    cmos_i2c_addr_T *addr = NULL;

    switch(request)
    {

        /* 设置从设备地址 */
        case CMOS_I_SET_I2C_ADDR:
            {
                addr = (cmos_i2c_addr_T *)mode;
                if(NULL == addr)
                { 
                    cmos_err_log("i2c_ioctl CMOS_I_SET_I2C_ADDR request with null addr.");
                    goto err;
                }
                s_i2c_addr.dev_addr = addr->dev_addr;
                s_i2c_addr.reg_offset = addr->reg_offset; 
                break;
            }
        default:
            {
                cmos_err_log("i2c_ioctl not implement 0x%08x request.", request);
                goto err;
            }
    } 
    
    return cmos_OK_E;

err:
    return cmos_PARA_E;
}

static cmos_status_T i2c_close(const void *dev_id)
{
    return cmos_ERR_E;
}

void I2C3_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&s_i2c_handle);
}

/* I2C读取完成回调 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* TODO: */
    /* 1、恢复接收任务 */
    /* 2、解锁接收功能 */
}

