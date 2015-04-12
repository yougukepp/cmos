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
#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

/************************************ 头文件 ***********************************/
#include "cmsis_os.h"

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

/*********************************** 类型定义 **********************************/
typedef unsigned char       cm_uint8_t;
typedef unsigned short      cm_uint16_t;
typedef unsigned int        cm_uint32_t;
typedef unsigned long long  cm_uint64_t;
typedef char                cm_int8_t;
typedef short               cm_int16_t;
typedef int                 cm_int32_t;
typedef long long           cm_int64_t;
typedef unsigned char       cm_bool_t;

/* 函数入口地址 */
typedef void                (*cm_func_t)(void);


/* 函数状态 通常用于返回值 */
typedef osStatus            cm_status_t;

/* 线程id号 */
typedef osThreadId          cm_thread_id_t;
/* 线程定义 */
typedef osThreadDef_t       cm_thread_def_t;
/* 线程入口地址 */
typedef os_pthread          cm_pthread_t;

/* 线程控制块 */
typedef struct os_thread_cb
{
    os_pthread pthread;
    void       *argv;

    cm_uint32_t *psp;
    osPriority priority;
    cm_uint32_t stack_size;
}cm_tcb_t;

/*
 * 空闲内存块链表 
 * 内存块 大小固定为 sizeof(cm_tcb_t)
 */
#define CM_TCB_SIZE (5)
//const cm_uint32_t CM_TCB_SIZE = sizeof(cm_tcb_t);
typedef struct cm_mem_block_tag
{
    cm_uint32_t data[CM_TCB_SIZE];
    struct cm_mem_block_tag *next;
}cm_mem_block_t;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* __TYPE_DEF_H__ */




