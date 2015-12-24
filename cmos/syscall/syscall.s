;******************************************************************************
;*
;* 文件名  ： syscall.s
;* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
;* 创建日期： 20151023
;* 版本号  ： v1.0
;* 文件描述： stm32f芯片的系统调用汇编(for MDK-ARM)
;*            它主要完成系统调用的汇编部分
;* 版权说明： Copyright (c) 2000-2020 GNU
;* 其    他： 详细的说明见syscall.c
;* 修改日志： 无
;*
;*******************************************************************************/
                PRESERVE8
                THUMB

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

svc_init        PROC
                EXPORT  svc_init
                SVC 0x00
                BX LR
                ENDP

svc_start       PROC
                EXPORT  svc_start
                SVC 0x01
                ; 出错才会执行到此
                BX LR
                ENDP

cmos_running    PROC
                EXPORT  cmos_running
                SVC 0x02
                BX LR
                ENDP

cmos_create     PROC
                EXPORT  cmos_create
                SVC 0x10
                BX  LR
                ENDP

cmos_delay      PROC
                EXPORT  cmos_delay
                SVC 0x20
                BX  LR
                ENDP

svc_ipc         PROC
                EXPORT svc_ipc
                SVC 0x30
                BX  LR
                ENDP

svc_open        PROC
                EXPORT  svc_open
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

svc_write       PROC
                EXPORT  svc_write
                SVC  0xa3
                BX   LR
                ENDP

cmos_ioctl      PROC
                EXPORT  cmos_ioctl
                SVC 0xa4
                BX  LR
                ENDP

                ALIGN 
                
                END
