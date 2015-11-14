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

/* 测试使用 */
cmos_uint32_T cmos_i2c_read_buf(cmos_uint8_T dev_addr, cmos_uint16_T reg_addr,
        cmos_uint8_T *ptr_read_buf, cmos_uint32_T buf_len);


/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

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

    status = cmos_start();
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
    }

    cmos_printf("cmos start.\r\n");

#if 0
    unsigned char val = 0;
    int i = 0;
    int iMax = 0;
    int read_num = 0;
    unsigned char bmp180_reg_addr[] = {
        0xAA, 0xAB,
        0xAC, 0xAD,
        0xAE, 0xAF,
        0xBA, 0xBB,
        0xBC, 0xBD,
        0xBE, 0xBF,
        0xD0, 0xE0, 0xF4, 0xF6, 0xF7, 0xF8};

    cmos_printf("BMP180 REGS Values:\r\n");
    iMax = sizeof(bmp180_reg_addr) /sizeof(bmp180_reg_addr[0]);
    for(i=0;i<iMax;i++) 
    { 
        read_num = cmos_i2c_read_buf(0xEF, bmp180_reg_addr[i], &val, 1);
        if(read_num != 1)
        {
            assert_failed(__FILE__, __LINE__);
        }
        cmos_printf("0x%02x:0x%02x\r\n", bmp180_reg_addr[i], val);
    }
    while(TRUE);
#else 

    cmos_i2c_addr_T bmp180_addr =
    {
        .dev_addr = 0x00,
        .reg_offset = 0x00
    };
    cmos_int32_T read_bytes = 0;
    cmos_uint8_T buf[10];

    cmos_int32_T s_bmp180_fd = cmos_open(CMOS_IMU_I2CBUS_PATH, CMOS_O_RDWR);
    status = cmos_ioctl(s_bmp180_fd, CMOS_I_SET_I2C_ADDR, &bmp180_addr);
    read_bytes = cmos_read(s_bmp180_fd, buf, 10); 
    
    /* 写入待测试 */
    /* static cmos_int32_T i2c_write(const void *dev_id, const void *buf, cmos_int32_T n_bytes) */


#endif
}

