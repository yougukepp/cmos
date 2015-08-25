/******************************************************************************
 *
 * 文件名  ： port.h
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150825 
 * 版本号  ： v1.0
 * 文件描述： 底层适配层头文件
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PORT_H_
#define _PORT_H_

/************************************ 头文件 ***********************************/
#ifdef X86_64
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#else
#endif

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
#ifdef X86_64
typedef pthread_mutex_t MUTEX_T; /* 锁类型 */
typedef pthread_t thread_id_T; /* 线程标识符 */
typedef void *(*thread_loop_T)(void *argv);
#else
#endif

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
int get_ms(unsigned long *ms);
int delay_ms(unsigned long ms);
int mutex_init(MUTEX_T *mutex);
int mutex_lock(MUTEX_T *mutex);
int mutex_unlock(MUTEX_T *mutex);
thread_id_T thread_create(thread_loop_T loop, void *argv);
int get_gyro(float *gyro);

#endif // #ifndef _PORT_H_

