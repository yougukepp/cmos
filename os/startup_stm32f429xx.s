;******************************************************************************
;*
;* 文件名  ： startup_stm32f429xx.s
;* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
;* 创建日期： 20150410 
;* 版本号  ： v1.0
;* 文件描述： stm32f429xx芯片启动汇编(for MDK-ARM)
;*            它主要完成
;*            - 设置初始SP
;*            - 设置初始PC == Reset_Handler
;*            - 设置向量表
;*            - 跳转到C运行时库的__main函数,该函数会调用main
;*            复位之后Cortex-M4进入Tread模式,特权级别(Privileged),SP使用MSP
;* 
;* 版权说明： Copyright (c) 2000-2020 GNU
;* 其    他： 无
;* 修改日志： 无
;*
;*******************************************************************************/

; data + bbs + 4k系统栈 + 188k堆 = 192k 片内 SRAM
; 系统栈(MSP) 4kB
Stack_Size      EQU     0x0001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

; 堆 188kBytes - data - bss端
; 需要查看 .map文件中Execution Region RW_IRAM1
Heap_Size       EQU     0x00001000

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

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; 外部中断 暂时不实现
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

SVC_Handler     PROC 
                IMPORT syscall_c
                TST   LR, #4                        ; 测试EXC_RETURN的bit2 用于检查当前SP为 PSP or MSP
                ITE   EQ
                MRSEQ R0, MSP                       ; 为0则使用MSP
                MRSNE R0, PSP                       ; 为1则使用MSP
                B     syscall_c                     ; 调用C语言主逻辑
                ENDP

DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler
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
                LDMIA.W R0!, {R4-R11,LR} ; /* 等效于 POP {R4-R11, LR} */
                MSR PSP, R0
                ISB 
                
                ; step 4
                BX LR
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
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

