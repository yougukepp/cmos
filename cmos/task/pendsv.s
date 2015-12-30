;******************************************************************************
;*
;* 文件名  ： pendsv.s
;* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
;* 创建日期： 20151119
;* 版本号  ： v1.1
;* 文件描述： stm32f芯片的任务切换的汇编(for MDK-ARM)
;* 版权说明： Copyright (c) 2000-2020 GNU
;* 其    他： 无
;* 修改日志： 无
;*
;*
;* TODO: 修改为带浮点
;* 任务栈结构:
;*     带浮点                                       无浮点
;*     
;*     
;*     
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
;*
;*
;* 任务切换算法(无浮点):
;*    1. 把 R4-R11 LR(EXC_RETURN)压入PSP栈中, 保护寄存器原来的内容.
;*    2. 更新当前的PSP栈指针为任务调度算法函数返回的栈指针.
;*    3. 利用新的PSP栈指针, 从PSP栈中恢复 R4-R11 LR(EXC_RETURN)的内容.
;*    4. 利用 bx lr 指令进行中断返回, 实现任务切换.
;*    返回地址保存在栈中,而我们把栈指针指向了另一个任务的栈,
;*    所以中断函数的返回地址就被我们偷偷地修改了
;*
;*
;******************************************************************************/
                AREA    |.text|, CODE, READONLY

PendSV_Handler  PROC
                ; 导入调度算法
                IMPORT cmos_task_switch_switch
                EXPORT PendSV_Handler

                ; step 1
                MRS R0 , PSP

                ; step 2
                TST LR, #0x10               ; 测试bit4. =0, 需要入栈浮点寄存器
                IT EQ
                VSTMDBEQ R0!, {S16-S31}     ; 等效于PUSH {S16-S31}

                ; step 3
                STMDB.W R0!, {R4-R11,LR}    ; 等效于PUSH {R4-R11, LR}
                ; r0此时已经是最新的psp值

                ; step 4 r0作为参数和返回值
PendSV_Tail     BL cmos_task_switch_switch
                ; r0此时已经是最新的psp值 
                
                ; step 5
                LDMIA.W R0!, {R4-R11,LR}    ; 等效于POP {R4-R11, LR}

                ; step 6
                TST LR, #0x10               ; 测试bit4. =0, 需要出栈浮点寄存器
                IT EQ
                VLDMIAEQ R0!, {S16-S31}     ; 等效于POP {S16-S31}

                ; step 7
                MSR PSP, R0
                DSB
                ISB

                ; step 8
                BX LR
                ENDP

; 启动调度器
cmos_task_switch_start_s PROC
                EXPORT cmos_task_switch_start_s

                LDR R1, =PendSV_Tail
                BX R1
                ENDP

                ALIGN 
                END

