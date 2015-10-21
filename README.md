# cmos说明文档
cmos将作为一个符合CMSIS标准的系统,他是Cortex-M Operating System的简称.本文档暂时用于描述开发计划.

## 目录结构
待施工

## 计划
### 近期计划
0. 实现app/flyer/doc/README.md计划
1. 按照结构图修改目录结构
2. 系统调用的所有代码整理入syscall.s syscall.c syscall.h 三个文件
3. 实现驱动层堆STM HAL层封装,参考串口\I2C\RCC时钟模块的CMSIS驱动模型,RCC时钟模块
4. 实现串口命令行,参考CMSIS驱动模型
5. 系统调用加用户空间运行部分
6. 实现CMSIS OS要求SignalManagement

### 远期计划
1. 整理TODO
2. 所有函数参数检查
3. 参数尽可能使用const inline
4. TCB链表操作使用双链表
5. HAL定义尽量靠近CMSIS驱动层定义 使用api层封装HAL层
6. Idle任务可以检查任务堆栈\(CPU占用率\)之后WFI处理器
7. 实现文件系统 类似Linux一切皆文件的框架
8. 实现网络协议栈(TCP/IP)
9. 清理对HAL层的调用 直接调用寄存器级别 工程仅依赖于 CMSIS BSP目录

## 注意:
1. 文档的编写使用[md编辑器][1]
2. 任务切换只会发生在 系统调用期间
3. CUBE4 在 E:\Keil\_v5\STM32Cube\_FW\_F4
4. HAL层对RTOS的支持有风险 inc/stm32f4xx\_hal\_conf.h USE\_RTOS
5. 参考FreeRtos RT-Thread实现

---------

[1]: http://write.blog.csdn.net/mdeditor

