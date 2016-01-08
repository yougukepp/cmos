/********************************************************************************
*
* 文件名  ： tyepdef.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150321
* 版本号  ： v1.0
* 文件描述： 通用类型定义文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TYPE_DEF_H_
#define _CMOS_TYPE_DEF_H_

/************************************ 头文件 ***********************************/
#include <stdint.h>

/************************************ 宏定义 ***********************************/
/* bool 宏 */
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

/* 空字符 */
#ifndef NUL
#define NUL ('\0')
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

/* 设备初始化的时候连接使用
 * 例如USART1 <= CMOS_DEV_INSTNAME(USART, 1)
 * */
#define CMOS_DEV_INSTNAME(dev_name, index) dev_name##index


/*********************************** 类型定义 **********************************/
typedef unsigned char       cmos_uint8_T;
typedef unsigned short      cmos_uint16_T;
typedef unsigned int        cmos_uint32_T;
typedef unsigned long long  cmos_uint64_T;
typedef char                cmos_int8_T;
typedef short               cmos_int16_T;
typedef int                 cmos_int32_T;
typedef long long           cmos_int64_T;
typedef unsigned char       cmos_bool_T;
typedef void(*cmos_func_T)(void *argv);

/* CMOS函数状态返回 */
typedef enum{
  cmos_OK_E             =  0x00000000, /* int32正数 最高位为 0 */
  cmos_MULT_E           =  0x7FFFFFF8, /* 多任务运行中 */
  cmos_IDLE_E           =  0x7FFFFFF9, /* 空闲中 */
  cmos_INIT_E           =  0x7FFFFFFA, /* 单任务 */
  cmos_NULL_E           =  0x7FFFFFFB, /* 空指针 */
  cmos_PARA_E           =  0x7FFFFFFC, /* 参数错误 */
  cmos_MEM_LACK_E       =  0x7FFFFFFD, /* 内存不足 无法malloc */
  cmos_ERR_E            =  0x7FFFFFFE,
  cmos_RESERVED_E       =  0x7FFFFFFF
}cmos_status_T;

/* 任务控制 */
/* 任务号 */
typedef struct cmos_task_tcb_T_tag cmos_task_tcb_T;
typedef cmos_task_tcb_T *cmos_task_id_T;
/* 任务优先级 */
typedef enum{
    cmos_priority_idle = 0x01,
    cmos_priority_low = 0x02,
    cmos_priority_below_normal = 0x04,
    cmos_priority_normal = 0x08,
    cmos_priority_above_normal = 0x10,
    cmos_priority_high = 0x20,
    cmos_priority_realtime = 0x40,
    cmos_priority_err = 0xff
}cmos_priority_T;
/* 任务flag属性 */
typedef enum{
    cmos_task_with_default = 0x00,
    cmos_task_with_float = 0x01,
    cmos_task_with_privileged = 0x02,
}cmos_tasK_flag_T;
/* 任务启动参数 */
typedef struct{
    cmos_func_T       entry;
    void              *argv;
    cmos_priority_T   priority;   /* 优先级 */
    cmos_int32_T      stack_size; /* 任务栈大小 */
    cmos_int32_T      tick_total; /* 同一优先级有多个线程 该线程的运行时间 SysTick为单位 */
    cmos_int32_T      flag;       /* 存放任务的参数:浮点 特权 */
}cmos_task_attribute_T;

/* 文件句柄 */
typedef struct cmos_fd_fcb_T_tag cmos_fd_fcb_T;
typedef cmos_fd_fcb_T *cmos_fd_T;

/* IPC类型 */
typedef enum{
    cmos_ipc_interrupt_enable = 0x00,
    cmos_ipc_interrupt_disable = 0x01,
    cmos_ipc_switch_enable = 0x02,
    cmos_ipc_switch_disable = 0x03,
    cmos_ipc_mutex_lock = 0x04,
    cmos_ipc_mutex_unlock = 0x05,
    cmos_ipc_mutex_lock_spin = 0x06,
    cmos_ipc_mutex_unlock_spin = 0x07,
    cmos_ipc_err = 0xff
}cmos_ipc_type_T;

/* i2c 相关 */
/* TODO: 想办法移入驱动 */
typedef struct cmos_i2c_addr_T_tag{
    cmos_uint8_T dev_addr;
    cmos_uint8_T reg_offset;
}cmos_i2c_addr_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_TYPE_DEF_H_ */

