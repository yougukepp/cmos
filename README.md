# cmos说明文档
cmos将作为一个符合CMSIS标准的系统,他是Cortex-M Operating System的简称.本文档暂时用于描述开发计划.

## 目录结构
待施工

## 计划
### debug\(重构\)计划
1. 实现CMOS HAL测试框架
2. 实现CMOS UART HAL驱动
3. 实现控制台模块 用于调试
4. 实现CMOS USB HAL驱动
5. 实现近期计划

### 近期计划
1. 按照结构图修改目录结构
2. 系统调用的所有代码整理入syscall.s syscall.c syscall.h 三个文件
3. 实现驱动层堆STM HAL层封装
4. 实现串口命令行
5. 修改main中的demo直接调用cmos\(后续准备实现飞行姿态控制\)
6. 系统调用加用户空间运行部分
7. 实现CMSIS OS要求SignalManagement

### 远期计划
1. 整理TODO
2. Idle任务可以 检查任务堆栈 CPU占用率 之后WFI处理器
3. 实现文件系统 类似Linux一切皆文件的框架
4. 实现网络协议栈(TCP/IP)
5. TCB链表操作使用双链表
6. 清理对HAL层的调用 直接调用寄存器级别 工程仅依赖于 CMSIS BSP目录
7. 所有函数参数检查
8. 参数尽可能使用const

## 注意:
1. 任务切换只会发生在 系统调用期间
2. CUBE4 在 E:\Keil\_v5\STM32Cube\_FW\_F4
3. HAL层对RTOS的支持有风险 inc/stm32f4xx\_hal\_conf.h USE\_RTOS
4. 参考FreeRtos RT-Thread实现
5. 文档的编写使用[md编辑器][1]

---------

[1]: http://write.blog.csdn.net/mdeditor

