# cmos说明文档
cmos将作为一个符合CMSIS标准的系统,他是Cortex-M Operating System的简称.本文档暂时用于描述开发计划.

## 目录结构
待施工

## 代码原则
1. 依据CMOS结构图构建目录结构
2. 所有函数参数检查
3. 部分静态变量使用const
4. 去除跨层次耦合
5. 系统调用加用户空间运行部分

## 计划
### 测试计划

### 近期计划
1. 实现串口命令行
2. 实现app/flyer/doc/README.md计划

### 远期计划
1. 同步问题
   a. 关中断(已经实现)
   b. 调度器上锁(已经实现)
   c. 自旋锁
   d. 信号量(包含互斥量)
2. 由于串口打印使用系统调用,故svc调用栈内的代码无法使用串口打印?
3. 空指针检查用宏实现(优化性能)
4. 整理TODO
5. vfs与tree间加入中间层,参考tcb\_list.c
6. Idle任务可以检查任务堆栈\(CPU占用率\)之后WFI处理器
7. 实现文件系统 类似Linux一切皆文件的框架
8. 实现网络协议栈(TCP/IP)
9. 清理对HAL层的调用 直接调用寄存器级别 工程仅依赖于 CMSIS BSP目录
10. 实现驱动层RCC时钟模块封装

## 注意:
1. 文档的编写使用[md编辑器][1]
2. 任务切换只会发生在 系统调用期间
3. CUBE4 在 E:\Keil\_v5\STM32Cube\_FW\_F4
4. HAL层对RTOS的支持有风险 inc/stm32f4xx\_hal\_conf.h USE\_RTOS
5. 参考FreeRtos RT-Thread实现

---------

[1]: http://write.blog.csdn.net/mdeditor

