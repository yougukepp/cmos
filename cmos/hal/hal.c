/******************************************************************************
 *
 * 文件名  ： hal.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150615
 * 版本号  ： 1.0
 * 文件描述： cmos hal,用于封装stm32 hal
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "hal.h"
#include "uart.h"
#include "i2c.h"
#include "cortex.h"
#include "stm32f429idiscovery_hardware.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_status_T SystemClock_Config(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : hal_init
* 负责人  : 彭鹏
* 创建日期: 20150615
* 函数功能: cmos hal初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
cmos_status_T hal_init(void)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_int32_T i = 0;

    /* stm32 hal 初始化 */
    if(HAL_OK != HAL_Init())
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    /* HAL_Init 执行后才可以使用 */
    /* 时钟配置 180M */
    status = SystemClock_Config();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    } 

    /* 设置核心中断优先级 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SVCall_IRQn, SVC_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(PendSV_IRQn, PENDSV_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);
    cmos_hal_cortex_cortex_systick_disable(); /* 避免进入时钟心跳 干扰初始化 */

    /* 逐个初始化硬件 */
    while(NULL != g_hardware_init_list[i].init)
    {
        g_hardware_init_list[i].init(g_hardware_init_list[i].para);
        i++;
    }

    return cmos_OK_E;
}

/* FIXME:不同的芯片需要修改 当前使用stm32f429 后面考虑stm32f401 */
static cmos_status_T SystemClock_Config(void)
{
    cmos_status_T status = cmos_ERR_E;

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    HAL_PWREx_EnableOverDrive();

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    return cmos_OK_E;
}

