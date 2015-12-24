/******************************************************************************
 *
 * 文件名  ： syscall.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： 1.1
 * 文件描述： 系统调用总控
 * 版权说明： Copyright (c) GNU
 * 其    他： 每个系统调用的主要逻辑在对应的文件中
 *            例如cmos_write系统调用在write.c中
 *            系统调用包含3部分:
 *            1. 用户态  执行类似锁定等需要用户态执行的逻辑
 *            2. svc执行 状态切换
 *            3. 内核态  执行特权级下执行的逻辑 由于svc中的级别很高 等效于关中断
 *
 *            _u 用户态代码
 *            _p 内核态代码
 *            syscall.s 中完成svc指令
 *
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>

#include "cmos_config.h"
#include "stm32f4xx_hal_conf.h"
#include "cmos_api.h"

#include "syscall.h"
#include "open.h"
#include "write.h"
#include "ipc.h"

#include "cortex.h"
#include "console.h"
#include "kernel.h"
#include "task.h"
#include "switch.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : syscall_c
 * 负责人  : 彭鹏
 * 创建日期：20150409 
 * 函数功能: 系统调用C主逻辑
 *
 * 输入参数: sp任务堆栈
 *
 * 输出参数: 无
 *
 * 返回值  : 调用状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void syscall_c(cmos_uint32_T *sp)
{
    if(NULL == sp)
    {
        CMOS_ERR_STR("sp is null");
    }

    cmos_uint8_T svc_number = ((cmos_uint8_T *) sp[6])[-2];

    cmos_uint32_T stacked_r0 = sp[0];
    cmos_uint32_T stacked_r1 = sp[1];
    cmos_uint32_T stacked_r2 = sp[2];

    /***************************************************************************
     *
     *  系统调用编号规则:
     *  - 系统调用编号1 Byte.
     *  - 高四位指示系统调用类别()
     *  - 低四位指示系统调用类别中的不同调用 
     *    目前已经定义的系统调用号：
     *      0x0 内核基本控制(参考CMSIS Kernel Information and Control)
     *        0x00 初始化
     *        0x01 多任务启动
     *        0x02 多任务是否启动
     *      0x1 任务控制(参考CMSIS Thread Management)
     *        0x10 创建任务
     *      0x2 时间管理(参考CMSIS Thread Management)
     *        0x20 延迟
     *      0x3 任务通信与同步
     *        最强IPC 对实时性(中断延迟)影响巨大 数条指令使用
     *        0x30 ipc相关系统调用
     *      0xa 驱动系统调用(利用Linux VFS思想)
     *        0xa0 打开文件
     *        0xa1 关闭文件
     *        0xa2 读文件
     *        0xa3 写文件
     *        0xa4 文件杂项 类似Linux ioctl
     *
     **************************************************************************/
    switch(svc_number)
    {
        /* 内核基本 */
        case 0x00:
            {
                cmos_init_p();
                break;
            }
        case 0x01:
            { 
                cmos_start_p();
                break;
            }
        case 0x02:
            { 
                sp[0] = cmos_running_p();
                break;
            }

        /* 任务控制 */
        case 0x10:
            { 
                sp[0] = cmos_create_p((cmos_task_id_T *)stacked_r0, (const cmos_task_attribute_T *)stacked_r1);
                break;
            }

        /* 时间管理 */
        case 0x20:
            { 
                sp[0] = cmos_delay_p((cmos_int32_T)stacked_r0);
                break;
            }

        /* 任务通信与同步 */
        case 0x30:
            {
                cmos_ipc_svc((cmos_ipc_type_T)stacked_r0, (void *)stacked_r1);
            }

        /* 驱动系统调用(利用Linux VFS思想) */
        case 0xa0:
            { 
                sp[0] = (cmos_fd_T)cmos_open_svc((const cmos_int8_T *)stacked_r0, (cmos_uint32_T)stacked_r1, stacked_r2); 
                break;
            }
        case 0xa1:
            {
                sp[0] = cmos_close_p((cmos_fd_T)stacked_r0); 
                break;
            }
        case 0xa2:
            {
                sp[0] = cmos_read_p((cmos_fd_T)stacked_r0, (void *)stacked_r1, (cmos_int32_T)stacked_r2);
                break;
            }
        case 0xa3:
            {
                sp[0] = cmos_write_svc((cmos_fd_fcb_T *)stacked_r0, (void *)stacked_r1, (cmos_int32_T)stacked_r2);
                break;
            }
        case 0xa4:
            {
                sp[0] = cmos_ioctl_p((cmos_fd_T)stacked_r0, (cmos_uint32_T)stacked_r1, stacked_r2);
                break;
            }
        default:
            {
                cmos_assert(FALSE, __FILE__, __LINE__);
                break;
            }
    }

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_init_p
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 内核初始化 特权
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_init_p(void)
{
   cmos_kernel_init();
}


/*******************************************************************************
 *
 * 函数名  : cmos_start_p
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 多任务启动 特权
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_start_p(void)
{
    cmos_kernel_start();
}

/*******************************************************************************
 *
 * 函数名  : cmos_running_c
 * 负责人  : 彭鹏
 * 创建日期：20151218 
 * 函数功能: 多任务是否启动 特权
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : TRUE  多任务启动
 *           FALSE 多任务未启动
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_bool_T cmos_running_p(void)
{
    return cmos_kernel_running();
}

/*******************************************************************************
 *
 * 函数名  : cmos_create_p
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 创建任务 特权
 *
 * 输入参数: task_attribute 任务入口 任务参数 任务属性 堆栈 优先级 等
 * 输出参数: task_id 任务id号
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_create_p(cmos_task_id_T *task_id, 
        const cmos_task_attribute_T *task_attribute)
{
    cmos_status_T status = cmos_ERR_E;
    if(NULL == task_attribute)
    {
        CMOS_ERR_STR("task attribute should not to be null.");
        return cmos_NULL_E;
    }

    status = cmos_task_create(task_id, task_attribute);

    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_delay_p
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: 延迟当前任务 特权
 *
 * 输入参数: 延迟时间(CMOS_TICK_TIMES)数
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : CMOS_TICK_TIMES一般以ms为单位 该函数延迟任务millisec毫秒
 *
 ******************************************************************************/
inline cmos_status_T cmos_delay_p(cmos_int32_T millisec)
{
    return cmos_task_delay(millisec);
}

/*******************************************************************************
 *
 * 函数名  : cmos_close_p
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_close 特权
 *
 * 输入参数: fd 文件句柄
 *
 * 输出参数: 无
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_close_p(cmos_fd_T fd)
{
    cmos_assert(0 != fd, __FILE__, __LINE__);
    cmos_fd_close((cmos_fd_fcb_T *)fd);
    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_read_p
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_read 特权
 *
 * 输入参数: fd      文件句柄
 *           buf     读取数据的缓存
 *           n_bytes 要求读取的字节数
 *
 * 输出参数: 无
 * 返回值  : 实际读取字节数
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_int32_T cmos_read_p(cmos_fd_T fd, void *buf, cmos_int32_T n_bytes)
{
    cmos_assert(0 != fd, __FILE__, __LINE__);
    cmos_assert(NULL != buf, __FILE__, __LINE__);
    cmos_assert(0 < n_bytes, __FILE__, __LINE__);

    cmos_int32_T n_reads = 0;

    n_reads = cmos_fd_read((cmos_fd_fcb_T *)fd, buf, n_bytes);

    return n_reads;
}

/*******************************************************************************
 *
 * 函数名  : cmos_ioctl_p
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_ioctl 特权
 *
 * 输入参数: fd      文件句柄
 *           buf     操作类型
 *           n_bytes 由操作类型决定
 *
 * 输出参数: 无
 *
 * 返回值  : 实际写入字节数
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_ioctl_p(cmos_fd_T fd, cmos_uint32_T request, ...)
{ 
    cmos_assert(0 != fd, __FILE__, __LINE__);

    cmos_status_T status = cmos_ERR_E;
    cmos_uint32_T mode = 0;
    va_list args;

    va_start(args, request);
    mode = va_arg(args, cmos_uint32_T);
    va_end(args);

    /* 返回的是指针 */
    status = cmos_fd_ioctl((cmos_fd_fcb_T *)fd, request, mode);
    return status;
}

