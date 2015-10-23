;******************************************************************************
;*
;* 文件名  ： syscall.s
;* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
;* 创建日期： 20151023
;* 版本号  ： v1.0
;* 文件描述： stm32f芯片的系统调用汇编(for MDK-ARM)
;*            它主要完成
;*            - 1

;* 版权说明： Copyright (c) 2000-2020 GNU
;* 其    他： 无
;* 修改日志： 无
;*
;*******************************************************************************/
                AREA    |.text|, CODE, READONLY

SVC_Handler     PROC
                IMPORT syscall_c
                TST   LR, #4                        ; 测试EXC_RETURN的bit2 用于检查当前SP为 PSP or MSP
                ITE   EQ
                MRSEQ R0, MSP                       ; 为0则使用MSP
                MRSNE R0, PSP                       ; 为1则使用MSP
                B     syscall_c                     ; 调用C语言主逻辑
                ENDP

cmos_open       PROC
                EXPORT  cmos_open
                SVC 0xa1
                BX  LR
                ENDP

cmos_close      PROC
                EXPORT  cmos_close
                SVC 0xa2
                BX  LR
                ENDP


cmos_read       PROC
                EXPORT  cmos_read
                SVC 0xa3
                BX  LR
                ENDP

cmos_write      PROC
                EXPORT  cmos_write
                SVC 0xa4
                BX  LR
                ENDP


cmos_ioctl      PROC
                EXPORT  cmos_ioctl
                SVC 0xa5
                BX  LR
                ENDP

                ALIGN

