/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： 1.0
 * 文件描述： 操作系统测试桩主文件
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmsis_os.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stlogo.h"
#include "misc.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void Display_DemoDescription(void);
static void HardWare_Init(void);

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

/*******************************************************************************
*
* 函数名  : job1
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: 任务1
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void job1 (void const *argv);
osThreadDef(job1, osPriorityNormal, 1, 0x1000);

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: 操作系统测试桩主函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: startup_<device_name>.s 调用的第二个函数
* 其 它   : 无
*
******************************************************************************/
int main(void)
{
    HardWare_Init();

    osKernelInitialize(); 
    osThreadCreate(osThread(job1), NULL);
    osKernelStart();
	
    while(1);
}

static void job1 (void const *argument)
{
    while (1)
    {
        BSP_LED_Toggle(LED4);
        osDelay(300); /* 延迟300ms */
    }
}

static void Display_DemoDescription(void)
{
  uint8_t desc[50];
  
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);
  
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  
  
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)"STM32F429I BSP", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t*)"Drivers examples", CENTER_MODE);
  
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 65, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t*)"Copyright (c) STMicroelectronics 2014", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t*)"Press USER Button to start:", CENTER_MODE);
  //sprintf((char *)desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 45, (uint8_t *)desc, CENTER_MODE);   
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

static void HardWare_Init(void)
{
    HAL_Init(); 
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4); 
    /* 系统时钟180 MHz */
    SystemClock_Config(); 
    /* Configure USER Button */
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI); 
    /*##-1- Initialize the LCD #################################################*/
    /* Initialize the LCD */
    BSP_LCD_Init(); 
    /* Initialize the LCD Layers */
    BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER); 

    Display_DemoDescription();
}

