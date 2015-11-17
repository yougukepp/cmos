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
#include "vfs.h"
#include "hal.h"
#include "cortex.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_status_T cmos_start_c(void);
static cmos_status_T cmos_task_create_c(cmos_task_id_T *task_id, 
        cmos_func_T task_func,
        void *argv,
        const cmos_task_para_T *task_para);
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

    cmos_uint32_T stacked_r0 = 0;
    cmos_uint32_T stacked_r1 = 0;
    cmos_uint32_T stacked_r2 = 0;
    cmos_uint32_T stacked_r3 = 0;

    cmos_uint8_T svc_number = 0;

    svc_number = ((cmos_uint8_T *) sp[6])[-2];
    stacked_r0 = sp[0];
    stacked_r1 = sp[1];
    stacked_r2 = sp[2];
    stacked_r3 = sp[3];

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
     *        0x10 cmos_task_create
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
                /* cmos_init 没有汇编部分 */
                assert_failed(__FILE__, __LINE__);
                break;
            }
        case 0x01:
            { 
                sp[0] = cmos_start_c(); 
                break;
            }

        /* 任务控制 */
        case 0x10:
            { 
                sp[0] = cmos_task_create_c((cmos_task_id_T *)stacked_r0,
                        (cmos_func_T)stacked_r1, 
                        (void *)stacked_r1, 
                        (const cmos_task_para_T *)stacked_r3);
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
 * 函数名  : cmos_start_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 内核启动
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T cmos_start_c(void)
{
    /* 开始调度 */
    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_create_c
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 创建任务
 *
 * 输入参数: task_func      任务入口
 *           argv           任务参数
 *           task_attribute 任务属性 堆栈 优先级 等
 *
 * 输出参数: task_id 任务id号
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T cmos_task_create_c(cmos_task_id_T *task_id, 
        cmos_func_T task_func,
        void *argv,
        const cmos_task_para_T *task_attribute)
{
    if((NULL == task_func)
    || (NULL == task_attribute))
    {
        CMOS_ERR_STR("task func and task attribute should not to be null.");
    }

    return cmos_OK_E;
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
    fd = vfs_open(path, flag, mode);

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

    return cmos_OK_E;
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

    return 0;
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

    n_writes = vfs_write(fd, buf, n_bytes);

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
        CMOS_ERR_STR("write fd < 0.");
        return cmos_PARA_E;
    }

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : cmos_init
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
* 其 它   : 无需汇编实现
*
******************************************************************************/
cmos_status_T cmos_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* cmos hal vfs初始化 */
    status = vfs_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }


    /* cmos hal 硬件底层初始化 */
    status = hal_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
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

    /* TODO:创建idle任务 */

    /* 进入任务环境 */
    GOTO_TASK_CONTEXT;

    return cmos_OK_E;
}

