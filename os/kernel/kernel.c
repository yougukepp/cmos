/******************************************************************************
 *
 * 文件名  ： kernel.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150410 
 * 版本号  ： v1.0
 * 文件描述： 内核信息与控制(Kernel Information and Control)
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "cmos_config.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "misc.h"
#include "mem.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
/*******************************************************************************
*
* 函数名  : SystemClock_Config
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: 系统时钟配置
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: main调用
* 其 它   : 系统时钟配置如下: 
*           System Clock source            = PLL (HSE)
*           SYSCLK(Hz)                     = 180000000
*           HCLK(Hz)                       = 180000000
*           AHB Prescaler                  = 1
*           APB1 Prescaler                 = 4
*           APB2 Prescaler                 = 2
*           HSE Frequency(Hz)              = 8000000
*           PLL_M                          = 8
*           PLL_N                          = 360
*           PLL_P                          = 2
*           PLL_Q                          = 7
*           VDD(V)                         = 3.3
*           Main regulator output voltage  = Scale1 mode
*           Flash Latency(WS)              = 5
*
******************************************************************************/
static void SystemClock_Config(void);
/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/

void syscall_kernel_initialize(void)
{
    mem_init();
   
    HAL_Init(); 
  
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4); 
    
    /* 系统时钟180 MHz */
    SystemClock_Config(); 

    /* 使能所有的Fault */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk; 
    
    /* TODO:关中断 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SVCall_IRQn, SVC_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(PendSV_IRQn, PENDSV_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);
    /* TODO:开中断 */ 
    
    /* TODO:SVC中无法实现 不开浮点 使用MSP 非特权级 */ 
    __set_CONTROL(0x00000001);
}

void syscall_kernel_start(void)
{
    /* psp指向最先创建的任务(idle)栈顶 启动之 */
    cm_uint32_t *psp_top = CMOS_FIRST_STACK_INIT_TOP;
    __set_PSP((cm_uint32_t)psp_top); /* 初始化PSP */ 
    
    /* TODO:SVC中无法实现 不开浮点 使用PSP 非特权级 */ 
    __set_CONTROL(0x00000003);
    void first_thread_start();
    first_thread_start(CMOS_INITIAL_EXEC_RETURN);
}


static void SystemClock_Config(void)
{
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
    Error_Handler();
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
    Error_Handler();
  }
}
