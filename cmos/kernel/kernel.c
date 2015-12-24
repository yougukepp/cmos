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
#include "kernel.h"
#include "task.h"
#include "switch.h"
#include "hal.h"
#include "vfs.h"
#include "cortex.h"
#include "stm32f429idiscovery_hardware.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 空闲任务id */
cmos_task_id_T g_idle_task_id = 0;

/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/

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
void cmos_kernel_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* 关闭任务切换 保证初始化的串行完成 */
    cmos_hal_cortex_cortex_disalbe_switch();
	
    /* cmos hal vfs初始化 */
    status = vfs_init();
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    /* cmos hal 硬件底层初始化 */
    status = hal_init();
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    /* 初始化用户内存 便于调试 */
#if (CMOS_DEBUG_LEVEL > 0) 
    cmos_int32_T *sp = NULL;
    cmos_int32_T i = 0;
    cmos_int32_T iMax = 1024 * 10; /* 10kword 40kbyte */
    sp = (int *)CMOS_TASK_STACK_BASE;
    for(i = 1; i <= iMax; i++)
    {
        sp -= 1;
        *sp = 0xA5A5A5A5;
    }
#endif
    return;
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
void cmos_kernel_start(void)
{ 
    cmos_task_switch_start();
}

/*******************************************************************************
*
* 函数名  : cmos_kernel_running
* 负责人  : 彭鹏
* 创建日期: 20151218
* 函数功能: CMOS 是否启动(已经开始多任务)
*
* 输入参数: 无
* 输出参数: 无
*
* 返回值  : cmos_RUNNING_E 多任务
*           cmos_SINGLE_E  尚未开始多任务(单任务)
* 调用关系: 无
* 其 它   : 永不返回
*
******************************************************************************/
inline cmos_status_T cmos_kernel_status(void)
{ 
    /* 获取当前任务 */
    cmos_task_tcb_T *s_current_tcb = cmos_task_self();
    if(NULL == s_current_tcb)
    {
        return cmos_RUNNING_E;
    }
    return cmos_SINGLE_E;
}

