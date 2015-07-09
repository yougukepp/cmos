/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150629 
 * 版本号  ： 1.0
 * 文件描述： MPU9250传感器 I2C读取
 * 版权说明： Copyright (c) GNU
 * 其    他： TODO:研究清楚算法
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
/* 消除中文打印警告 */
#pragma  diag_suppress 870

/************************************ 头文件 ***********************************/
#include <math.h>
#include "cmos_config.h"
#include "cmos_api.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

/*----------------------------------- 声明区 ----------------------------------*/
#define MAIN_DIM        (3)
#define MAIN_YAW        (0)
#define MAIN_PITCH      (1)
#define MAIN_ROLL       (2)
#define MAIN_PI         (3.1415926)

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static int init(void);
static void update(unsigned long *time, float *temper, float *ypr, float *accel, float *gyro, float *compass);
static void compute_yaw_pitch_roll(float *ypr, long *quat);
static void run_self_test(void);

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
    unsigned long time = 0;         /* 时间 */
    float temper = 0;               /* 温度 */
    float ypr[MAIN_DIM] = {0};      /* yaw pitch roll */
    float accel[MAIN_DIM] = {0};    /* 加速度 x y z*/
    float gyro[MAIN_DIM] = {0};     /* 陀螺仪 x y z*/
    float compass[MAIN_DIM] = {0};  /* 磁力计 x y z*/

    int times = 0;                  /* 用于控制打印 */

    init();
    do{
        update(&time, &temper, ypr, accel, gyro, compass);
        if( (0 != time)
         && (times >= 0) ) /* 控制打印 */
        {
            cmos_printf("attitude(%5.2fs,%5.2fC) :%5.2f,%5.2f,%5.2f.\r\n",
                    time / 1000.0, temper, ypr[MAIN_YAW], ypr[MAIN_PITCH], ypr[MAIN_ROLL]);
            cmos_printf("accel                   :%5.2f,%5.2f,%5.2f.\r\n", accel[0], accel[1], accel[2]);
            cmos_printf("gyro                    :%5.2f,%5.2f,%5.2f.\r\n", gyro[0], gyro[1], gyro[2]);
            cmos_printf("compass                 :%5.2f,%5.2f,%5.2f.\r\n", compass[0], compass[1], compass[2]);
            cmos_printf("\r\n");
            times = 0;
        }
        times++;

        mpu9250_delay_ms(5);
    }while(TRUE);
}

/*******************************************************************************
*
* 函数名  : init
* 负责人  : 彭鹏
* 创建日期: 20150707
* 函数功能: 主初始化
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返回值  : 0       成功
*           其他    失败
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static int init(void)
{
    unsigned char dev_status = 0;

    short accel[MAIN_DIM] = {0};    /* 加速度 x y z*/
    short gyro[MAIN_DIM] = {0};     /* 陀螺仪 x y z*/
    long quat[4] = {0};             /* 四元数 */
    unsigned long time = 0;         /* 时间 */
    short sensors = 0;              /* FIFO中数据的掩码 */
    unsigned char fifo_count = 0;   /* FIFO中数据字节数 */
    unsigned char rst = 0;          /* 函数返回值 */

    /* 系统初始化 */
    cmos_init();
    cmos_i2c_init(MPU9250_I2C_INDEX, MPU9250_SPEED);

    /* invensence 初始化 */
    cmos_printf("初始化MPU...\r\n");
    if (mpu_init(NULL) != 0)
    {
        cmos_printf("初始化MPU失败!\r\n");
        return -1;
    }
    cmos_printf("打开传感器...\r\n");
    if (mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL|INV_XYZ_COMPASS)!=0)
    {
        cmos_printf("打开传感器失败.\r\n");
        return -1;
    }
    cmos_printf("设置陀螺仪量程...\r\n");
    if (mpu_set_gyro_fsr(2000)!=0)
    {
        cmos_printf("设置陀螺仪量程失败.\r\n");
        return -1;
    }
    cmos_printf("设置加速度计量程...\r\n");
    if (mpu_set_accel_fsr(2)!=0)
    {
        cmos_printf("设置加速度计量程失败.\r\n");
        return -1;
    }
    cmos_printf("MPU上电...\r\n");
    mpu_get_power_state(&dev_status);
    cmos_printf("MPU9250 上电%s", dev_status? "成功!\r\n" : "失败!\r\n");
    cmos_printf("设置MPU FIFO...\r\n");
    if (mpu_configure_fifo(INV_XYZ_GYRO|INV_XYZ_ACCEL)!=0)
    {
        cmos_printf("设置MPU FIFO失败.\r\n");
        return -1;
    }
    cmos_printf("下载DMP固件...\r\n");
    if (dmp_load_motion_driver_firmware()!=0)
    {
        cmos_printf("下载DMP固件失败.\r\n");
        return -1;
    }
    cmos_printf("启动DMP...\r\n");
    if (mpu_set_dmp_state(1)!=0) 
    {
        cmos_printf("启动DMP失败r\n");
        return -1;
    }
    cmos_printf("使能DMP...\r\n");
    if(0 != dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT
                | DMP_FEATURE_SEND_RAW_ACCEL
                | DMP_FEATURE_SEND_CAL_GYRO
                | DMP_FEATURE_GYRO_CAL)) 
    {
        cmos_printf("使能DMP失败.\r\n");
        return -1;
    } 
    cmos_printf("设置DMP FIFO速率...\r\n");
    if (dmp_set_fifo_rate(MPU9250_DMP_FIFO_RATE)!=0)
    {
        cmos_printf("设置DMP FIFO速率失败.\r\n");
        return -1;
    }
    cmos_printf("复位FIFO队列...\r\n");
    if (mpu_reset_fifo()!=0)
    {
        cmos_printf("复位FIFO队列失败.\r\n");
        return -1;
    }

    /* 加入MPU自检 */
    cmos_printf("自检...\r\n"); 
    run_self_test();
    do
    {
        mpu9250_delay_ms(1000/MPU9250_DMP_FIFO_RATE);  /* dmp will habve 4 (5-1) packets based on the fifo_rate */
        rst = dmp_read_fifo(gyro, accel, quat, &time, &sensors, &fifo_count);
    } while (0 != rst || fifo_count < 5); /* packtets!!! */
    cmos_printf("初始化完成.\r\n");

    return 0;
}

/*******************************************************************************
*
* 函数名  : update
* 负责人  : 彭鹏
* 创建日期: 20150707
* 函数功能: 更新数据
*
* 输入参数: 无
*
* 输出参数: (unsigned long *time, float *temper, float *ypr, float *accel, float *gyro, float *compass)
*
* 返回值  : 0       成功
*           其他    失败
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void update(unsigned long *time, float *temper, float *ypr, float *accel, float *gyro, float *compass)
{
    short accel_i[MAIN_DIM] = {0};  /* 加速度 x y z*/
    short gyro_i[MAIN_DIM] = {0};   /* 陀螺仪 x y z*/
    long  quat[4] = {0};            /* 四元数 */
    short sensors = 0;              /* FIFO中数据的掩码 */
    unsigned char fifo_count = 0;   /* FIFO中数据字节数 */
    unsigned char rst = 0;          /* 函数返回值 */

    long temper_i = 0;               /* 温度 */
    short compass_i[MAIN_DIM] = {0};/* 磁力计 x y z*/ 
    
    rst = dmp_read_fifo(gyro_i, accel_i, quat, time, &sensors, &fifo_count); 
    if(0 != rst)
    {
        /* cmos_printf("dmp_read_fifo 失败.\r\n"); */
        *time = 0;
        *temper = 0;
        ypr[0] = 0;
        ypr[1] = 0;
        ypr[2] = 0;
        accel[0] = 0;
        accel[1] = 0;
        accel[2] = 0;
        gyro[0] = 0;
        gyro[1] = 0;
        gyro[2] = 0;
        compass[0] = 0;
        compass[1] = 0;
        compass[2] = 0;
        return;
    }
    
    compute_yaw_pitch_roll(ypr, quat);
    
    /* 温度 */
    mpu_get_temperature(&temper_i, NULL);
    *temper = (float)temper_i/65536L; 
    
    /* 磁场 */
    mpu_get_compass_reg(compass_i, NULL); 
    

    /* 转换为角度 */
    for(int i=0;i<MAIN_DIM;i++)
    {
        ypr[i] *= (180 / MAIN_PI);
    }
		
 #if 0  
    ypr[0] = wrap_180(ypr[0]);
#endif 

    /* MPU与标准pitch定义反向 */
    ypr[1]*=-1.0;

    /* x 0, y 1, z 2
     * 转换 MPU xyz 与yaw pitch roll一致 */
    for (int i=0;i<MAIN_DIM;i++)
    {
        gyro[i]   = (float)(gyro_i[MAIN_DIM-i-1]);
        accel[i]   = (float)(accel_i[MAIN_DIM-i-1]);
        compass[i] = (float)(compass_i[MAIN_DIM-i-1]);
    }

}

/* TODO:补充注释 */
static void compute_yaw_pitch_roll(float *ypr, long *quat)
{
    /* 四元数 */
    float w = 0;
    float x = 0;
    float y = 0;
    float z = 0;

    float gravity[MAIN_DIM] = {0}; /* 重力场 */

    w = (float)quat[0] / 16384.0f;
    x = (float)quat[1] / 16384.0f;
    y = (float)quat[2] / 16384.0f;
    z = (float)quat[3] / 16384.0f;

    gravity[0] = 2 * (x*z - w*y);
    gravity[1] = 2 * (w*x + y*z);
    gravity[2] = w*w - x*x - y*y + z*z; 
    
    /* yaw:   (Z 轴) */
    ypr[0] = atan2(2*x*y - 2*w*z, 2*w*w + 2*x*x - 1);
    /* pitch: (y 轴) */
    ypr[1] = atan(gravity[0]/ sqrt(gravity[1]*gravity[1] + gravity[2]*gravity[2]));
    /* roll:  (X 轴) */
    ypr[2] = atan(gravity[1]/ sqrt(gravity[0]*gravity[0] + gravity[2]*gravity[2]));
}

static void run_self_test(void)
{
    int result = 0;
    long gyro[3] = {0};
    long accel[3] = {0};

    result = mpu_run_6500_self_test(gyro, accel, 0);
    if (result == 0x7)
    {
        cmos_printf("通过!\r\n");
        cmos_printf("accel: %7.4f %7.4f %7.4f\r\n",
                accel[0]/65536.f,
                accel[1]/65536.f,
                accel[2]/65536.f);
        cmos_printf("gyro: %7.4f %7.4f %7.4f\n",
                gyro[0]/65536.f,
                gyro[1]/65536.f,
                gyro[2]/65536.f);

        /* 自检测试通过 我们需要更新校准数据 与offset寄存器 */
        unsigned char i = 0;
        for(i = 0; i<3; i++)
        {
            gyro[i] = (long)(gyro[i] * 32.8f); /* 运用量程 +-1000dps */
            accel[i] *= 2048.f; /* 运用量程convert to +-16G */
            accel[i] = accel[i] >> 16;
            gyro[i] = (long)(gyro[i] >> 16);
        }

        mpu_set_gyro_bias_reg(gyro);
        mpu_set_accel_bias_6500_reg(accel);
    }
    else
    {
        if (!(result & 0x1))
        {
            cmos_printf("Gyro failed.\r\n");
        }
        if (!(result & 0x2))
        {
            cmos_printf("Accel failed.\r\n");
        }
        if (!(result & 0x4))
        {
            cmos_printf("Compass failed.\r\n");
        }
    }

    return;
}


