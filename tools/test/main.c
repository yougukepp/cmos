/******************************************************************************
 *
 * 文件名  ： 
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： 
 * 文件描述： 
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmos_config.h"
#include "pc.h"
#include "tree.h"
#include "vfs.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : main
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 主函数
 *
 * 输入参数: argc - 参数个数
 *           argv - 命令行参数数组
 *
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int main(int argc, char *argv[])
{ 
    cmos_status_T status = cmos_OK_E;
    cmos_trace_log("start.");
    status = vfs_init();
    if(cmos_OK_E != status)
    {
        cmos_trace_log("status:0x%08X.", status);
        assert_failed(__FILE__, __LINE__);
    } 

    vfs_print();


#if 0

    /* 加入i2c设备结点*/
    status = vfs_node_add((const cmos_uint8_T *)CMOS_VFS_DEV_DIR, (const cmos_uint8_T *)("i2c"), vfs_dev, NULL);
    if(cmos_OK_E != status)
    {
        return status;
    }
#endif

    cmos_trace_log("done.");

    return 0;
}

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : assert_failed
* 负责人  : 彭鹏
* 创建日期: 20151030
* 函数功能: 断言失败调用
*
* 输入参数: file 出错源文件名字
*           line 出错源文件行数
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void assert_failed(char *file, cmos_int32_T line)
{
    printf("%s,%d: assert failed!\n", file, line);
    fflush(stderr);
    fflush(stdout);
    exit(0);
}

/*******************************************************************************
*
* 函数名  : cmos_console_printf
* 负责人  : 彭鹏
* 创建日期: 20151030
* 函数功能: 控制台打印 模式cmos输出行为
*
* 输入参数: 与printf参数含义一致
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 系统错误或调试打印需要第一时间输出 采用轮询方式输出
*
******************************************************************************/
cmos_int32_T cmos_console_printf(char *fmt, ...)
{ 
    char printf_buf[1024];
    va_list args;
    cmos_int32_T n = 0;
   
    va_start(args, fmt);
    n = vsprintf(printf_buf, fmt, args);
    va_end(args);

    printf(printf_buf);
    fflush(stdout);

    return n;
}

/*******************************************************************************
*
* 函数名  : cmos_malloc
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: 动态分配内存接口
*
* 输入参数: size 欲分配的字节数
*
* 输出参数: 无
*
* 返回值  : 分配的内存首地址
*
* 调用关系: 无
* 其 它   : 自己实现
*
******************************************************************************/
void *cmos_malloc(cmos_int32_T size)
{
    return malloc(size);
}

/*******************************************************************************
*
* 函数名  : cmos_free
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: 动态释放内存接口
*
* 输入参数: ptr 欲释放的地址
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 自己实现
*
******************************************************************************/
void cmos_free(void *ptr)
{
    free(ptr);
}

