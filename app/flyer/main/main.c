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
#define DATA_SIZE       (18)
#define GET_TIME_TIMES  (1000)

/************************************ 头文件 ***********************************/
#include "config.h"
#include "misc.h"
#include "console.h"
#include "imu.h"
#include "mpu9250.h"
#include "main.h"
#include "inv_mpu.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/ 
uint32_T g_get_accel_data_time = 0;
uint32_T g_get_gyro_data_time = 0;

static int32_T s_task_flag = 0;

typedef struct{
    unsigned long time1;
    unsigned long time2;
    unsigned long time3;
    float accel[3];
    float gyro[3];
    float compass[3];
}data_T;

/********************************** 函数声明区 *********************************/
static void init(void);
static void task_gyro(void);
static void task_accel(void);
static void task_mag(void);

static void get_time(void);

static void clock_init(void);

static void get_gyro(float *gyro, unsigned long *time_stamp);
static void get_accel(float *accel, unsigned long *time_stamp);
static void get_compass(float *compass, unsigned long *time_stamp);

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
static data_T data[DATA_NUM];
int main(void)
{ 
    init();

    int i = 0;
    for(i = 0; i < DATA_NUM; i++)
    { 
        get_gyro(data[i].gyro, &data[i].time1);
        get_accel(data[i].accel, &data[i].time2);
        get_compass(data[i].compass, &data[i].time3);

        debug_log("accel  :% 5ld,%7.4f,%7.4f,%7.4f\r\n", data[i].time1, data[i].accel[0], data[i].accel[1], data[i].accel[2]);
        debug_log("gyro   :% 5ld,%7.4f,%7.4f,%7.4f\r\n", data[i].time2, data[i].gyro[0], data[i].gyro[1], data[i].gyro[2]);
        debug_log("compass:% 5ld,%7.4f,%7.4f,%7.4f\r\n", data[i].time2, data[i].compass[0], data[i].compass[1], data[i].compass[2]);
        debug_log("\r\n");
        HAL_Delay(1000);
    }

#if 0
#if 1
    int i = 0;
    for(i = 0; i < DATA_NUM; i++)
    {
        /* 时间 */
        data[i].time1 = HAL_GetTick();
        /* accel */
        imu_read(MPU9250_DEV_ADDR, MPU9250_ACCEL_DATA_ADDR, data[i].data, 6);
        data[i].time2 = HAL_GetTick();

        /* gyro */
        imu_read(MPU9250_DEV_ADDR, MPU9250_GYRO_DATA_ADDR, data[i].data, 6);
        data[i].time3 = HAL_GetTick();

        /* mag 实现mag数据读取 */
        //imu_read(MPU9250_DEV_ADDR, 0xXX, data[i].data + 12, 6);
        //short compass[3] = {0};
        //mpu_get_compass_reg(compass, NULL); 
        //data[i].time4 = HAL_GetTick();
    } 


    float gyro_sens = 0.0f;
    unsigned short accel_sens = 0;

    int x_i = 0;
    int y_i = 0;
    int z_i = 0;

    float x = 0;
    float y = 0;
    float z = 0; 

    mpu_get_gyro_sens(&gyro_sens);
    mpu_get_accel_sens(&accel_sens);
    
    for(i = 0; i < DATA_NUM; i++)
    { 
        debug_log("time:%05d,%05d,%05d,%05d\r\n", data[i].time1, data[i].time2, data[i].time3, data[i].time4);

        x_i = (data[i].data[0] << 8 | data[i].data[1]);
        y_i = (data[i].data[2] << 8 | data[i].data[3]);
        z_i = (data[i].data[4] << 8 | data[i].data[5]);

        x = x_i / gyro_sens;
        y = y_i / gyro_sens;
        z = z_i / gyro_sens;

        debug_log("accel:%7.4f,%7.4f,%7.4f,", x, y, z);

        x_i = (data[i].data[6] << 8 | data[i].data[7]);
        y_i = (data[i].data[8] << 8 | data[i].data[9]);
        z_i = (data[i].data[10] << 8 | data[i].data[11]);

        x = 1.0 * x_i / accel_sens;
        y = 1.0 * y_i / accel_sens;
        z = 1.0 * z_i / accel_sens;
        debug_log("gyro:%7.4f,%7.4f,%7.4f\r\n", x, y, z);

        debug_log("accel:0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,", 
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

        debug_log("\r\n");
    } 
    
    debug_log("sens:%7.4f, %d\r\n", gyro_sens, accel_sens);

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

    /* 参数设置 */
    get_time();
    debug_log("参数设置完成.\r\n");
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

/* TODO:使用中断测试延迟 */
static uint32_T get_accel_gyro_time(uint8_T addr)
{
    uint32_T start = 0;
    uint32_T end = 0;
    uint32_T max = 0;
    uint8_T  buf[6];

    int i = 0;
    for(i = 0; i< GET_TIME_TIMES; i++)
    {
        /* 时间 */
        start = HAL_GetTick();

        /* accel */
        imu_read(MPU9250_DEV_ADDR, addr, buf, 6);

        end = HAL_GetTick();

        if(max < end - start)
        {
            max = end - start;
        }
    }

    return max;
}

static void get_time(void)
{ 
    g_get_accel_data_time = get_accel_gyro_time(MPU9250_ACCEL_DATA_ADDR);
    g_get_gyro_data_time = get_accel_gyro_time(MPU9250_GYRO_DATA_ADDR);
}

static void get_gyro(float *gyro, unsigned long *time_stamp)
{ 
    short gyro_i[3] = {0};
    float sens = 0;

    mpu_get_gyro_reg(gyro_i, time_stamp);
    mpu_get_gyro_sens(&sens);
    for(int i=0;i<3;i++)
    {
        gyro[i] = gyro_i[i] / sens;
    }
}

static void get_accel(float *accel, unsigned long *time_stamp)
{
    short accel_i[3] = {0};
    unsigned short sens = 0;

    mpu_get_accel_reg(accel_i, time_stamp);
    mpu_get_accel_sens(&sens);
    for(int i=0;i<3;i++)
    {
        accel[i] = 1.0 * accel_i[i] / sens;
    }
}

static void get_compass(float *compass, unsigned long *time_stamp)
{ 
    short compass_i[3] = {0};

    mpu_get_compass_reg(compass_i, time_stamp); 

    for(int i=0;i<3;i++)
    {
        compass[i] = 1.0 * compass_i[i];
    }
}
