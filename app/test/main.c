/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150321 
 * 版本号  ： 1.1
 * 文件描述： CMOS测试桩主文件
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 日期     操作人   操作
 *            20150321 pengpeng 创建
 *            20150614 pengpeng 修改为测试桩
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "cmos_api.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void task1(void *argv);
static void task2(void *argv);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: CMOS测试桩主函数
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

#if 1
    cmos_task_id_T task1_id;
    /* 创建idle任务 使用cmos_create系统调用 */
    cmos_task_attribute_T task1_attribute =
    {
        .priority = cmos_priority_low,
        .stack_size = 4096,
        .tick_total = 10,
        .flag = cmos_task_with_default
    };
    cmos_int32_T argv1 = 100;
    status = cmos_create(&task1_id, task1, &argv1, &task1_attribute); 
    cmos_printf("task1 create %d:0x%08x.\r\n", status, (cmos_int32_T)task1_id);
#endif

    status = cmos_start();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }


#if 0
    cmos_i2c_addr_T bmp180_addr =
    {
        .dev_addr = 0xEF,
        .reg_offset = 0xF4
    };
    cmos_int32_T read_bytes = 0;
    cmos_int32_T write_bytes = 0;
    cmos_uint8_T buf[6] = {0};

    cmos_int32_T s_bmp180_fd = cmos_open(CMOS_IMU_I2CBUS_PATH, CMOS_O_RDWR);

    /* 读取测试 */
    status = cmos_ioctl(s_bmp180_fd, CMOS_I_SET_I2C_ADDR, &bmp180_addr);
    if(cmos_OK_E != status)
    {
        cmos_printf("cmos_ioctl err 0x%08x", status);
        goto out;
    }
    read_bytes = cmos_read(s_bmp180_fd, buf, 6); 

    cmos_printf("cmos read bmp180 %d:\r\n", read_bytes);
    for(int i = 0; i< 6; i++)
    {
        cmos_printf("0x%02x:0x%02x\r\n", 0xF4 + i, buf[i]);
    }



    /* 写入测试 */
    buf[0] = 0xC0;
    write_bytes = cmos_write(s_bmp180_fd, buf, 1); 
    cmos_printf("cmos write bmp180 %d:\r\n", write_bytes);

    /* 回读 */
    read_bytes = cmos_read(s_bmp180_fd, buf, 6); 

    cmos_printf("cmos read bmp180 %d:\r\n", read_bytes);
    for(int i = 0; i< 6; i++)
    {
        cmos_printf("0x%02x:0x%02x\r\n", 0xF4 + i, buf[i]);
    }

out:
#endif

    while(TRUE);
}

static void task1(void *argv)
{
    cmos_int32_T val = 0;
    val = *((cmos_int32_T *)argv);

#if 0

    /* 新建任务2 */
    cmos_status_T status = cmos_ERR_E;
    cmos_task_id_T task2_id;
    cmos_task_attribute_T task2_attribute =
    {
        .priority = cmos_priority_low,
        .stack_size = 2048,
        .tick_total = 3,
        .flag = cmos_task_with_float
    };
    float argv2 = 200.0f;
    status = cmos_create(&task2_id, task2, &argv2, &task2_attribute); 
    cmos_printf("task2 create %d:0x%08x.\r\n", status, (cmos_int32_T)task2_id);
#endif


    while(TRUE)
    {
        cmos_printf("task1: %d\r\n", val);
    }
}

static void task2(void *argv)
{
    float val = 0;
    val = *((float *)argv);

    while(TRUE)
    {
        cmos_printf("task2: %.2f", val);
    }
}

