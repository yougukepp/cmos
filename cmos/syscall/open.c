/******************************************************************************
 *
 * 文件名  ： open.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： open系统调用
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>

#include "cmos_config.h"
#include "misc.h"
#include "open.h"
#include "fd.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void cmos_open_before(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
static void cmos_open_after(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);

/* syscall.s中定义 */
cmos_int32_T svc_open(const cmos_int8_T *path, cmos_uint32_T flag, ...);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_open
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_open
 *
 * 输入参数: path vfs的路径
 *           flag 调用标记
 *           ...  第三个参数由flag决定
 *
 * 输出参数: 无
 * 返回值  : NULL   出错
 *           其他   fcb指针
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_fd_T cmos_open(const cmos_int8_T *path, cmos_uint32_T flag, ...)
{ 
    cmos_assert(NULL != path, __FILE__, __LINE__);

    cmos_fd_T fd = 0;
    cmos_uint32_T mode = 0;

    va_list args;

    va_start(args, flag);
    mode = va_arg(args, cmos_uint32_T);
    va_end(args);

    cmos_open_before(path, flag, mode);

    fd = svc_open(path, flag, mode);
    cmos_assert(0 != fd, __FILE__, __LINE__);

    cmos_open_after(path, flag, mode);

    return fd;
}

/*******************************************************************************
 *
 * 函数名  : cmos_open_svc
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_open 特权
 *
 * 输入参数: path vfs的路径
 *           flag 调用标记
 *           mode 打开模式
 *
 * 输出参数: 无
 * 返回值  : NULL   出错
 *           其他   fcb指针
 *          
 * 调用关系: 无
 * 其 它   : TODO: 完成 实际功能
 *
 ******************************************************************************/
cmos_fd_fcb_T *cmos_open_svc(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{ 
    cmos_assert(NULL != path, __FILE__, __LINE__);

    cmos_fd_fcb_T *fcb = NULL;

    /* 返回的是指针 */
    fcb = cmos_fd_open(path, flag, mode);

    return fcb;
}

/*******************************************************************************
 *
 * 函数名  : cmos_open_before
 * 负责人  : 彭鹏
 * 创建日期：20151221 
 * 函数功能: 系统调用cmos_open陷入特权级别之前的操作
 *
 * 输入参数: 与cmos_open一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_open_before(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
     return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_open_after
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_open_after返回用户级别之后的操作
 *
 * 输入参数: 与cmos_open一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_open_after(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
    return;
}

