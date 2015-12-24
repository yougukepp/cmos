/******************************************************************************
 *
 * 文件名  ： init.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： init系统调用
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"
#include "init.h"
#include "console.h"
#include "kernel.h"
#include "vfs.h"
#include "idle.h"
#include "vfs.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
inline static void cmos_init_before(void);
inline static void cmos_init_after(void);

/* syscall.s中定义 */
void svc_init(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_init
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_init
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_init(void)
{ 
    cmos_init_before();
    svc_init();
    cmos_init_after();
}

/*******************************************************************************
 *
 * 函数名  : cmos_init_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_init 特权代码
 *
 * 输入参数: 与cmos_init一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_init_svc(void)
{
   cmos_kernel_init();
}

/*******************************************************************************
 *
 * 函数名  : cmos_init_before
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_init 进入特权代码之前的代码
 *
 * 输入参数: 与cmos_init一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_init_before(void)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_init
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_init 出特权代码之后的代码
 *
 * 输入参数: 与cmos_init一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_init_after(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* 打印目录树 */
    cmos_console_printf("cmos init done with vfs tree:\r\n");
    vfs_print();

    /* 创建idle任务 使用cmos_create_c */
    cmos_task_attribute_T idle_attribute =
    {
        .entry = cmos_task_idle_task,
        .argv = NULL,
        .priority = CMOS_IDLE_PRIORITY,
        .stack_size = CMOS_IDLE_STACK_SIZE,
        .tick_total = CMOS_IDLE_TICK_TOTAL,
        .flag = cmos_task_with_default
    };
    status = cmos_create(&g_idle_task_id, &idle_attribute);
    cmos_assert(cmos_OK_E == status, __FILE__, __LINE__);

    cmos_printf("task_idle create %d:0x%08x.\r\n", status, (cmos_int32_T)g_idle_task_id);
}

