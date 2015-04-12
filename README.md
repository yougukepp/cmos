# cmos说明文档
cmos将作为一个符合CMSIS标准的系统,他是Cortex-M Operating System的简称.本文档暂时用于描述开发计划.

## 目录结构

## 计划
1. 实现动态内存管理
2. 自己初始化内存不依赖与keil运行时
3. cmos下一步实现调度算法
2.1 位图算法
2.2 TCB链表实现
4. 实现延迟
5. Idle任务可以统计系统信息 检查任务堆栈 之后休眠处理器
6. 清理代码格式 不用tab等 清理C++相关#ifdef 完善注释
7. 清理对HAL层的调用 直接调用寄存器级别 工程仅依赖于 CMSIS BSP目录

## 注意:
1. 任务切换只可以发生在 系统调用期间
2. CUBE4 在 E:\Keil\_v5\STM32Cube\_FW\_F4
3. HAL层对RTOS的支持有风险 inc/stm32f4xx\_hal\_conf.h USE\_RTOS
4. 参考FreeRtos实现
5. 文档的编写使用[md编辑器][1]

---------

[1]: http://write.blog.csdn.net/mdeditor


