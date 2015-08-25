/******************************************************************************
 *
 * 文件名  ： port.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150825 
 * 版本号  ： v1.0
 * 文件描述： 抽象底层系统
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/

/*----------------------------------- 声明区 ----------------------------------*/
#include "pc.h"
#include "port.h"

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : get_ms
 * 负责人  : 彭鹏
 * 创建日期: 20150824
 * 函数功能: 获取当前进程的ms值
 *
 * 输入参数: 无
 * 输出参数: ms    毫秒值
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int get_ms(unsigned long *ms)
{
#ifdef X86_64
    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    /* FIXME: 溢出 */
    *ms = timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
#else
#endif
    return 0;
}

int delay_ms(unsigned long ms)
{
#ifdef X86_64
#else
#endif
    return 0;
}

thread_id_T thread_create(thread_loop_T loop, void *argv)
{
#ifdef X86_64
    int rst = 0;
    thread_id_T tid; 
    rst = pthread_create(&tid, NULL, loop, argv);
    if(0 == rst)
    { 
        return 0;
    }
    return tid;
#else
#endif
}

inline int mutex_init(MUTEX_T *mutex)
{
#ifdef X86_64
    return pthread_mutex_init(mutex, NULL);
#else
#endif
}

inline int mutex_lock(MUTEX_T *mutex)
{
#ifdef X86_64
    return pthread_mutex_lock(mutex);
#else
#endif
}

inline int mutex_unlock(MUTEX_T *mutex)
{
#ifdef X86_64
    return pthread_mutex_unlock(mutex);
#else
#endif
}

int get_gyro(float *gyro)
{
#ifdef X86_64
    return pc_get_gyro(gyro);
#else
#endif
}

