/******************************************************************************
 *
 * 文件名  ： write.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151224 
 * 版本号  ： 1.0
 * 文件描述： write系统调用
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "misc.h"
#include "fd.h"
#include "write.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void cmos_write_before(cmos_fd_fcb_T *fcb, const void *buf, cmos_int32_T n_bytes);
static void cmos_write_after(cmos_fd_fcb_T *fd, const void *buf, cmos_int32_T n_bytes);

/* syscall.s中定义 */
cmos_int32_T svc_write(cmos_fd_fcb_T *fd, const void *buf, cmos_int32_T n_bytes);

/********************************** 函数实现区 *********************************/
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
 * 函数名  : cmos_write_p
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

    return cmos_fd_write(fcb, buf, n_bytes);
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
     
     cmos_fd_mutex_T *mutex_lock = cmos_fd_fcb_get_lock(fcb); 
     cmos_fd_mutex_lock(mutex_lock); 
     /* 执行到此表示已经成功锁定 可以正常写 */ 

     return;
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
    return;
}

