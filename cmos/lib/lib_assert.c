/******************************************************************************
 *
 * 文件名  ： lib_assert.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150614 
 * 版本号  ： 1.0
 * 文件描述： 实现无处可放的函数
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "lib.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : assert_failed
* 负责人  : 彭鹏
* 创建日期: 20150614
* 函数功能: 断言失败调用
*
* 输入参数: file 出错源文件名字
* 输入参数: line 出错源文件行数
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void assert_failed(cmos_uint8_T *file, cmos_int32_T line)
{
    /* FIXME: 串口初始化完成前无法打印 */
    cmos_err_log("%s,%d: assert failed!", file, line);
    while(1);
}

