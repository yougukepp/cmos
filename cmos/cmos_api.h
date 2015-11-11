/********************************************************************************
*
* 文件名  ： cmos_api.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： cmos api头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_API_H_
#define _CMOS_API_H_

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "console.h"
#include "lib.h"

/************************************ 宏定义 ***********************************/
/* 控制台输出 */
#define cmos_printf(...)        cmos_console_printf(__VA_ARGS__);

/* 系统调用cmos_open参数宏 */
#define CMOS_O_RDWR             (0x00000000)
/* 系统调用cmos_ioctl参数宏 */
#define CMOS_I_SETBAUDRATE      (0x00000000)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* 内核基本 */
cmos_status_T cmos_init(void);
cmos_status_T cmos_start(void);

/* 任务管理 */
cmos_status_T cmos_task_create(cmos_task_id_T *task_id, 
        cmos_func_T task_func, 
        void *argv, 
        const cmos_task_para_T *task_para);

/* 外设驱动 */
cmos_int32_T cmos_open(const cmos_uint8_T *path, cmos_uint32_T flag, ...);
cmos_int32_T cmos_read(cmos_int32_T dev_id, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write(cmos_int32_T dev_id, const void *buf, cmos_int32_T n_bytes);
cmos_status_T cmos_ioctl(cmos_int32_T dev_id, cmos_uint32_T request, ...);
cmos_status_T cmos_close(cmos_int32_T dev_id);

#endif /* _CMOS_API_H_ */

