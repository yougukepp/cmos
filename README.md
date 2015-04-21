# cmos说明文档
cmos将作为一个符合CMSIS标准的系统,他是Cortex-M Operating System的简称.本文档暂时用于描述开发计划.

## 目录结构

## 计划
0. 系统调用加用户空间运行部分
1. 参数尽可能使用const 所有函数参数检查 尽量使用inline汇编 整理TODO 完善注释汇编代码分离 完善各函数接口
2. Idle任务可以 检查任务堆栈 之后WFI处理器
3. TCB链表操作使用双链表
4. 实现CMSIS OS要求的系统调用
5. 清理对HAL层的调用 直接调用寄存器级别 工程仅依赖于 CMSIS BSP目录
6. 实现文件系统 类似Linux一切皆文件的框架
7. 实现网络协议栈(TCP/IP)

## 注意:
1. 任务切换只会发生在 系统调用期间
2. CUBE4 在 E:\Keil\_v5\STM32Cube\_FW\_F4
3. HAL层对RTOS的支持有风险 inc/stm32f4xx\_hal\_conf.h USE\_RTOS
4. 参考FreeRtos RT-Thread实现
5. 文档的编写使用[md编辑器][1]

---------

[1]: http://write.blog.csdn.net/mdeditor

