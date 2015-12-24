/******************************************************************************
 *
 * 文件名  ： syscall.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： 1.1
 * 文件描述： 系统调用总控
 * 版权说明： Copyright (c) GNU
 * 其    他： 系统调用按照分类组织
 *            例如文件相关系统调用在syscall_fd模块
 *            系统调用包含3部分:
 *            1. svc前用户态        实现类似锁定(需要用户态执行的逻辑),参数检查等
 *            2. svc陷入内核执行    实现特权级别才可以实现的功能
 *            2. svc后用户态        实现类似系统调用嵌套的功能
 *
 *            _before svc前用户态代码
 *            _svc    特权代码
 *            _after  svc后用户态代码
 *            syscall.s 中完成svc指令
 *
 *            本文件实现不同的系统调用的判断(switch表格)
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "misc.h"

#include "syscall.h"
#include "syscall_kernel.h"
#include "syscall_fd.h"
#include "syscall_ipc.h"
#include "syscall_task.h"

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
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void syscall_c(cmos_uint32_T *sp)
{
    cmos_assert(NULL != sp, __FILE__, __LINE__);

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
     *        0x11 延迟
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
                cmos_init_svc();
                break;
            }
        case 0x01:
            { 
                cmos_start_svc();
                break;
            }
        case 0x02:
            { 
                sp[0] = cmos_status_svc();
                break;
            }

        /* 任务控制 */
        case 0x10:
            { 
                cmos_create_svc((cmos_task_id_T *)stacked_r0, (const cmos_task_attribute_T *)stacked_r1);
                break;
            }
        case 0x11:
            { 
                cmos_delay_svc((cmos_int32_T)stacked_r0);
                break;
            }

        /* 任务通信与同步 */
        case 0x30:
            {
                cmos_ipc_svc((cmos_ipc_type_T)stacked_r0, (void *)stacked_r1);
                break;
            }

        /* 驱动系统调用(利用Linux VFS思想) */
        case 0xa0:
            { 
                sp[0] = (cmos_uint32_T)cmos_open_svc((const cmos_int8_T *)stacked_r0, 
                        (cmos_uint32_T)stacked_r1,
                        stacked_r2); 
                break;
            }
        case 0xa1:
            {
                cmos_close_svc((cmos_fd_T)stacked_r0); 
                break;
            }
        case 0xa2:
            {
                sp[0] = cmos_read_svc((cmos_fd_T)stacked_r0, (void *)stacked_r1, (cmos_int32_T)stacked_r2);
                break;
            }
        case 0xa3:
            {
                sp[0] = cmos_write_svc((cmos_fd_T)stacked_r0, (void *)stacked_r1, (cmos_int32_T)stacked_r2);
                break;
            }
        case 0xa4:
            {
                cmos_ioctl_svc((cmos_fd_T)stacked_r0, (cmos_uint32_T)stacked_r1, stacked_r2);
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

