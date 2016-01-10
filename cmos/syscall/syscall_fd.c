/******************************************************************************
 *
 * 文件名  ： syscall_fd.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： 文件相关系统调用
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

#include "syscall.h"
#include "misc.h"
#include "kernel.h"
#include "fd.h"
#include "syscall_fd.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void cmos_open_before(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
static void cmos_open_after(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);

static void cmos_close_before(cmos_fd_fcb_T *fcb);
static void cmos_close_after(cmos_fd_fcb_T *fcb);

static void cmos_read_before(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes);
static void cmos_read_after(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes);

static void cmos_write_before(cmos_fd_fcb_T *fcb, const void *buf, cmos_int32_T n_bytes);
static void cmos_write_after(cmos_fd_fcb_T *fd, const void *buf, cmos_int32_T n_bytes);

static void cmos_ioctl_before(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para);
static void cmos_ioctl_after(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para);

/* syscall.s中定义 */
cmos_fd_fcb_T *svc_open(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
void svc_close(cmos_fd_fcb_T *fcb);
cmos_int32_T svc_write(cmos_fd_fcb_T *fcb, const void *buf, cmos_int32_T n_bytes);
cmos_int32_T svc_read(cmos_fd_fcb_T *fcb, const void *buf, cmos_int32_T n_bytes);
void svc_ioctl(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para);

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
cmos_fd_T cmos_open_svc(const cmos_int8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
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
}

/*******************************************************************************
 *
 * 函数名  : cmos_write
 * 负责人  : 彭鹏
 * 创建日期：20151223 
 * 函数功能: 系统调用cmos_write
 *
 * 输入参数: fd      文件句柄(文件控制块)
 *           buf     写入数据的缓存
 *           n_bytes 要求写入的字节数
 *
 * 输出参数: 无
 * 返回值  : 实际写入字节数
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_int32_T cmos_write(cmos_fd_T fd, const void *buf, cmos_int32_T n_bytes)
{ 
    cmos_int32_T rst = 0;

    cmos_write_before((cmos_fd_fcb_T *)fd, buf, n_bytes);

    rst = svc_write((cmos_fd_fcb_T *)fd, buf, n_bytes); 

    cmos_write_after((cmos_fd_fcb_T *)fd, buf, n_bytes);

    return rst;
}

/*******************************************************************************
 *
 * 函数名  : cmos_write_svc
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_write特权级代码
 *
 * 输入参数: fd      文件句柄
 *           buf     写入数据的缓存
 *           n_bytes 要求写入的字节数
 *
 * 输出参数: 无
 * 返回值  : 实际写入字节数
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_int32_T cmos_write_svc(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes)
{
    cmos_assert(0 != fcb, __FILE__, __LINE__);
    cmos_assert(NULL != buf, __FILE__, __LINE__);
    cmos_assert(0 < n_bytes, __FILE__, __LINE__);

    cmos_int32_T rst = cmos_fd_write(fcb, buf, n_bytes);

    return rst;
}

/*******************************************************************************
 *
 * 函数名  : cmos_write_before
 * 负责人  : 彭鹏
 * 创建日期：20151221 
 * 函数功能: 系统调用cmos_write陷入特权级别之前的操作
 *
 * 输入参数: 与cmos_write一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_write_before(cmos_fd_fcb_T *fcb, const void *buf, cmos_int32_T n_bytes)
{
     cmos_assert(NULL != fcb, __FILE__, __LINE__); 
     
     cmos_fd_mutex_T *mutex = cmos_fd_fcb_get_lock(fcb); 
     cmos_status_T status = cmos_kernel_status();
     
     /* 初始化 不锁定 */
     if(cmos_INIT_E == status)
     {
         return;
     }
     /* 空闲任务自旋锁 */
     else if(cmos_IDLE_E == status)
     {
         cmos_mutex_lock_spin(mutex);
     }
     /* 正常多任务阻塞锁 */
     else if(cmos_MULT_E == status)
     {
         cmos_mutex_lock(mutex);
     }

     /* 执行到此表示已经成功锁定 可以正常写 */ 
}

/*******************************************************************************
 *
 * 函数名  : cmos_write_after
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_write返回用户级别之后的操作
 *
 * 输入参数: 与cmos_write一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_write_after(cmos_fd_fcb_T *fd, const void *buf, cmos_int32_T n_bytes)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_close
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_close
 *
 * 输入参数: fcb 文件控制块
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_close(cmos_fd_fcb_T *fcb)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__);

    cmos_close_before(fcb);
    svc_close(fcb);
    cmos_close_after(fcb);
}

/*******************************************************************************
 *
 * 函数名  : cmos_close_svc
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_close特权代码
 *
 * 输入参数: fcb 文件控制块
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_close_svc(cmos_fd_fcb_T *fcb)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__);
    cmos_fd_close(fcb);

}

/*******************************************************************************
 *
 * 函数名  : cmos_close_before
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_close特权代码之前的用户态代码
 *
 * 输入参数: fcb 文件控制块
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_close_before(cmos_fd_fcb_T *fcb)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_close_after
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_close特权代码之后的用户态代码
 *
 * 输入参数: fcb 文件控制块
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_close_after(cmos_fd_fcb_T *fcb)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_read
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_read
 *
 * 输入参数: fcb     文件句柄
 *           buf     读取数据的缓存
 *           n_bytes 要求读取的字节数
 *
 * 输出参数: 无
 * 返回值  : 实际读取字节数
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_int32_T cmos_read(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes)
{
    cmos_int32_T read_bytes = 0;
    cmos_assert(NULL != fcb, __FILE__, __LINE__);

    cmos_read_before(fcb, buf, n_bytes);
    read_bytes = svc_read(fcb, buf, n_bytes);
    cmos_read_after(fcb, buf, n_bytes);

    return read_bytes;
}

/*******************************************************************************
 *
 * 函数名  : cmos_read_svc
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_read特权代码
 *
 * 输入参数: 与cmos_read一致
 * 输出参数: 无
 * 返回值  : 实际读取字节数
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_int32_T cmos_read_svc(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__);
    cmos_int32_T rst = cmos_fd_read(fcb, buf, n_bytes);


    return rst;
}

/*******************************************************************************
 *
 * 函数名  : cmos_read_before
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_read特权代码之前用户态代码
 *
 * 输入参数: 与cmos_read一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_read_before(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_read_after
 * 负责人  : 彭鹏
 * 创建日期：20151023 
 * 函数功能: 系统调用cmos_read特权代码之后用户态代码
 *
 * 输入参数: 与cmos_read一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_read_after(cmos_fd_fcb_T *fcb, void *buf, cmos_int32_T n_bytes)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_ioctl
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ioctl
 *
 * 输入参数: fcb     文件句柄
 *           request 操作类型
 *           para    由操作类型决定
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_ioctl(cmos_fd_fcb_T *fcb, cmos_uint32_T request, ...)
{ 
    cmos_assert(NULL != fcb, __FILE__, __LINE__);

    cmos_uint32_T para = 0;
    va_list args;

    va_start(args, request);
    para = va_arg(args, cmos_uint32_T);
    va_end(args);

    cmos_ioctl_before(fcb, request, para);
    svc_ioctl(fcb, request, para);
    cmos_ioctl_after(fcb, request, para);
}

/*******************************************************************************
 *
 * 函数名  : cmos_ioctl
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ioctl特权代码
 *
 * 输入参数: 与cmos_ioctl一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_ioctl_svc(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para)
{
    cmos_assert(NULL != fcb, __FILE__, __LINE__);
    cmos_fd_ioctl(fcb, request, para);

}

/*******************************************************************************
 *
 * 函数名  : cmos_ioctl_before
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ioctl特权代码之前用户态代码
 *
 * 输入参数: 与cmos_ioctl一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_ioctl_before(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para)
{
}

/*******************************************************************************
 *
 * 函数名  : cmos_ioctl_after
 * 负责人  : 彭鹏
 * 创建日期：20151224 
 * 函数功能: 系统调用cmos_ioctl特权代码之后用户态代码
 *
 * 输入参数: 与cmos_ioctl一致
 * 输出参数: 无
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_ioctl_after(cmos_fd_fcb_T *fcb, cmos_uint32_T request, cmos_uint32_T para)
{
}
