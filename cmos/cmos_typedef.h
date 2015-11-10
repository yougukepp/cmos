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

  cmos_BUF_SMALL_E      =  0x7FFFFFF5, /* 缓冲过小 */
  cmos_END_E            =  0x7FFFFFF6, /* 结束 */
  cmos_PARA_E           =  0x7FFFFFF7, /* 参数错误 */
  cmos_NULL_E           =  0x7FFFFFF8, /* 空指针 */
  cmos_MEM_LACK_E       =  0x7FFFFFF9, /* 内存不足 无法malloc */
  cmos_CONSOLE_E        =  0x7FFFFFFA, /* 串口错误 */
  cmos_DEV_TIMEOUT_E    =  0x7FFFFFFB,
  cmos_DEV_BUSY_E       =  0x7FFFFFFC,
  cmos_DEV_ERR_E        =  0x7FFFFFFD,
  cmos_ERR_E            =  0x7FFFFFFE,
  cmos_RESERVED_E       =  0x7FFFFFFF
}cmos_status_T;

/* 任务优先级 */
typedef enum{
    cmos_priority_idle = 0x00,
    cmos_priority_low = 0x01,
    cmos_priority_below_normal = 0x02,
    cmos_priority_normal = 0x04,
    cmos_priority_above_normal = 0x08,
    cmos_priority_high = 0x10,
    cmos_priority_realtime = 0x20,
    cmos_priority_err = 0xff
}cmos_priority_T;

/* 任务启动参数 */
typedef struct{
    cmos_priority_T   priority;   /* 优先级 */
    cmos_uint32_T     stack_size; /* 任务栈大小 */
    cmos_uint32_T     time_slice; /* 同一优先级有多个线程 该线程的运行时间 SysTick为单位 */
}cmos_task_para_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_TYPE_DEF_H_ */

