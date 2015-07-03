/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150629 
 * 版本号  ： 1.0
 * 文件描述： MPU9250传感器 I2C读取
 * 版权说明： Copyright (c) GNU
 * 其    他： 无 
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"
#include "mpu9250.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: MPU9250 测试主函数
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 主程序永不返回
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
int main(void)
{ 
    cmos_status_T status = cmos_ERR_E;
    status = cmos_init();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    mpu9250_init();

    while(1);
}

