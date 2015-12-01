/******************************************************************************
 *
 * 文件名  ： api.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： 1.1
 * 文件描述： 封装CMOS内部各模块 对应用层提供统一接口
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>

#include "cmos_config.h"
#include "cmos_api.h"

#include "cortex.h"
#include "console.h"
#include "kernel.h"
#include "fd.h"
#include "task.h"
#include "switch.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
cmos_status_T cmos_init_c(void);
static cmos_status_T cmos_start_c(void);

static cmos_status_T cmos_create_c(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute);

static cmos_status_T cmos_delay_c(cmos_int32_T millisec);

static void cmos_enable_interrupt_c(void);
static void cmos_disable_interrupt_c(void);
static void cmos_enable_switch_c(void);
static void cmos_disable_switch_c(void);

static cmos_int32_T cmos_open_c(const cmos_uint8_T *path, cmos_uint32_T flag, ...);
static cmos_status_T cmos_close_c(cmos_int32_T fd);
static cmos_int32_T cmos_read_c(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes);
static cmos_int32_T cmos_write_c(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes);
static cmos_status_T cmos_ioctl_c(cmos_int32_T fd, cmos_uint32_T request, ...);

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
     *        0x00 cmos_init 没有汇编部分
     *        0x01 cmos_start 没有汇编部分
     *      0x1 任务控制(参考CMSIS Thread Management)
     *        0x10 cmos_create
     *      0x2 时间管理(参考CMSIS Thread Management)
     *        0x20 cmos_delay
     *      0x3 任务通信与同步
     *        最强IPC 对实时性(中断延迟)影响巨大 数条指令使用
     *        0x30 开中断     
     *        0x31 关中断
     *        任务同步IPC 对ISR无效 任务同步使用
     *        0x32 开调度器锁
     *        0x33 关调度器锁
     *      0xa 驱动系统调用(利用Linux VFS思想)
     *        0xa0 cmos_open
     *        0xa1 cmos_close
     *        0xa2 cmos_read
     *        0xa3 cmos_write
     *        0xa4 cmos_ioctl
     *
     **************************************************************************/
    switch(svc_number)
    {
        /* 内核基本 */
        case 0x00:
            {
                CMOS_ERR_STR("syscall_c should not used by svc:0x00 asm.");
                break;
            }
        case 0x01:
            { 
                cmos_start_c();
                break;
            }

        /* 任务控制 */
        case 0x10:
            { 
                sp[0] = cmos_create_c((cmos_task_id_T *)stacked_r0, (const cmos_task_attribute_T *)stacked_r1);
                break;
            }

        /* 时间管理 */
        case 0x20:
            { 
                sp[0] = cmos_delay_c((cmos_int32_T)stacked_r0);
                break;
            }

        /* 任务通信与同步 */
        case 0x30:
            {
                cmos_enable_interrupt_c();
                break;
            }

        case 0x31:
            {
                cmos_disable_interrupt_c();
                break;
            }

        case 0x32:
            {
                cmos_enable_switch_c();
                break;
            }

        case 0x33:
            {
                cmos_disable_switch_c();
                break;
            }


        /* 驱动系统调用(利用Linux VFS思想) */
        case 0xa0:
            { 
                sp[0] = cmos_open_c((const cmos_uint8_T *)stacked_r0, (cmos_uint32_T)stacked_r1, stacked_r2); 
                break;
            }
        case 0xa1:
            {
                sp[0] = cmos_close_c((cmos_uint32_T)stacked_r0); 
                break;
            }
        case 0xa2:
            {
                sp[0] = cmos_read_c((cmos_int32_T)stacked_r0, (void *)stacked_r1, (cmos_int32_T)stacked_r2);
                break;
            }
        case 0xa3:
            {
                sp[0] = cmos_write_c((cmos_int32_T)stacked_r0, (void *)stacked_r1, (cmos_int32_T)stacked_r2);
                break;
            }
        case 0xa4:
            {
                sp[0] = cmos_ioctl_c((cmos_int32_T)stacked_r0, (cmos_uint32_T)stacked_r1, stacked_r2);
                break;
            }
        default:
            {
                assert_failed(__FILE__, __LINE__);
                break;
            }
    }

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_init_c
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 内核初始化
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 汇编会调用 不加static
 *
 ******************************************************************************/
inline cmos_status_T cmos_init_c(void)
{
   return cmos_kernel_init();
}


/*******************************************************************************
 *
 * 函数名  : cmos_start_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 多任务启动
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_status_T cmos_start_c(void)
{
    return cmos_kernel_start();
}

/*******************************************************************************
 *
 * 函数名  : cmos_create_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 创建任务
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
static cmos_status_T cmos_create_c(cmos_task_id_T *task_id, 
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
 * 函数名  : cmos_delay_c
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: 延迟当前任务
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
inline static cmos_status_T cmos_delay_c(cmos_int32_T millisec)
{
    return cmos_task_switch_delay(millisec);
}

/*******************************************************************************
 *
 * 函数名  : cmos_enable_interrupt_c
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 开中断
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_enable_interrupt_c(void)
{
    cmos_hal_cortex_cortex_disable_interrupt();
}

/*******************************************************************************
 *
 * 函数名  : cmos_disable_interrupt_c
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 关中断
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_disable_interrupt_c(void)
{
    cmos_hal_cortex_cortex_enable_interrupt();
}

/*******************************************************************************
 *
 * 函数名  : cmos_enable_switch_c
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 开调度器锁
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_enable_switch_c(void)
{
    cmos_hal_cortex_cortex_enable_switch();
}

/*******************************************************************************
 *
 * 函数名  : cmos_disable_switch_c
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 关调度器锁
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_disable_switch_c(void)
{
    cmos_hal_cortex_cortex_disalbe_switch();
}

/*******************************************************************************
 *
 * 函数名  : cmos_open_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_open的C语言主逻辑
 *
 * 输入参数: path vfs的路径
 *           flag 调用标记
 *           ...  第三个参数由flag决定
 *
 * 输出参数: 无
 * 返回值  : -1     出错
 *           其他   文件id从0开始
 *          
 * 调用关系: 无
 * 其 它   : TODO: 完成 实际功能
 *
 ******************************************************************************/
static cmos_int32_T cmos_open_c(const cmos_uint8_T *path, cmos_uint32_T flag, ...)
{ 
    if(NULL == path)
    {
        CMOS_ERR_STR("open path is null.");
        return -1;
    }

    cmos_int32_T fd = 0;
    cmos_uint32_T mode = 0;

    va_list args;

    va_start(args, flag);
    mode = va_arg(args, cmos_uint32_T);
    va_end(args);

    /* 返回的是指针 */
    fd = syscall_fd_open(path, flag, mode);

    return fd;
}

/*******************************************************************************
 *
 * 函数名  : cmos_close_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_close的C语言主逻辑
 *
 * 输入参数: 
 *           fd      文件句柄
 *
 * 输出参数: 无
 *
 * 返回值  : 实际写入字节数
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T cmos_close_c(cmos_int32_T fd)
{
    if(fd < 0)
    {
        CMOS_ERR_STR("close fd < 0.");
        return cmos_PARA_E;
    }

    return syscall_fd_close(fd);
}

/*******************************************************************************
 *
 * 函数名  : cmos_read_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_read的C语言主逻辑
 *
 * 输入参数: 
 *           fd      文件句柄
 *           buf     读取数据的缓存
 *           n_bytes 要求读取的字节数
 *
 * 输出参数: 无
 *
 * 返回值  : 实际读取字节数
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_int32_T cmos_read_c(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes)
{
    if(fd < 0)
    {
        CMOS_ERR_STR("read fd < 0.");
        return cmos_PARA_E;
    }

    if((NULL == buf)
    || (n_bytes <= 0))
    {
        CMOS_ERR_STR("read buf err.");
        return cmos_PARA_E;
    }

    cmos_int32_T n_reads = 0;

    n_reads = syscall_fd_read(fd, buf, n_bytes);

    return n_reads;
}

/*******************************************************************************
 *
 * 函数名  : cmos_write_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_write的C语言主逻辑
 *
 * 输入参数: 
 *           fd      文件句柄
 *           buf     写入数据的缓存
 *           n_bytes 要求写入的字节数
 *
 * 输出参数: 无
 *
 * 返回值  : 实际写入字节数
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_int32_T cmos_write_c(cmos_int32_T fd, void *buf, cmos_int32_T n_bytes)
{
    if(fd < 0)
    {
        CMOS_ERR_STR("write fd < 0.");
        return cmos_PARA_E;
    }

    if((NULL == buf)
    || (n_bytes <= 0))
    {
        CMOS_ERR_STR("write buf err.");
        return cmos_PARA_E;
    }

    cmos_int32_T n_writes = 0;

    n_writes = syscall_fd_write(fd, buf, n_bytes);

    return n_writes;
}

/*******************************************************************************
 *
 * 函数名  : cmos_ioctl_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_ioctl的C语言主逻辑
 *
 * 输入参数: 
 *           fd      文件句柄
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
static cmos_status_T cmos_ioctl_c(cmos_int32_T fd, cmos_uint32_T request, ...)
{ 
    if(fd < 0)
    {
        CMOS_ERR_STR("ioctl fd < 0.");
        return cmos_PARA_E;
    }

    cmos_status_T status = cmos_ERR_E;
    cmos_uint32_T mode = 0;
    va_list args;

    va_start(args, request);
    mode = va_arg(args, cmos_uint32_T);
    va_end(args);

    /* 返回的是指针 */
    status = syscall_fd_ioctl(fd, request, mode);
    return status;
}

