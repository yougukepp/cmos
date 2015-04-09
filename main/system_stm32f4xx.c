/******************************************************************************
 *
 * 文件名  ： system_stm32f4xx.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： v1.0
 * 文件描述： 硬件时钟等配置
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/
/*******************************************************************************
 *
 * 变量功能: 标识系统时钟
 * 取值范围: 无
 * 注意事项: 这个变量有三种方式更新: 
 *           1. 调用 CMSIS 函数 SystemCoreClockUpdate()
 *           2. 调用 HAL API函数 HAL_RCC_GetHCLKFreq()
 *           3. 每次调用 HAL_RCC_ClockConfig()
 *
 *           注意:如果使用这个函数配置系统时钟,就没有必要调用1、2两个函数,
 *                因为SystemCoreClock将会自动更新.
 *
 *******************************************************************************/
uint32_t SystemCoreClock = 16000000;
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : SystemInit
 * 负责人  : 彭鹏
 * 创建日期：20150321 
 * 函数功能: 系统初始化
 *           - 初始化 FPU,
 *           - 初始化 异常向量表(vector table location)
 *           - 初始化 外部存储(External memory)
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值:   无
 *
 * 调用关系: startup_<device_name>.s 调用的第一个函数
 * 其 它:    无
 *
 *
 ******************************************************************************/
void SystemInit(void)
{
  /* FPU设置 */
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    /* CP10 & CP11 完全进入(Full Access) */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
  #endif

  /* 复位RCC时钟配置 */
  /* 设置 HSION 位 */
  RCC->CR |= (uint32_t)0x00000001;

  /* 复位 CFGR 寄存器 */
  RCC->CFGR = 0x00000000;

  /* 复位 HSEON、CSSON和PLLON 位 */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* 复位 PLLCFGR 寄存器 */
  RCC->PLLCFGR = 0x24003010;

  /* 复位 HSEBYP 位 */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* 关闭所有中断 */
  RCC->CIR = 0x00000000;

  /* 配置异常向量表偏移 */
#ifdef VECT_TAB_SRAM
  /* 异常向量表重定位到片内SRAM */
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
#else
  /* 异常向量表重定位到片内FLASH */
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
#endif
}

/*******************************************************************************
 *
 * 函数名  : SystemCoreClockUpdate
 * 负责人  : 彭鹏
 * 创建日期：20150321 
 * 函数功能: 更新SystemCoreClock变量至时钟寄存器
 *           SystemCoreClock包括核心时钟(HCLK)
 *           用户应用可以使用它设置SysTick定时器或其他参数
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值:   无
 *
 * 调用关系: 无
 * 其 它:    
 *           1. 每次HCLK改变,该函数需要调用(用来更改SystemCoreClock值),
 *              否则任何基于这个变量(SystemCoreClock)的配置将不正确.
 *           2. 通过这个函数计算出来的系统频率(system frequency)不是
 *              芯片的真实的频率.它仅仅是基于预分频常量(predefined constant)
 *              和所选的时钟源的计算值.
 *
 *              - 如果 系统时钟源(SYSCLK source)是HSI,SystemCoreClock将基于HSI_VALUE(*)
 *
 *              - 如果 系统时钟源(SYSCLK source)是HSE,SystemCoreClock将基于HSE_VALUE(**)
 *
 *              - 如果 系统时钟源(SYSCLK source)是PLL,SystemCoreClock将基于HSI_VALUE(*)
 *                或者 HSE_VALUE(**) 的PLL因子的积或商
 *
 *              (*) HSI_VALUE 是一个定义于 stm32f4xx_hal_conf.h 文件的常量,
 *                  它的默认值是16MHz,但是实际值取决于系统电压和温度
 *
 *             (**) HSE_VALUE 是一个定义于 stm32f4xx_hal_conf.h 文件的常量,
 *                  它的值应该与电路的晶体振荡器保持相同,否则该函数回计算出错误的值
 *
 ******************************************************************************/
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
  
  /* 获取时钟源(SYSCLK source) */
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI 作为时钟源(system clock source) */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE 作为时钟源(system clock source) */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL 作为时钟源(system clock source) */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
       * SYSCLK = PLL_VCO / PLL_P */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
      if (pllsource != 0)
      {
        /* HSE 作为PLL时钟源(PLL clock source) */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {
        /* HSI 作为PLL时钟源(PLL clock source) */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* 计算 HCLK 频率 */
  /* 获取 HCLK 预分频(prescaler) */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK 频率 */
  SystemCoreClock >>= tmp;
}

