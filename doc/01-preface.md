# CMOS内核--序言
本文介绍一些CMOS中需要用的基础知识。由于在单片机系统中不会有MMU所以单片机系统中的每个任务就是一个线程，共用系统的地址空间，为了精确性，后文中措辞中使用线程替换任务，例如任务切换叫做线程切换。

## RTOS应用开发
使用实时内核的时候会写多个线程，每个线程交替执行，由于线程切换的频率一般比较快，所以看起来就是多个线程并发执行。每个线程都是一个死循环，用流程图表示会是这个样子：

```flow
st=>start: 线程创建
e=>end: 结束
op1=>operation: 线程初始化
op2=>operation: 实际工作
cond=>condition: 出错？

st->op1->op2->cond
cond(yes)->e
cond(no)->op2
```
多个线程共存时系统会是这个样子：
```sequence
命令行-->屏幕: 显示一下当前速度.
屏幕-->AD采样: 命令行要我显示速度，采样一下
AD采样-->屏幕: 等下！
屏幕-->AD采样: 搞快点啊，命令行不耐烦，要喷我了!
AD采样-->屏幕: 采到了，现在速度10m/s.
屏幕-->命令行:现在速度10m/s
```
上图中,命令行、屏幕、AD采样是三个线程，在处理器上并行执行，互不干扰。
用C语言边写会是这个样子:
### 代码块
代码块语法遵循标准markdown代码，例如：
``` c
#include "os.h"

void job1(void *argv)
{
    job1_init();
    while(1)
    {
        do_job1();
    }
}

void job2(void *argv)
{
    job2_init();
    while(1)
    {
        do_job2();
    }
}

int main()
{
    osInit();
    osCreateThread(job1, job1_priority, job1_stack_size);
    osCreateThread(job2, job2_priority, job2_stack_size);
    osStart();              /* 启动调度 */

    /* 不可达 */
    return;
}
```
