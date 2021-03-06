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
#include "misc.h"
#include "cortex.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static void cmos_task_tcb_stack_init(cmos_task_tcb_T *tcb, cmos_task_tcb_psp_T stack_base);
static cmos_bool_T cmos_task_tcb_with_float(const cmos_task_tcb_T *tcb);

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
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_tcb_init(cmos_task_tcb_T *tcb, 
        const cmos_task_attribute_T *task_attribute,
        cmos_task_tcb_psp_T stack_base)
{
    cmos_int32_T stack_size = 0; /* 栈大小 Byte单位*/

    cmos_assert(NULL != tcb, __FILE__, __LINE__);
    cmos_assert(NULL != task_attribute, __FILE__, __LINE__);

    stack_size = task_attribute->stack_size;
    /* 栈大小必须8Bytes(双字)对齐 */
    cmos_assert(0 == (stack_size & 0x00000007), __FILE__, __LINE__);

    /* TODO: 函数封装 */
    tcb->entry = task_attribute->entry;
    tcb->argv = task_attribute->argv;
    tcb->stack_size = stack_size;

    tcb->priority = task_attribute->priority;
    tcb->tick_total = task_attribute->tick_total;
    tcb->flag = task_attribute->flag;

    tcb->tick = tcb->tick_total;
    tcb->delay_ms = 0;

    /* 初始化任务栈 */
    cmos_task_tcb_stack_init(tcb, stack_base);

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_stack_init
 * 负责人  : 彭鹏
 * 创建日期：20151117 
 * 函数功能: 初始化任务栈
 *
 * 输入参数: ptr_psp    任务栈指针的地址
 *           entry      任务入口函数
 *           argv       任务入口函数参数
 *           stack_base 任务栈指针
 *
 * 输出参数: ptr_psp 任务栈指针的地址
 *
 * 返回值  : 执行状态
 * 调用关系: 无
 * 其 它   : FIXME:目前仅使用非特权任务
 *
 ******************************************************************************/
static void cmos_task_tcb_stack_init(cmos_task_tcb_T *tcb, cmos_task_tcb_psp_T stack_base)
{
    cmos_assert((NULL != tcb) && (NULL != stack_base), __FILE__, __LINE__);

    cmos_task_tcb_psp_T sp = NULL;      /* 任务sp指针 */
    const cmos_func_T entry = tcb->entry;
    const void *argv = tcb->argv;
    cmos_bool_T with_float = FALSE;

    sp = stack_base;

    with_float = cmos_task_tcb_with_float(tcb);

    /* 调试使用 寄存器写入特殊值 */
#if (CMOS_DEBUG_LEVEL > 0) 
    if(with_float)
    {
        sp--;
        *sp = 0xfefefefe; /* cm4要求 双字节对齐 占位 */

        sp--;
        *sp = CMOS_INITIAL_FPSCR; /* FPSCR */

        sp--;
        *sp = 0x0f150f15; /* S15 */

        sp--;
        *sp = 0x0f140f14; /* S14 */

        sp--;
        *sp = 0x0f130f13; /* S13 */

        sp--;
        *sp = 0x0f120f12; /* S12 */

        sp--;
        *sp = 0x0f110f11; /* S11 */

        sp--;
        *sp = 0x0f100f10; /* S10 */

        sp--;
        *sp = 0x0f090f09; /* S09 */

        sp--;
        *sp = 0x0f080f08; /* S08 */

        sp--;
        *sp = 0x0f070f07; /* S07 */

        sp--;
        *sp = 0x0f060f06; /* S06 */

        sp--;
        *sp = 0x0f050f05; /* S05 */

        sp--;
        *sp = 0x0f040f04; /* S04 */ 
        
        sp--;
        *sp = 0x0f030f03; /* S03 */

        sp--;
        *sp = 0x0f020f02; /* S02 */

        sp--;
        *sp = 0x0f010f01; /* S01 */

        sp--;
        *sp = 0x0f000f00; /* S00 */
    }
    sp--;
    *sp = CMOS_INITIAL_XPSR; /* xPSR */

    sp--;
    *sp = (cmos_word_T)entry; /* PC */

    sp--;
    *sp = (cmos_word_T)err_loop; /* LR */

    sp--;
    *sp = 0x12121212; /* R12 */

    sp--;
    *sp = 0x03030303; /* R3 */

    sp--;
    *sp = 0x02020202; /* R2 */

    sp--;
    *sp = 0x01010101; /* R1 */

    sp--;
    *sp = (cmos_word_T)argv; /* R0 */

    /* 任务切换时
     * 以上内容硬件保存
     * 以下内容软件保存 */
    if(with_float) /* 需要浮点 */
    {
        sp--;
        *sp = 0x0f310f31; /* S31 */

        sp--;
        *sp = 0x0f300f30; /* S30 */

        sp--;
        *sp = 0x0f290f29; /* S29 */

        sp--;
        *sp = 0x0f280f28; /* S28 */

        sp--;
        *sp = 0x0f270f27; /* S27 */

        sp--;
        *sp = 0x0f260f26; /* S26 */

        sp--;
        *sp = 0x0f250f25; /* S25 */

        sp--;
        *sp = 0x0f240f24; /* S24 */

        sp--;
        *sp = 0x0f230f23; /* S23 */

        sp--;
        *sp = 0x0f220f22; /* S22 */

        sp--;
        *sp = 0x0f210f21; /* S21 */

        sp--;
        *sp = 0x0f200f20; /* S20 */

        sp--;
        *sp = 0x0f190f19; /* S19 */

        sp--;
        *sp = 0x0f180f18; /* S18 */

        sp--;
        *sp = 0x0f170f17; /* S17 */

        sp--;
        *sp = 0x0f160f16; /* S16 */
    }

    /* 用于任务切换的中断返回 */
    sp--;
    if(with_float)
    {
      *sp = CMOS_INITIAL_EXEC_RETURN_WITH_FLOAT;
    }
    else
    {
      *sp = CMOS_INITIAL_EXEC_RETURN_WITHOUT_FLOAT;
    }

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

    /* 实际使用 FLOAT */
#else
    if(with_float) /* 需要浮点 */
    {
        sp -= 2; /* cm4要求 双字节对齐 占位 + FPSCR */

        *sp = CMOS_INITIAL_FPSCR; /* FPSCR */

        sp -= 16; /* S15 - S0 */
    }

    sp--;
    *sp = CMOS_INITIAL_XPSR; /* xPSR */

    sp--;
    *sp = (cmos_word_T)entry; /* PC */

    sp--;
    *sp = (cmos_word_T)err_loop; /* LR */

    sp -= 4; /* R12 R3 R2 R1 */

    sp--;
    *sp = (cmos_word_T)argv; /* R0 */

    /* 任务切换时
     * 以上内容硬件保存
     * 以下内容软件保存 */

    if(with_float) /* 需要浮点 */
    {
        sp -= 17; /* S32 - S16 and LR */
        *sp = CMOS_INITIAL_EXEC_RETURN_WITH_FLOAT;
    }
    else
    {
        sp--; /* LR */
        *sp = CMOS_INITIAL_EXEC_RETURN_WITHOUT_FLOAT;
    }
    sp -= 8;
#endif

    tcb->psp = sp;

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_get_stack_size
 * 负责人  : 彭鹏
 * 创建日期：20151117 
 * 函数功能: 获取任务栈
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 任务栈大小
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_int32_T cmos_task_tcb_get_stack_size(const cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_get_stack_size with null tcb pointer.");
        return -1;
    }

    return tcb->stack_size;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_get_priority
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 获取任务优先级
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 任务优先级
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_priority_T cmos_task_tcb_get_priority(const cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_get_stack_size with null tcb pointer.");
        return cmos_priority_err;
    }

    return tcb->priority;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_set_psp
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 设置当前任务栈指针
 *
 * 输入参数: cur_psp
 * 输出参数: tcb 任务控制块指针
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_task_tcb_set_psp(cmos_task_tcb_T *tcb, const cmos_task_tcb_psp_T psp)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_set_psp whit null tcb pointer.");
        return;
    }

    tcb->psp = psp;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_get_psp
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 获取当前任务栈指针
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : tcb的psp
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_task_tcb_psp_T cmos_task_tcb_get_psp(const cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_get_psp whit null tcb pointer.");
        return NULL;
    }

    return tcb->psp;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_with_float
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 判断tcb是否需要浮点
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : tcb的psp
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static cmos_bool_T cmos_task_tcb_with_float(const cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_with_float whit null tcb pointer.");
        return FALSE;
    }

    if(0 == (cmos_task_with_float & tcb->flag))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_print
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 调试使用 打印任务栈
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : tcb的psp
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_tcb_stack_print(const cmos_task_tcb_psp_T psp, cmos_int32_T print_size)
{
    cmos_int32_T i = 0;
    cmos_task_tcb_psp_T sp = psp;

    for(i = 0; i < print_size; i++)
    { 
        cmos_console_printf("0x%08x:0x%08x\r\n", sp, *sp);
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_set_delay_ms
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: 设置当前任务延迟
 *
 * 输入参数: ms 延迟数
 * 输出参数: tcb 任务控制块指针
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_task_tcb_set_delay_ms(cmos_task_tcb_T *tcb, cmos_int32_T ms)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_set_delay_ms whit null tcb pointer.");
        return;
    }

    tcb->delay_ms = ms;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_dec_delay_ms
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: 自减tcb中的delay_ms域
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_task_tcb_dec_delay_ms(cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_dec_delay_ms whit null tcb pointer.");
        return;
    }

    /* 避免多次减值溢出 */
    if(0 < tcb->delay_ms)
    {
        tcb->delay_ms--;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_zero_delay_ms
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: tcb中的delay_ms域是否为零
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_bool_T cmos_task_tcb_zero_delay_ms(cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_zero_delay_ms whit null tcb pointer.");
        return FALSE;
    }

    if(0 == tcb->delay_ms)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_dec_tick
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: tcb中的tick域自减
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline void cmos_task_tcb_dec_tick(cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_dec_tick whit null tcb pointer.");
        return;
    }


    /* 避免多次减值溢出 */
    if(0 < tcb->tick)
    {
        tcb->tick--;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_zero_tick
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: tcb中的tick域是否为零
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_bool_T cmos_task_tcb_zero_tick(cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_zero_tick whit null tcb pointer.");
        return FALSE;
    }

    if(0 == tcb->tick_total) /* 独占该优先级 */
    {
        return FALSE;
    }

    if(0 == tcb->tick)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tcb_reset_tick
 * 负责人  : 彭鹏
 * 创建日期：20151201 
 * 函数功能: 复位tick值
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 无
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_tcb_reset_tick(cmos_task_tcb_T *tcb)
{
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_tcb_zero_tick whit null tcb pointer.");
        return;
    }

    tcb->tick = tcb->tick_total;
}

