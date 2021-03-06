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
#include "misc.h"

/************************************ 宏定义 ***********************************/
/* 控制台输出 */
#define cmos_printf(...)        cmos_console_printf(__VA_ARGS__)

/* 系统调用cmos_open参数宏 */
#define CMOS_O_RDWR             (0x00000000)

/* 系统调用cmos_ioctl参数宏 */
#define CMOS_I_SETBAUDRATE      (0x00000000)
#define CMOS_I_SET_I2C_ADDR     (0x00000001)
#define CMOS_I_SET_POLL         (0x00000002)
#define CMOS_I_SET_IT           (0x00000003)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 类型定义 **********************************/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
/* 内核基本 */
void cmos_init(void);
void cmos_start(void);
cmos_status_T cmos_status(void);

/* 任务管理 */
cmos_status_T cmos_create(cmos_task_id_T *task_id, const cmos_task_attribute_T *task_attribute);

/* 时间管理 */
cmos_status_T cmos_delay(cmos_int32_T ms);

/* 任务通信与同步 所有的ipc共用一个系统调用 */
void cmos_ipc(cmos_ipc_type_T type, void *para);
#define cmos_interrupt_enable()     do{cmos_ipc(cmos_ipc_interrupt_enable, NULL);}while(0);
#define cmos_interrupt_disable()    cmos_ipc(cmos_ipc_interrupt_disable, NULL)
#define cmos_switch_enable()        cmos_ipc(cmos_ipc_interrupt_enable, NULL)
#define cmos_switch_disable()       cmos_ipc(cmos_ipc_interrupt_disable, NULL)
#define cmos_mutex_lock(mutex)        cmos_ipc(cmos_ipc_mutex_lock, mutex)
#define cmos_mutex_unlock(mutex)       cmos_ipc(cmos_ipc_mutex_unlock, mutex)

/* 外设驱动 */
cmos_fd_T cmos_open(const cmos_int8_T *path, cmos_uint32_T flag, ...);
cmos_int32_T cmos_read(cmos_fd_T dev_id, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_read_poll(cmos_fd_T dev_id, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write(cmos_fd_T dev_id, const void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write_poll(cmos_fd_T dev_id, const void *buf, cmos_int32_T n_bytes);
void cmos_ioctl(cmos_fd_T dev_id, cmos_uint32_T request, ...);
void cmos_close(cmos_fd_T dev_id);
cmos_int32_T cmos_read_poll(cmos_fd_T dev_id, void *buf, cmos_int32_T n_bytes);
cmos_int32_T cmos_write_poll(cmos_fd_T dev_id, const void *buf, cmos_int32_T n_bytes);

#endif /* _CMOS_API_H_ */

