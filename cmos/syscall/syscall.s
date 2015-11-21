;******************************************************************************
;*
;* 文件名  ： syscall.s
;* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
;* 创建日期： 20151023
;* 版本号  ： v1.0
;* 文件描述： stm32f芯片的系统调用汇编(for MDK-ARM)
;*            它主要完成系统调用的汇编部分
;* 版权说明： Copyright (c) 2000-2020 GNU
;* 其    他： 无
;* 修改日志： 无
;*
;*******************************************************************************/
                AREA    |.text|, CODE, READONLY

SVC_Handler     PROC
                EXPORT SVC_Handler
                IMPORT syscall_c
                TST   LR, #4                        ; 测试EXC_RETURN的bit2 用于检查当前SP为 PSP or MSP
                ITE   EQ
                MRSEQ R0, MSP                       ; 为0则使用MSP
                MRSNE R0, PSP                       ; 为1则使用MSP
                B     syscall_c                     ; 调用C语言主逻辑
                ENDP

                ; 内部调用创建任务(idle任务)的系统调用不可嵌套,故不使用svc机制
cmos_init       PROC
                EXPORT  cmos_init 
                IMPORT  cmos_init_c
                LDR     R0, = cmos_init_c
                BX      R0
                ENDP

                ; 执行完成后MUC切换到idle任务,idle任务中进入特权级别
                ; 之后只可通过svc作为入口
                ; cmos_start的末尾使用中断返回 故必须使用 svc
cmos_start      PROC
                EXPORT  cmos_start
                SVC 0x01
                ; 出错才会执行到此
                BX LR
                ENDP

cmos_create     PROC
                EXPORT  cmos_create
                SVC 0x10
                BX  LR
                ENDP

cmos_open       PROC
                EXPORT  cmos_open
                SVC 0xa0
                BX  LR
                ENDP

cmos_close      PROC
                EXPORT  cmos_close
                SVC 0xa1
                BX  LR
                ENDP


cmos_read       PROC
                EXPORT  cmos_read
                SVC 0xa2
                BX  LR
                ENDP

cmos_write      PROC
                EXPORT  cmos_write
                SVC 0xa3
                BX  LR
                ENDP


cmos_ioctl      PROC
                EXPORT  cmos_ioctl
                SVC 0xa4
                BX  LR
                ENDP

                ALIGN 
                
                END
