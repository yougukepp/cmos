;******************************************************************************
;*
;* 文件名  ： startup_stm32f429xx.s
;* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
;* 创建日期： 20150410
;* 版本号  ： v1.0
;* 文件描述： stm32f429xx芯片启动汇编(for MDK-ARM)
;*            它主要完成
;*            - 设置向量表
;*            - 设置初始SP
;*            - 设置初始PC == Reset_Handler
;*            - 跳转到C运行时库的__main函数,该函数会调用main
;*            - 设置系统堆栈
;*            - 实现任务切换 PendSV_Handler
;*            - 实现系统调用 SVC_Handler
;*            - 实现系统调用函数
;*            - 完成首个任务启动 first_thread_start
;*            复位之后Cortex-M4进入Tread模式,特权级别(Privileged),SP使用MSP
;*
;* 版权说明： Copyright (c) 2000-2020 GNU
;* 其    他： 无
;* 修改日志： 无
;*
;*******************************************************************************/

; data + bbs + 4k系统栈 + 188k堆 = 192k 片内 SRAM
; 系统栈(MSP) 4kB
Stack_Size      EQU     0x0004000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

; 堆 188kBytes - data - bss端
; 需要查看 .map文件中Execution Region RW_IRAM1
Heap_Size       EQU     0x00004000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; 复位后向量表位于0x00000000
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp            ; Top of Stack
                DCD     Reset_Handler           ; Reset Handler
                DCD     NMI_Handler             ; NMI Handler
                DCD     HardFault_Handler       ; Hard Fault Handler
                DCD     MemManage_Handler       ; MPU Fault Handler
                DCD     BusFault_Handler        ; Bus Fault Handler
                DCD     UsageFault_Handler      ; Usage Fault Handler
                DCD     0                       ; Reserved
                DCD     0                       ; Reserved
                DCD     0                       ; Reserved
                DCD     0                       ; Reserved
                DCD     SVC_Handler             ; SVCall Handler
                DCD     DebugMon_Handler        ; Debug Monitor Handler
                DCD     0                       ; Reserved
                DCD     PendSV_Handler          ; PendSV Handler
                DCD     SysTick_Handler         ; SysTick Handler

                ; 外部中断 仅实现USART1作为控制台
                DCD     0                       ; Window WatchDog
                DCD     0                       ; PVD through EXTI Line detection
                DCD     0                       ; Tamper and TimeStamps through the EXTI line
                DCD     0                       ; RTC Wakeup through the EXTI line
                DCD     0                       ; FLASH
                DCD     0                       ; RCC
                DCD     0                       ; EXTI Line0
                DCD     0                       ; EXTI Line1
                DCD     0                       ; EXTI Line2
                DCD     0                       ; EXTI Line3
                DCD     0                       ; EXTI Line4
                DCD     0                       ; DMA1 Stream 0
                DCD     0                       ; DMA1 Stream 1
                DCD     0                       ; DMA1 Stream 2
                DCD     0                       ; DMA1 Stream 3
                DCD     0                       ; DMA1 Stream 4
                DCD     0                       ; DMA1 Stream 5
                DCD     0                       ; DMA1 Stream 6
                DCD     0                       ; ADC1, ADC2 and ADC3s
                DCD     0                       ; CAN1 TX
                DCD     0                       ; CAN1 RX0
                DCD     0                       ; CAN1 RX1
                DCD     0                       ; CAN1 SCE
                DCD     0                       ; External Line[9:5]s
                DCD     0                       ; TIM1 Break and TIM9
                DCD     0                       ; TIM1 Update and TIM10
                DCD     0                       ; TIM1 Trigger and Commutation and TIM11
                DCD     0                       ; TIM1 Capture Compare
                DCD     0                       ; TIM2
                DCD     0                       ; TIM3
                DCD     0                       ; TIM4
                DCD     0                       ; I2C1 Event
                DCD     0                       ; I2C1 Error
                DCD     0                       ; I2C2 Event
                DCD     0                       ; I2C2 Error
                DCD     0                       ; SPI1
                DCD     0                       ; SPI2
                DCD     Console_Handler         ; USART1
                DCD     0                       ; USART2
                DCD     0                       ; USART3
                DCD     0                       ; External Line[15:10]s
                DCD     0                       ; RTC Alarm (A and B) through EXTI Line
                DCD     0                       ; USB OTG FS Wakeup through EXTI line
                DCD     0                       ; TIM8 Break and TIM12
                DCD     0                       ; TIM8 Update and TIM13
                DCD     0                       ; TIM8 Trigger and Commutation and TIM14
                DCD     0                       ; TIM8 Capture Compare
                DCD     0                       ; DMA1 Stream7
                DCD     0                       ; FMC
                DCD     0                       ; SDIO
                DCD     0                       ; TIM5
                DCD     0                       ; SPI3
                DCD     0                       ; UART4
                DCD     0                       ; UART5
                DCD     0                       ; TIM6 and DAC1&2 underrun errors
                DCD     0                       ; TIM7
                DCD     0                       ; DMA2 Stream 0
                DCD     0                       ; DMA2 Stream 1
                DCD     0                       ; DMA2 Stream 2
                DCD     0                       ; DMA2 Stream 3
                DCD     0                       ; DMA2 Stream 4
                DCD     0                       ; Ethernet
                DCD     0                       ; Ethernet Wakeup through EXTI line
                DCD     0                       ; CAN2 TX
                DCD     0                       ; CAN2 RX0
                DCD     0                       ; CAN2 RX1
                DCD     0                       ; CAN2 SCE
                DCD     0                       ; USB OTG FS
                DCD     0                       ; DMA2 Stream 5
                DCD     0                       ; DMA2 Stream 6
                DCD     0                       ; DMA2 Stream 7
                DCD     0                       ; USART6
                DCD     0                       ; I2C3 event
                DCD     0                       ; I2C3 error
                DCD     0                       ; USB OTG HS End Point 1 Out
                DCD     0                       ; USB OTG HS End Point 1 In
                DCD     0                       ; USB OTG HS Wakeup through EXTI
                DCD     0                       ; USB OTG HS
                DCD     0                       ; DCMI
                DCD     0                       ; Reserved
                DCD     0                       ; Hash and Rng
                DCD     0                       ; FPU
                DCD     0                       ; UART7
                DCD     0                       ; UART8
                DCD     0                       ; SPI4
                DCD     0                       ; SPI5
                DCD     0                       ; SPI6
                DCD     0                       ; SAI1
                DCD     0                       ; LTDC
                DCD     0                       ; LTDC error
                DCD     0                       ; DMA2D
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; 复位句柄
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; 系统心跳句柄
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

; 串口控制台句柄
Console_Handler PROC
                EXPORT  Console_Handler            [WEAK]
                B       .
                ENDP

;*******************************************************************************
;* 任务切换:
;*    1. 把 R4-R11 LR(EXC_RETURN)压入PSP栈中, 保护寄存器原来的内容.
;*    2. 更新当前的PSP栈指针为任务调度算法函数返回的栈指针.
;*    3. 利用新的PSP栈指针, 从PSP栈中恢复 R4-R11 LR(EXC_RETURN)的内容.
;*    4. 利用 bx lr 指令进行中断返回, 实现任务切换.
;*    返回地址保存在栈中,而我们把栈指针指向了另一个任务的栈,
;*    所以中断函数的返回地址就被我们偷偷地修改了
;*
;* 任务栈结构:
;*     xPSR
;*     PC
;*     LR
;*     R12
;*     R3
;*     R2
;*     R1
;*     R0             以上硬件负责入栈
;*     ---------------------------------
;*     LR(EXC_RETURN) 以下软件负责入栈
;*     R11
;*     R10
;*     R9
;*     R8
;*     R7
;*     R6
;*     R5
;*     R4             新PS指向
;*
;******************************************************************************/
PendSV_Handler  PROC
                ; 导入调度算法
                IMPORT thread_switch

                ; step 1
                MRS R0 , PSP
                STMDB.W R0!, {R4-R11,LR} ; 等效于PUSH {R4-R11, LR}
                ; r0此时已经是最新的psp值

                ; step 2 r0作为参数和返回值
                BL thread_switch
                ; r0此时已经是最新的psp值

                ; step 3
step3           LDMIA.W R0!, {R4-R11,LR} ; /* 等效于 POP {R4-R11, LR} */
                MSR PSP, R0
                ISB

                ; step 4
                BX LR
                ENDP

SVC_Handler     PROC
                IMPORT syscall_c
                TST   LR, #4                        ; 测试EXC_RETURN的bit2 用于检查当前SP为 PSP or MSP
                ITE   EQ
                MRSEQ R0, MSP                       ; 为0则使用MSP
                MRSNE R0, PSP                       ; 为1则使用MSP
                B     syscall_c                     ; 调用C语言主逻辑
                ENDP

; 未使用的异常 死循环
NMI_Handler     PROC
                EXPORT  NMI_Handler
                B       .
                ENDP

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler
                B       .
                ENDP

MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler
                B       .
                ENDP

BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler
                B       .
                ENDP

UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler
                B       .
                ENDP

DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler
                B       .
                ENDP


;*******************************************************************************
; 系统调用
;*******************************************************************************
kernel_initialize PROC
                IMPORT  idle_create
                EXPORT  kernel_initialize          [WEAK]
                SVC 0x00                           ; 1. 初始化
                PUSH {LR}
                LDR R0, =idle_create               ; 2. 创建并启动idle线程
                BLX R0
                POP {LR}
                BX  LR
                ENDP

kernel_start    PROC
                EXPORT  kernel_start               [WEAK]
                SVC 0x01
                BX  LR
                ENDP

thread_create   PROC
                EXPORT  thread_create              [WEAK]
                SVC 0x10
                BX  LR
                ENDP

delay           PROC
                EXPORT  delay                      [WEAK]
                SVC 0x20
                ;PUSH {LR}
                ;返回之前需要确保已经进行任务切换了
                ;LDR R0, =switch_done
                ;BLX R0
                ;POP {LR}
                BX  LR
                ENDP

; 汇编函数库
; 初始启动线程
first_thread_start PROC
                EXPORT  first_thread_start
                MRS R0, PSP
                LDR R1, =step3
                BX R1
                ENDP

; 获取堆分布信息
get_heap_base   PROC
                EXPORT  get_heap_base
                LDR R0, =__heap_base
                BX LR
                ENDP

get_heap_limit  PROC
                EXPORT  get_heap_limit
                LDR R0, =__heap_limit
                BX LR
                ENDP

get_initial_sp  PROC
                EXPORT  get_initial_sp
                LDR R0, =__initial_sp
                BX LR
                ENDP


                ALIGN

;*******************************************************************************
; 堆栈初始化
;*******************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap
                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

