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
#include "cmos_config.h"
#include "cmos_api.h"
#include "hal.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
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
* 其 它   : 无
*
******************************************************************************/
cmos_status_T cmos_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    /* cmos hal 初始化 */
    status = hal_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    /* 后面的初始化可以使用控制台输出了 */
    return cmos_OK_E;
}

inline void cmos_delay_ms(cmos_uint32_T ms)
{
    delay_ms(ms);
}

inline cmos_uint32_T cmos_get_ms(void)
{
    return get_ms();
}

