/******************************************************************************
 *
 * 文件名  ： kernel/c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.0
 * 文件描述： 系统整体功能的api 与CMSIS Kernel Information and Control对应
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"
#include "idle.h"
#include "switch.h"
#include "vfs.h"
#include "hal.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/
/* 空闲任务id */
static cmos_task_id_T s_idle_task_id = 0;


/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_kernel_init
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: CMOS 初始化
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
cmos_status_T cmos_kernel_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* cmos hal vfs初始化 */
    status = vfs_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }


    /* cmos hal 硬件底层初始化 */
    status = hal_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    /* 尽早初始化控制台便于打印 所以放在这里而没有放在hal_init函数执行之后 */
    status = cmos_console_init(CMOS_CONSOLE_BAUDRATE);
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }
    /* 后面的初始化可以使用控制台输出了 */

    /* 打印目录树 */
    cmos_printf("cmos init done with vfs tree:\r\n");
    vfs_print(); 

#if 1 /* 初始化用户内存 便于调试 */
#define CMOS_TASK_STACK_BASE  (0x20030000)
    cmos_int32_T *sp = NULL;
    cmos_int32_T i = 0;
    cmos_int32_T iMax = 1024 * 10; /* 10kword 40kbyte */
    sp = (int *)CMOS_TASK_STACK_BASE;
    for(i = 1; i <= iMax; i++)
    {
        sp -= 1;
        *sp = 0xA5A5A5A5;
    }
#undef CMOS_TASK_STACK_BASE
#endif

    /* 创建idle任务 使用cmos_create系统调用 */
    cmos_task_attribute_T idle_attribute =
    {
        .priority = CMOS_IDLE_PRIORITY,
        .stack_size = CMOS_IDLE_STACK_SIZE,
        .tick_total = CMOS_IDLE_TICK_TOTAL,
        .flag = cmos_task_with_default
    };
    status = cmos_create(&s_idle_task_id, cmos_task_idle_task, NULL, &idle_attribute); 
    if(cmos_OK_E != status)
    {
        CMOS_ERR_STR("create idle task failed.");
        return status;
    }

    return status;
}

/*******************************************************************************
*
* 函数名  : cmos_kernel_start
* 负责人  : 彭鹏
* 创建日期: 20151119
* 函数功能: CMOS 调度启动
*
* 输入参数: 无
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 永不返回
*
******************************************************************************/
cmos_status_T cmos_kernel_start(void)
{ 
    cmos_task_switch_start();

    /* 永不返回 */
    return cmos_OK_E;
}
