/******************************************************************************
 *
 * 文件名  ： idle.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150416 
 * 版本号  ： v1.0
 * 文件描述： 实现idle线程 当没有用户线程时 系统仅有idle线程
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "idle.h"
#include "switch.h"
#include "systick.h"
#include "stm32f429i_discovery.h"
#include "uart.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/ 
static void idle_check_user_stack(void);
static void idle_check_cpu(void);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
static void cm_idle_thread(void const *argument)
{
    while (1)
    {
        idle_check_user_stack();
        idle_check_cpu();
        BSP_LED_Toggle(LED3);

        //__WFI();
    }
}

void idle_create(void)
{
    osThreadCreate(osThread(cm_idle_thread), NULL);
}

static void idle_check_user_stack(void)
{
    switch_check_user_stack();
}

/* TODO:负载太轻不好测试 */ 
static void idle_check_cpu(void)
{
    cm_uint32_t start = 0;
    cm_uint32_t end = 0;
    cm_uint32_t total = 0;
    cm_uint32_t idle = 0;
    cm_uint32_t user = 0;

    cm_int32_t i = 0;
    cm_float_t rate = 0;

    
    i = 0x2ffffff; /* 无负载 基准0x460ms */
    idle = 0x460;

    start = systick_get();
    while(i>0)
    {
        i--;
    }
    end = systick_get();

    /* TODO: tick溢出(49天)时会有一个错误 故循环减 */
    total = end - start;

    if(0 != total)
    {
        user = total - idle;
        rate = 100.0 * user / total;
    }
    else
    {
        rate = 0.0;
    }

    //printf("cpu usage rate;%f\n", rate);

    total = 0;
}

