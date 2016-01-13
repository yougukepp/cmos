/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20160112
 * 版本号  ： 1.2
 * 文件描述： 飞控主控模块 不使用os
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
/* 消除中文打印警告 */
#pragma  diag_suppress 870

#define DATA_NUM        (1000)
#define DATA_SIZE       (12)

/************************************ 头文件 ***********************************/
#include "config.h"
#include "misc.h"
#include "console.h"
#include "imu.h"
#include "mpu9250.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static int32_T s_task_flag = 0;

typedef struct{
    uint32_T time;
    uint8_T  data[DATA_SIZE];
}data_T;

/********************************** 函数声明区 *********************************/
static void init(void);
static void task_gyro(void);
static void task_accel(void);
static void task_mag(void);

static void clock_init(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20160112
* 函数功能: MPU9250 主函数
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 主程序永不返回
*
* 调用关系: 无
* 其 它   : 获取MPU9250数据 中断中完成
*
******************************************************************************/
int main(void)
{
    init();

#if 1
    HAL_Delay(1000); /* 1s等待 待稳定 */

    int i = 0;
    data_T data[DATA_NUM];
    for(i = 0; i < DATA_NUM; i++)
    {
        /* accel + gyro */
        imu_read(0xD0, 0x3B, data[i].data, DATA_SIZE);

        /* 研究 */
        //mpu_get_compass_reg(compass_i, time_stamp); 

        /* 时间 */
        data[i].time = HAL_GetTick();

    } 
    
    for(i = 0; i < DATA_NUM; i++)
    {
        debug_log("time:%05d", data[i].time);
        debug_log("accel:0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\t", 
                data[i].data[0],
                data[i].data[1],
                data[i].data[2],
                data[i].data[3],
                data[i].data[4],
                data[i].data[5]);
        debug_log("gyro: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x\r\n",
                data[i].data[6],
                data[i].data[7],
                data[i].data[8],
                data[i].data[9],
                data[i].data[10],
                data[i].data[11]);
    } 

    while(1);

#else
    /* 每轮循环从优先级最高的开始逐个检查 */
    /* GYRO > ACCEL > MAG */
    /* IMU(MPU9250)数据 获取优先级最高 使用中断 */
    while(1)
    {
        if(TASK_GYRO && s_task_flag)
        {
            task_gyro();
        }
        else if(TASK_ACCEL && s_task_flag)
        {
            task_accel();
        }
        else if(TASK_MAG && s_task_flag)
        { 
            task_mag();
        }
        else
        { 
            assert_failed(__FILE__, __LINE__);
        }
    }
#endif
}

/* 初始化 */
static void init(void)
{ 
    /* step1: 初始化硬件 */
    if(HAL_OK != HAL_Init())
    {
        while(1);
    }

    /* HAL_Init 执行后才可以使用 */
    /* 时钟配置 180M */
    clock_init();

    /* 设置核心中断优先级 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);

    /* 逐个初始化硬件 */
    /* 串口 */
    console_init(); /* 此后可以开始打印 */ 
    debug_log("串口初始化完成.\r\n");

    /* imu i2c */
    imu_init();
    debug_log("imu i2c 初始化完成.\r\n");

    /* bmp180 */
#if 0
    unsigned char val = 0;
    int i = 0;
    int iMax = 0;
    unsigned char bmp180_reg_addr[] = {
        0xAA, 0xAB,
        0xAC, 0xAD,
        0xAE, 0xAF,
        0xBA, 0xBB,
        0xBC, 0xBD,
        0xBE, 0xBF,
        0xD0, 0xE0, 0xF4, 0xF6, 0xF7, 0xF8};

    /* 初始化硬件 */

    debug_log("BMP180 寄存器值:\r\n");
    iMax = sizeof(bmp180_reg_addr) /sizeof(bmp180_reg_addr[0]);
    for(i=0;i<iMax;i++) 
    { 
        imu_read(0xEF, bmp180_reg_addr[i], &val, 1);
        debug_log("0x%02x:0x%02x\r\n", bmp180_reg_addr[i], val);
    }
    while(TRUE);
#else

    /* imu mpu9250 */
    mpu9250_init();
    debug_log("mpu9250 初始化完成.\r\n");
#endif

    /* step2: 启动任务 */
    s_task_flag = (TASK_GYRO | TASK_ACCEL | TASK_MAG);
}

/* 陀螺仪融合 */
static void task_gyro(void)
{}

/* 加计融合 */
static void task_accel(void)
{}

/* 磁场融合 */
static void task_mag(void)
{}

static void clock_init(void)
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
        while(1);
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
        while(1);
    }

    return;
}
