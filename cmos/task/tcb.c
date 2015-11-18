/******************************************************************************
 *
 * 文件名  ： tcb.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151176 
 * 版本号  ： v1.1
 * 文件描述： 实现任务控制块的相关功能
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"
#include "console.h"


/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_init
 * 负责人  : 彭鹏
 * 创建日期：20151117 
 * 函数功能: 创建任务
 *
 * 输入参数: task_func      任务入口
 *           argv           任务参数
 *           task_attribute 任务属性 堆栈 优先级 等
 *
 * 输出参数: task_id 任务id号
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_tcb_init(cmos_task_tcb_T *tcb, 
        cmos_uint8_T *stack_base,
        cmos_func_T entry,
        void *argv,
        const cmos_task_attribute_T *task_attribute)
{
    cmos_int32_T stack_size = 0; /* 栈大小 Byte单位*/
    cmos_status_T status = cmos_ERR_E;

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_init with null tcb pointer.");
        return cmos_NULL_E;
    }

    if((NULL == entry)
    || (NULL == task_attribute))
    {
        CMOS_ERR_STR("task func and task attribute should not to be null.");
        return cmos_PARA_E;
    }

    stack_size = task_attribute->stack_size;
    /* 栈大小必须8Bytes(双字)对齐 */
    if(0 != (stack_size & 0x00000007))
    {
        return cmos_PARA_E;
    }

    tcb->entry = entry;
    tcb->argv = argv;
    tcb->stack_size = stack_size;

    tcb->priority = task_attribute->priority;
    tcb->tick_total = task_attribute->tick_total;

    tcb->tick = 0;

    /* 初始化任务栈 */
    status = cmos_task_tcb_stack_init(tcb, stack_base);
    if(cmos_OK_E != status)
    {
        return status;
    }

    tcb->psp = s_user_stack_base;

    s_user_stack_base -= stack_size; /* 占用 */
}

static cmos_status_T cmos_task_tcb_stack_init(cmos_task_tcb_T *tcb, cmos_uint8_T *stack_bask)
{
    sp--;
    *sp = CMOS_INITIAL_XPSR; /* xPSR */

    sp--;
    *sp = (cm_uint32_t)funcName; /* PC */

    sp--;
    *sp = (cm_uint32_t )Error_Handler; /* LR */

    /* 保留寄存器 R12 R3 R2 R1 R0 的空间 */
#if 0
    sp -= 5;    /* R12, R3, R2 and R1. */
    *sp = (cm_uint32_t )argv; /* R0 */
#else /* 调试 */
    sp--;
    *sp = 0x12121212; /* R12 */

    sp--;
    *sp = 0x03030303; /* R3 */

    sp--;
    *sp = 0x02020202; /* R2 */

    sp--;
    *sp = 0x01010101; /* R1 */

    sp--;
    *sp = (cm_uint32_t)argv; /* R0 */
#endif

    /* 用于任务切换的中断返回 */
    sp--;
    *sp = CMOS_INITIAL_EXEC_RETURN;

    /* 保留R11, R10, R9, R8, R7, R6, R5 and R4. */
#if 0
    sp -= 8;
#else /* 调试 */
    sp--;
    *sp = 0x11111111; /* R11 */

    sp--;
    *sp = 0x10101010; /* R10 */

    sp--;
    *sp = 0x09090909; /* R9 */

    sp--;
    *sp = 0x08080808; /* R8 */

    sp--;
    *sp = 0x07070707; /* R7 */

    sp--;
    *sp = 0x06060606; /* R6 */

    sp--;
    *sp = 0x05050505; /* R5 */

    sp--;
    *sp = 0x04040404; /* R4 */
#endif

    return sp;
}

