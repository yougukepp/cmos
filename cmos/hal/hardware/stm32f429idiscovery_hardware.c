/******************************************************************************
 *
 * 文件名  ： stm32f4xx_hal_msp.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150615
 * 版本号  ： 1.0
 * 文件描述： stm32f4 cube hal 初始化回调
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "stm32f429idiscovery_hardware.h"
#include "misc.h"
#include "console.h"
#include "device.h"
#include "uart.h"
#include "i2c.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* FIXME: 设备较多的话 占用较大堆内存 启动后 可以考虑释放 */
const static cmos_hal_uart_init_para_T s_uart_console_para =
{
    .uart_index = CMOS_CONSOLE_INDEX,
    .baud_rate = CMOS_CONSOLE_BAUDRATE
};

const static cmos_hal_i2c_init_para_T s_i2c_imu_para =
{
    .bus_num = CMOS_IMU_I2C_INDEX,
    .device_speed = CMOS_IMU_I2C_RATE
};

const cmos_hardware_init_item_T g_hardware_init_list[] =
{ 
    {cmos_hal_uart_init, (void *)&s_uart_console_para},
    {cmos_hal_i2c_init, (void *)&s_i2c_imu_para},
    {NULL, NULL}
};

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : HAL_UART_MspInit
* 负责人  : 彭鹏
* 创建日期: 20150615
* 函数功能: stm32f4 hal uart初始化回调
*
* 输入参数: stm32 hal uart句柄
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
    cmos_assert(NULL != huart, __FILE__, __LINE__);

    cmos_status_T status = cmos_ERR_E;
    /************************ 控制台使用的串口初始化 ************************/
    GPIO_InitTypeDef  GPIO_InitStruct;

    CONSOLE_UART_TX_GPIO_CLK_ENABLE();
    CONSOLE_UART_RX_GPIO_CLK_ENABLE();
    CONSOLE_UART_CLK_ENABLE();

    GPIO_InitStruct.Pin       = CONSOLE_UART_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = CONSOLE_UART_TX_AF;
    HAL_GPIO_Init(CONSOLE_UART_TX_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CONSOLE_UART_RX_PIN;
    GPIO_InitStruct.Alternate = CONSOLE_UART_RX_AF;
    HAL_GPIO_Init(CONSOLE_UART_RX_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CONSOLE_UART_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CONSOLE_UART_IRQn);

    status = hal_device_add(CMOS_CONSOLE_NAME, &g_uart_driver);
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    /**************************** 其他串口初始化 ****************************/
}

/*******************************************************************************
*
* 函数名  : HAL_UART_MspDeInit
* 负责人  : 彭鹏
* 创建日期: 20150615
* 函数功能: stm32f4 hal uart解初始化回调
*
* 输入参数: stm32 hal uart句柄
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    cmos_assert(NULL != huart, __FILE__, __LINE__);
    cmos_status_T status = cmos_ERR_E;
    /*********************** 控制台使用的串口解初始化 ***********************/
    CONSOLE_UART_FORCE_RESET();
    CONSOLE_UART_RELEASE_RESET();

    HAL_GPIO_DeInit(CONSOLE_UART_TX_GPIO_PORT, CONSOLE_UART_TX_PIN);
    HAL_GPIO_DeInit(CONSOLE_UART_RX_GPIO_PORT, CONSOLE_UART_RX_PIN);

    HAL_NVIC_DisableIRQ(CONSOLE_UART_IRQn);

    status = hal_device_del(CMOS_CONSOLE_NAME);
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    /*************************** 其他串口解初始化 ***************************/
}

/*******************************************************************************
*
* 函数名  : HAL_I2C_MspInit
* 负责人  : 彭鹏
* 创建日期: 20151113
* 函数功能: stm32f4 hal i2c初始化回调
*
* 输入参数: stm32 hal i2c句柄
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    cmos_assert(NULL != hi2c, __FILE__, __LINE__);

    cmos_status_T status = cmos_ERR_E; 
    GPIO_InitTypeDef  GPIO_InitStruct; 

    /************************** 其他I2C3总线初始化 **************************/
    IMU_I2C_SDA_GPIO_CLK_ENABLE();
    IMU_I2C_SCL_GPIO_CLK_ENABLE();

    /* 配置管脚 */
    GPIO_InitStruct.Pin       = IMU_I2C_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = IMU_I2C_SCL_SDA_AF;
    HAL_GPIO_Init(IMU_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = IMU_I2C_SDA_PIN;
    HAL_GPIO_Init(IMU_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);

    /* 使能时钟 */
    IMU_I2C_CLOCK_ENABLE();
    IMU_I2C_FORCE_RESET();
    IMU_I2C_RELEASE_RESET(); 
    
    /* 设置I2C中断优先级 */
    HAL_NVIC_SetPriority(IMU_I2C_EV_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(IMU_I2C_EV_IRQn);
    HAL_NVIC_SetPriority(IMU_I2C_ER_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(IMU_I2C_ER_IRQn); 
    
    /* 设置中断 */
    IMU_INT_CLK_ENABLE();
    GPIO_InitStruct.Pin   = IMU_INT_PIN;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(IMU_INT_GPIO_PORT, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(IMU_INT_EXTI, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(IMU_INT_EXTI);

    /* 加入总线设备 */
    status = hal_device_add(CMOS_IMU_I2CBUS_NAME, &g_i2c_driver);
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    /************************** 其他I2C总线初始化 ***************************/
}

/*******************************************************************************
*
* 函数名  : HAL_I2C_MspDeInit
* 负责人  : 彭鹏
* 创建日期: 20151113
* 函数功能: stm32f4 hal i2c解初始化回调
*
* 输入参数: stm32 hal i2c句柄
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : TODO: 实现stm32f4 hal 层解初始化
*
******************************************************************************/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    cmos_assert(NULL != hi2c, __FILE__, __LINE__);

    cmos_status_T status = cmos_ERR_E;

    status = hal_device_del(CMOS_IMU_I2CBUS_NAME);
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    /************************ 其他I2C总线解除初始化 *************************/
}

