/********************************************************************************
*
* 文件名  ： mpu9250.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： mpu9250驱动头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _MPU9250_H_
#define _MPU9250_H_

/************************************ 头文件 ***********************************/
#include "cmos_api.h"

/************************************ 宏定义 ***********************************/
/*********************************** 板级定义 **********************************/
#define MPU9250_SPEED                       (100000)/* 100k */
#define MPU9250_I2C_INDEX                   (3)     /* stm32f429 dis 板i2c3 */

/*********************************** MPU9250 ***********************************/
#define MPU9250_DEV_ID                      (0x71)  /* MPU9250 id */
#define MPU9250_DEV_ADDR                    (0x68)  /* 7'b1101000 */
#define MPU9250_SELF_TEST_XG_REG            (0x00)
#define MPU9250_SELF_TEST_YG_REG            (0x01)
#define MPU9250_SELF_TEST_ZG_REG            (0x02)
#define MPU9250_SELF_TEST_XA_REG            (0x0D)
#define MPU9250_SELF_TEST_YA_REG            (0x0E)
#define MPU9250_SELF_TEST_ZA_REG            (0x0F)
#define MPU9250_XG_OFFSET_H_REG             (0x13)
#define MPU9250_XG_OFFSET_L_REG             (0x14)
#define MPU9250_YG_OFFSET_H_REG             (0x15)
#define MPU9250_YG_OFFSET_L_REG             (0x16)
#define MPU9250_ZG_OFFSET_H_REG             (0x17)
#define MPU9250_ZG_OFFSET_L_REG             (0x18)
#define MPU9250_SMPLRT_DIV_REG              (0x19)
#define MPU9250_CONFIG_REG                  (0x1A)
#define MPU9250_GYRO_CONFIG_REG             (0x1B)
#define MPU9250_ACCEL_CONFIG_REG            (0x1C)
#define MPU9250_ACCEL_CONFIG_2_REG          (0x1D)
#define MPU9250_LP_ACCEL_ODR_REG            (0x1E)
#define MPU9250_MOT_THR_REG                 (0x1F)
#define MPU9250_FIFO_EN_REG                 (0x23)
#define MPU9250_I2C_MST_CTRL_REG            (0x24)
#define MPU9250_I2C_SLV0_ADDR_REG           (0x25)
#define MPU9250_I2C_SLV0_REG_REG            (0x26)
#define MPU9250_I2C_SLV0_CTRL_REG           (0x27)
#define MPU9250_I2C_SLV1_ADDR_REG           (0x28)
#define MPU9250_I2C_SLV1_REG_REG            (0x29)
#define MPU9250_I2C_SLV1_CTRL_REG           (0x2A)
#define MPU9250_I2C_SLV2_ADDR_REG           (0x2B)
#define MPU9250_I2C_SLV2_REG_REG            (0x2C)
#define MPU9250_I2C_SLV2_CTRL_REG           (0x2D)
#define MPU9250_I2C_SLV3_ADDR_REG           (0x2E)
#define MPU9250_I2C_SLV3_REG_REG            (0x2F)
#define MPU9250_I2C_SLV3_CTRL_REG           (0x30)
#define MPU9250_I2C_SLV4_ADDR_REG           (0x31)
#define MPU9250_I2C_SLV4_REG_REG            (0x32)
#define MPU9250_I2C_SLV4_DO_REG             (0x33)
#define MPU9250_I2C_SLV4_CTRL_REG           (0x34)
#define MPU9250_I2C_SLV4_DI_REG             (0x35)
#define MPU9250_I2C_MST_STATUS_REG          (0x36)
#define MPU9250_INT_PIN_CFG_REG             (0x37)
#define MPU9250_INT_ENABLE_REG              (0x38)
#define MPU9250_INT_STATUS_REG              (0x3A)
#define MPU9250_ACCEL_XOUT_H_REG            (0x3B)
#define MPU9250_ACCEL_XOUT_L_REG            (0x3C)
#define MPU9250_ACCEL_YOUT_H_REG            (0x3D)
#define MPU9250_ACCEL_YOUT_L_REG            (0x3E)
#define MPU9250_ACCEL_ZOUT_H_REG            (0x3F)
#define MPU9250_ACCEL_ZOUT_L_REG            (0x40)
#define MPU9250_TEMP_OUT_H_REG              (0x41)
#define MPU9250_TEMP_OUT_L_REG              (0x42)
#define MPU9250_GYRO_XOUT_H_REG             (0x43)
#define MPU9250_GYRO_XOUT_L_REG             (0x44)
#define MPU9250_GYRO_YOUT_H_REG             (0x45)
#define MPU9250_GYRO_YOUT_L_REG             (0x46)
#define MPU9250_GYRO_ZOUT_H_REG             (0x47)
#define MPU9250_GYRO_ZOUT_L_REG             (0x48)
#define MPU9250_EXT_SENS_DATA_00_REG        (0x49)
#define MPU9250_EXT_SENS_DATA_01_REG        (0x4A)
#define MPU9250_EXT_SENS_DATA_02_REG        (0x4B)
#define MPU9250_EXT_SENS_DATA_03_REG        (0x4C)
#define MPU9250_EXT_SENS_DATA_04_REG        (0x4D)
#define MPU9250_EXT_SENS_DATA_05_REG        (0x4E)
#define MPU9250_EXT_SENS_DATA_06_REG        (0x4F)
#define MPU9250_EXT_SENS_DATA_07_REG        (0x50)
#define MPU9250_EXT_SENS_DATA_08_REG        (0x51)
#define MPU9250_EXT_SENS_DATA_09_REG        (0x52)
#define MPU9250_EXT_SENS_DATA_10_REG        (0x53)
#define MPU9250_EXT_SENS_DATA_11_REG        (0x54)
#define MPU9250_EXT_SENS_DATA_12_REG        (0x55)
#define MPU9250_EXT_SENS_DATA_13_REG        (0x56)
#define MPU9250_EXT_SENS_DATA_14_REG        (0x57)
#define MPU9250_EXT_SENS_DATA_15_REG        (0x58)
#define MPU9250_EXT_SENS_DATA_16_REG        (0x59)
#define MPU9250_EXT_SENS_DATA_17_REG        (0x5A)
#define MPU9250_EXT_SENS_DATA_18_REG        (0x5B)
#define MPU9250_EXT_SENS_DATA_19_REG        (0x5C)
#define MPU9250_EXT_SENS_DATA_20_REG        (0x5D)
#define MPU9250_EXT_SENS_DATA_21_REG        (0x5E)
#define MPU9250_EXT_SENS_DATA_22_REG        (0x5F)
#define MPU9250_EXT_SENS_DATA_23_REG        (0x60)
#define MPU9250_I2C_SLV0_DO_REG             (0x63)
#define MPU9250_I2C_SLV1_DO_REG             (0x64)
#define MPU9250_I2C_SLV2_DO_REG             (0x65)
#define MPU9250_I2C_SLV3_DO_REG             (0x66)
#define MPU9250_I2C_MST_DELAY_CTRL_REG      (0x67)
#define MPU9250_SIGNAL_PATH_RESET_REG       (0x68)
#define MPU9250_MOT_DETECT_CTRL_REG         (0x69)
#define MPU9250_USER_CTRL_REG               (0x6A)
#define MPU9250_PWR_MGMT_1_REG              (0x6B)
#define MPU9250_PWR_MGMT_2_REG              (0x6C)
#define MPU9250_FIFO_COUNTH_REG             (0x72)
#define MPU9250_FIFO_COUNTL_REG             (0x73)
#define MPU9250_FIFO_R_W_REG                (0x74)
#define MPU9250_WHO_AM_I_REG                (0x75)  /* ID = 0x71 In MPU9250 */
#define MPU9250_XA_OFFSET_H_REG             (0x77)
#define MPU9250_XA_OFFSET_L_REG             (0x78)
#define MPU9250_YA_OFFSET_H_REG             (0x7A)
#define MPU9250_YA_OFFSET_L_REG             (0x7B)
#define MPU9250_ZA_OFFSET_H_REG             (0x7D)
#define MPU9250_ZA_OFFSET_L_REG             (0x7E)
/************************************ AK8963 ***********************************/
#define AK8963_I2C_ADDR                     (0x0C)  /* 7'b0001100 */
#define AK8963_DEV_ID                       (0x48)
/* Read-only Reg */
#define AK8963_WIA_REG                      (0x00)
#define AK8963_INFO_REG                     (0x01)
#define AK8963_ST1_REG                      (0x02)
#define AK8963_HXL_REG                      (0x03)
#define AK8963_HXH_REG                      (0x04)
#define AK8963_HYL_REG                      (0x05)
#define AK8963_HYH_REG                      (0x06)
#define AK8963_HZL_REG                      (0x07)
#define AK8963_HZH_REG                      (0x08)
#define AK8963_ST2_REG                      (0x09)
/* Write/Read Reg */
#define AK8963_CNTL1_REG                    (0x0A)
#define AK8963_CNTL2_REG                    (0x0B)
#define AK8963_ASTC_REG                     (0x0C)
#define AK8963_TS1_REG                      (0x0D)
#define AK8963_TS2_REG                      (0x0E)
#define AK8963_I2CDIS_REG                   (0x0F)
/* Read-only Reg ( ROM ) */
#define AK8963_ASAX_REG                     (0x10)
#define AK8963_ASAY_REG                     (0x11)
#define AK8963_ASAZ_REG                     (0x12)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
typedef struct accel_tag{
    unsigned short x;
    unsigned short y;
    unsigned short z;
}accel_T;

typedef struct gyro_tag{
    unsigned short x;
    unsigned short y;
    unsigned short z;
}gyro_T;

typedef struct mag_tag{
    unsigned short x;
    unsigned short y;
    unsigned short z;
}mag_T;

void mpu9250_init(void);
void mpu9250_read_accel(accel_T *ptr_accel);
void mpu9250_read_gyro(gyro_T *ptr_gyro);
void mpu9250_read_mag(mag_T *ptr_mag);
void mpu9250_read_tem(unsigned short *ptr_temp);

#endif /* _MPU9250_H_ */

