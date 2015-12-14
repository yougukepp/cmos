/******************************************************************************
 *
 * 文件名  ： ready.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151211 
 * 版本号  ： v1.0
 * 文件描述： 就绪任务实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 该模块实现了O(1)算法
 *            1. 优先级索引
 *            2. 位图表
 *            3. 每个优先级一个tcb链表
 *
 *            注意:该模块运行域svc中断中 不需要锁
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "ready.h"
#include "list.h"
#include "list_array.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/





/********************************** 函数声明区 *********************************/
static void set_tcb_table_by_priority(cmos_uint8_T priority, cmos_lib_list_T *tcb_list);
static cmos_lib_list_T *get_tcb_table_by_priority(cmos_int32_T priority);
static cmos_uint8_T get_bitmap_index(cmos_int32_T priority);
static void add_priority(cmos_priority_T priority);


/********************************** 变量实现区 *********************************/
/*******************************************************************************
 *
 * 跟踪(|运算)系统所有任务的优先级,便于在位图表中查找最高优先级任务 
 *
 * cmos_priority_idle           bit0
 * cmos_priority_low            bit1
 * cmos_priority_below_normal   bit2
 * cmos_priority_normal         bit3
 * cmos_priority_above_normal   bit4
 * cmos_priority_high           bit5
 * cmos_priority_realtime       bit6
 *
 * cmos_priority_err            bit7
 *
 * 0x00   < s_priority_index  <= 0x7F
 * 0x00  无任务                       
 * >0x7f 有错误优先级任务
 *
 * idle任务建立后 s_priority_index 变为 0x01
 *
 ******************************************************************************/
static cmos_int32_T s_priority_index = 0x00;

/*******************************************************************************
 *
 * 优先级任务链表数组
 * 每个元素指向某一优先级的任务链表
 *
 * cmos_priority_idle           0
 * cmos_priority_low            1
 * cmos_priority_below_normal   2
 * cmos_priority_normal         3
 * cmos_priority_above_normal   4
 * cmos_priority_high           5
 * cmos_priority_realtime       6
 *
 ******************************************************************************/
static cmos_lib_list_T *s_ready_tcb[CMOS_TASK_POOL_READY_PRIORITY_MAX] = {NULL};

/*******************************************************************************
 *
 * 每个位置表示该s_priority_index对应的系统最高优先级
 * tools/generate_bitmap.py生成
 *
 * 每行的内容依次为: 最高优先级(即s_ready_tcb索引) s_priority_index值 cmos_priority_T值
 *
 ******************************************************************************/
const static cmos_uint8_T s_priority_bitmap[] = 
{
    0,  /* 00000000 Idle 没有任何优先级线程就绪 这是不可能的情况 */
    0,  /* 00000001 Idle */
    1,  /* 00000010 Low */
    1,  /* 00000011 Low */
    2,  /* 00000100 BelowNormal */
    2,  /* 00000101 BelowNormal */
    2,  /* 00000110 BelowNormal */
    2,  /* 00000111 BelowNormal */
    3,  /* 00001000 Normal */
    3,  /* 00001001 Normal */
    3,  /* 00001010 Normal */
    3,  /* 00001011 Normal */
    3,  /* 00001100 Normal */
    3,  /* 00001101 Normal */
    3,  /* 00001110 Normal */
    3,  /* 00001111 Normal */
    4,  /* 00010000 AboveNormal */
    4,  /* 00010001 AboveNormal */
    4,  /* 00010010 AboveNormal */
    4,  /* 00010011 AboveNormal */
    4,  /* 00010100 AboveNormal */
    4,  /* 00010101 AboveNormal */
    4,  /* 00010110 AboveNormal */
    4,  /* 00010111 AboveNormal */
    4,  /* 00011000 AboveNormal */
    4,  /* 00011001 AboveNormal */
    4,  /* 00011010 AboveNormal */
    4,  /* 00011011 AboveNormal */
    4,  /* 00011100 AboveNormal */
    4,  /* 00011101 AboveNormal */
    4,  /* 00011110 AboveNormal */
    4,  /* 00011111 AboveNormal */
    5,  /* 00100000 High */
    5,  /* 00100001 High */
    5,  /* 00100010 High */
    5,  /* 00100011 High */
    5,  /* 00100100 High */
    5,  /* 00100101 High */
    5,  /* 00100110 High */
    5,  /* 00100111 High */
    5,  /* 00101000 High */
    5,  /* 00101001 High */
    5,  /* 00101010 High */
    5,  /* 00101011 High */
    5,  /* 00101100 High */
    5,  /* 00101101 High */
    5,  /* 00101110 High */
    5,  /* 00101111 High */
    5,  /* 00110000 High */
    5,  /* 00110001 High */
    5,  /* 00110010 High */
    5,  /* 00110011 High */
    5,  /* 00110100 High */
    5,  /* 00110101 High */
    5,  /* 00110110 High */
    5,  /* 00110111 High */
    5,  /* 00111000 High */
    5,  /* 00111001 High */
    5,  /* 00111010 High */
    5,  /* 00111011 High */
    5,  /* 00111100 High */
    5,  /* 00111101 High */
    5,  /* 00111110 High */
    5,  /* 00111111 High */
    6,  /* 01000000 Realtime */
    6,  /* 01000001 Realtime */
    6,  /* 01000010 Realtime */
    6,  /* 01000011 Realtime */
    6,  /* 01000100 Realtime */
    6,  /* 01000101 Realtime */
    6,  /* 01000110 Realtime */
    6,  /* 01000111 Realtime */
    6,  /* 01001000 Realtime */
    6,  /* 01001001 Realtime */
    6,  /* 01001010 Realtime */
    6,  /* 01001011 Realtime */
    6,  /* 01001100 Realtime */
    6,  /* 01001101 Realtime */
    6,  /* 01001110 Realtime */
    6,  /* 01001111 Realtime */
    6,  /* 01010000 Realtime */
    6,  /* 01010001 Realtime */
    6,  /* 01010010 Realtime */
    6,  /* 01010011 Realtime */
    6,  /* 01010100 Realtime */
    6,  /* 01010101 Realtime */
    6,  /* 01010110 Realtime */
    6,  /* 01010111 Realtime */
    6,  /* 01011000 Realtime */
    6,  /* 01011001 Realtime */
    6,  /* 01011010 Realtime */
    6,  /* 01011011 Realtime */
    6,  /* 01011100 Realtime */
    6,  /* 01011101 Realtime */
    6,  /* 01011110 Realtime */
    6,  /* 01011111 Realtime */
    6,  /* 01100000 Realtime */
    6,  /* 01100001 Realtime */
    6,  /* 01100010 Realtime */
    6,  /* 01100011 Realtime */
    6,  /* 01100100 Realtime */
    6,  /* 01100101 Realtime */
    6,  /* 01100110 Realtime */
    6,  /* 01100111 Realtime */
    6,  /* 01101000 Realtime */
    6,  /* 01101001 Realtime */
    6,  /* 01101010 Realtime */
    6,  /* 01101011 Realtime */
    6,  /* 01101100 Realtime */
    6,  /* 01101101 Realtime */
    6,  /* 01101110 Realtime */
    6,  /* 01101111 Realtime */
    6,  /* 01110000 Realtime */
    6,  /* 01110001 Realtime */
    6,  /* 01110010 Realtime */
    6,  /* 01110011 Realtime */
    6,  /* 01110100 Realtime */
    6,  /* 01110101 Realtime */
    6,  /* 01110110 Realtime */
    6,  /* 01110111 Realtime */
    6,  /* 01111000 Realtime */
    6,  /* 01111001 Realtime */
    6,  /* 01111010 Realtime */
    6,  /* 01111011 Realtime */
    6,  /* 01111100 Realtime */
    6,  /* 01111101 Realtime */
    6,  /* 01111110 Realtime */
    6   /* 01111111 Realtime */
};





/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_ready_add
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 将任务加入到就绪任务链表
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_pool_ready_add(const cmos_task_tcb_T *tcb)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_priority_T priority = cmos_priority_err;
    cmos_lib_list_T *tcb_list = NULL;

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_pool_ready_add with null tcb pointer.");
        return cmos_NULL_E;
    }

    priority = cmos_task_tcb_get_priority(tcb);

    tcb_list = get_tcb_table_by_priority(priority);

    /* tcb_list为空会自动将tcb作为头结点 */
    status = cmos_lib_list_push_tail(&tcb_list, tcb); /* 加入到该优先级链表 */
    if(cmos_OK_E != status)
    {
        CMOS_ERR_STR("cmos_lib_list_push_tail failed.");
        return status;
    }

    /* 处理边界条件: tcb是该优先级首个任务 */
    if(NULL == get_tcb_table_by_priority(priority))
    { 
        /* 更新优先级索引和链表 */
        add_priority(priority);
        set_tcb_table_by_priority(priority, tcb_list);
    } 

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_ready_get_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151213 
 * 函数功能: 获取最高优先级就绪任务tcb
 *
 * 输入参数: 无
 * 输出参数: 最高优先级任务tcb
 *
 * 返回值  : 最高优先级psp
 * 调用关系: 无
 * 其 它   : O(1)算法
 *
 ******************************************************************************/
cmos_task_tcb_T *cmos_task_pool_ready_get_tcb(void)
{ 
    cmos_lib_list_T *tcb_list = NULL;
    cmos_task_tcb_T *tcb = NULL;

    tcb_list = get_tcb_table_by_priority(s_priority_index);
    tcb = cmos_lib_list_get_head_data(tcb_list);
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_pool_ready_get_tcb get a null tcb pointer.");
        return NULL;
    } 
    
    return tcb;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_ready_pop_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: 弹出最高优先级就绪任务tcb
 *
 * 输入参数: 无
 * 输出参数: 最高优先级任务tcb
 *
 * 返回值  : 最高优先级psp
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_task_tcb_T *cmos_task_pool_ready_pop_tcb(void)
{ 
    cmos_lib_list_T *tcb_list = NULL;
    cmos_task_tcb_T *tcb = NULL;
    cmos_uint8_T index = 0;
    cmos_uint8_T highest_priority = 0;

    tcb_list = get_tcb_table_by_priority(s_priority_index);
    tcb = cmos_lib_list_pop_head(&tcb_list);
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_pool_ready_get_tcb get a null tcb pointer.");
    } 

    /* 弹出的是该优先级最后一个任务 回写 s_ready_tcb */
    if(NULL == tcb_list)
    {
        index = get_bitmap_index(s_priority_index); 
        cmos_task_pool_list_array_set(s_ready_tcb, index, NULL);

        /* 将最高优先级置零 */
        highest_priority = (0x1 << index);
        s_priority_index &= ~highest_priority;
    }
    
    return tcb;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_pool_ready_update_tick
 * 负责人  : 彭鹏
 * 创建日期：20151214 
 * 函数功能: tick定时到 更新内部数据结构
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 最高优先级psp
 * 调用关系: 无
 * 其 它   : running模块已经更新了当前(最高优先级就绪任务)任务的tick值
 *           故此处不再自减
 *
 ******************************************************************************/
void cmos_task_pool_ready_update_tick(void)
{ 
    cmos_status_T status = cmos_ERR_E;
    cmos_task_tcb_T *tcb = NULL;
    
    tcb = cmos_task_pool_ready_get_tcb();
    if(cmos_task_tcb_zero_tick(tcb)) /* 时间片运行完 */
    {
        /* 重置时间片 */
        cmos_task_tcb_reset_tick(tcb);

        /* 将当前任务移动到该优先级链表的末尾 */
        tcb = cmos_task_pool_ready_pop_tcb();
        if(NULL == tcb)
        {
            CMOS_ERR_STR("cmos_task_pool_ready_pop_tcb get a null pointer.");
            return;
        }
        status = cmos_task_pool_ready_add(tcb);
        if(cmos_OK_E != status)
        {
            CMOS_ERR_STR("cmos_task_pool_ready_add err.");
            return;
        }
    }

    /* 其他情况不处理 */
    return;
}

/*******************************************************************************
 *
 * 函数名  : get_highest_priority
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 通过优先级获取位图索引
 *
 * 输入参数: priority 优先级
 * 输出参数: 无
 *
 * 返回值  : 最高优先级
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void set_tcb_table_by_priority(cmos_uint8_T priority, cmos_lib_list_T *tcb_list)
{
    cmos_int32_T index = 0;

    index = get_bitmap_index(priority);
    cmos_task_pool_list_array_set(s_ready_tcb, index, tcb_list);
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_tcb_list_by_priority
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 获取priority对应的tcb表头
 *
 * 输入参数: priority 优先级
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static cmos_lib_list_T *get_tcb_table_by_priority(cmos_int32_T priority)
{ 
    cmos_int32_T index = 0;

    index = get_bitmap_index(priority);
    return cmos_task_pool_list_array_get(s_ready_tcb, index);
}

/*******************************************************************************
 *
 * 函数名  : add_priority
 * 负责人  : 彭鹏
 * 创建日期：20151213 
 * 函数功能: 加入一个新优先级
 *
 * 输入参数: priority 优先级
 * 输出参数: 无
 * 返回值  : 无
 *
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void add_priority(cmos_priority_T priority)
{
    s_priority_index |=  priority; 
}

/*******************************************************************************
 *
 * 函数名  : get_highest_priority
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 通过优先级获取位图索引
 *
 * 输入参数: priority 优先级
 * 输出参数: 无
 *
 * 返回值  : 最高优先级
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static cmos_uint8_T get_bitmap_index(cmos_int32_T priority)
{
    return s_priority_bitmap[priority];
}



#if 0
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"
#include "tcb_list.h"
#include "switch.h"
#include "console.h"
#include "cortex.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_task_tcb_list_T *cmos_task_switch_get_tcb_list_by_priority(cmos_int32_T priority);
static cmos_status_T cmos_task_switch_init_first_tcb(const cmos_task_tcb_T *tcb);
static cmos_int32_T cmos_task_switch_get_highest_priority(cmos_int32_T priority);
static cmos_task_tcb_T *cmos_task_switch_get_highest_tcb(void);
static void cmos_task_switch_set_running_tcb(const cmos_task_tcb_T *tcb);
static void cmos_task_switch_update_delay_tcb(cmos_task_tcb_T *tcb, void *para);
static void cmos_task_switch_set_tcb_list_by_priority(cmos_int32_T priority, cmos_task_tcb_list_T *list);

/********************************** 变量实现区 *********************************/
/* 当前运行任务的tcb 用于任务恢复 */
cmos_task_tcb_T *s_running_tcb = NULL; /* 需要加锁 */

/* cmos_delay的任务链表 */
static cmos_task_tcb_list_T *s_delay_tcb_list = NULL;

/*******************************************************************************
 *
 * 跟踪(|运算)系统所有任务的优先级,便于在位图表中查找最高优先级任务 
 *
 * cmos_priority_idle           bit0
 * cmos_priority_low            bit1
 * cmos_priority_below_normal   bit2
 * cmos_priority_normal         bit3
 * cmos_priority_above_normal   bit4
 * cmos_priority_high           bit5
 * cmos_priority_realtime       bit6
 *
 * cmos_priority_err            bit7
 *
 * 0x00   < s_priority_index  <= 0x7F
 * 0x00  无任务                       
 * >0x7f 有错误优先级任务
 *
 * idle任务建立后 s_priority_index 变为 0x01
 *
 ******************************************************************************/
static cmos_int32_T s_priority_index = 0x00;

/*******************************************************************************
 *
 * 优先级任务链表数组
 * 每个元素指向某一优先级的任务链表
 *
 * cmos_priority_idle           0
 * cmos_priority_low            1
 * cmos_priority_below_normal   2
 * cmos_priority_normal         3
 * cmos_priority_above_normal   4
 * cmos_priority_high           5
 * cmos_priority_realtime       6
 *
 ******************************************************************************/
static cmos_task_tcb_list_T *s_ready_tcb[CMOS_PRIORITY_NUMS] = {NULL};

/*******************************************************************************
 *
 * 每个位置表示该s_priority_index对应的系统最高优先级
 * tools/generate_bitmap.py生成
 *
 * 每行的内容依次为: 最高优先级(即s_ready_tcb索引) s_priority_index值 cmos_priority_T值
 *
 ******************************************************************************/
const static cmos_uint8_T s_priority_bitmap[] = {
    0,  /* 00000000 Idle 没有任何优先级线程就绪 这是不可能的情况 */
    0,  /* 00000001 Idle */
    1,  /* 00000010 Low */
    1,  /* 00000011 Low */
    2,  /* 00000100 BelowNormal */
    2,  /* 00000101 BelowNormal */
    2,  /* 00000110 BelowNormal */
    2,  /* 00000111 BelowNormal */
    3,  /* 00001000 Normal */
    3,  /* 00001001 Normal */
    3,  /* 00001010 Normal */
    3,  /* 00001011 Normal */
    3,  /* 00001100 Normal */
    3,  /* 00001101 Normal */
    3,  /* 00001110 Normal */
    3,  /* 00001111 Normal */
    4,  /* 00010000 AboveNormal */
    4,  /* 00010001 AboveNormal */
    4,  /* 00010010 AboveNormal */
    4,  /* 00010011 AboveNormal */
    4,  /* 00010100 AboveNormal */
    4,  /* 00010101 AboveNormal */
    4,  /* 00010110 AboveNormal */
    4,  /* 00010111 AboveNormal */
    4,  /* 00011000 AboveNormal */
    4,  /* 00011001 AboveNormal */
    4,  /* 00011010 AboveNormal */
    4,  /* 00011011 AboveNormal */
    4,  /* 00011100 AboveNormal */
    4,  /* 00011101 AboveNormal */
    4,  /* 00011110 AboveNormal */
    4,  /* 00011111 AboveNormal */
    5,  /* 00100000 High */
    5,  /* 00100001 High */
    5,  /* 00100010 High */
    5,  /* 00100011 High */
    5,  /* 00100100 High */
    5,  /* 00100101 High */
    5,  /* 00100110 High */
    5,  /* 00100111 High */
    5,  /* 00101000 High */
    5,  /* 00101001 High */
    5,  /* 00101010 High */
    5,  /* 00101011 High */
    5,  /* 00101100 High */
    5,  /* 00101101 High */
    5,  /* 00101110 High */
    5,  /* 00101111 High */
    5,  /* 00110000 High */
    5,  /* 00110001 High */
    5,  /* 00110010 High */
    5,  /* 00110011 High */
    5,  /* 00110100 High */
    5,  /* 00110101 High */
    5,  /* 00110110 High */
    5,  /* 00110111 High */
    5,  /* 00111000 High */
    5,  /* 00111001 High */
    5,  /* 00111010 High */
    5,  /* 00111011 High */
    5,  /* 00111100 High */
    5,  /* 00111101 High */
    5,  /* 00111110 High */
    5,  /* 00111111 High */
    6,  /* 01000000 Realtime */
    6,  /* 01000001 Realtime */
    6,  /* 01000010 Realtime */
    6,  /* 01000011 Realtime */
    6,  /* 01000100 Realtime */
    6,  /* 01000101 Realtime */
    6,  /* 01000110 Realtime */
    6,  /* 01000111 Realtime */
    6,  /* 01001000 Realtime */
    6,  /* 01001001 Realtime */
    6,  /* 01001010 Realtime */
    6,  /* 01001011 Realtime */
    6,  /* 01001100 Realtime */
    6,  /* 01001101 Realtime */
    6,  /* 01001110 Realtime */
    6,  /* 01001111 Realtime */
    6,  /* 01010000 Realtime */
    6,  /* 01010001 Realtime */
    6,  /* 01010010 Realtime */
    6,  /* 01010011 Realtime */
    6,  /* 01010100 Realtime */
    6,  /* 01010101 Realtime */
    6,  /* 01010110 Realtime */
    6,  /* 01010111 Realtime */
    6,  /* 01011000 Realtime */
    6,  /* 01011001 Realtime */
    6,  /* 01011010 Realtime */
    6,  /* 01011011 Realtime */
    6,  /* 01011100 Realtime */
    6,  /* 01011101 Realtime */
    6,  /* 01011110 Realtime */
    6,  /* 01011111 Realtime */
    6,  /* 01100000 Realtime */
    6,  /* 01100001 Realtime */
    6,  /* 01100010 Realtime */
    6,  /* 01100011 Realtime */
    6,  /* 01100100 Realtime */
    6,  /* 01100101 Realtime */
    6,  /* 01100110 Realtime */
    6,  /* 01100111 Realtime */
    6,  /* 01101000 Realtime */
    6,  /* 01101001 Realtime */
    6,  /* 01101010 Realtime */
    6,  /* 01101011 Realtime */
    6,  /* 01101100 Realtime */
    6,  /* 01101101 Realtime */
    6,  /* 01101110 Realtime */
    6,  /* 01101111 Realtime */
    6,  /* 01110000 Realtime */
    6,  /* 01110001 Realtime */
    6,  /* 01110010 Realtime */
    6,  /* 01110011 Realtime */
    6,  /* 01110100 Realtime */
    6,  /* 01110101 Realtime */
    6,  /* 01110110 Realtime */
    6,  /* 01110111 Realtime */
    6,  /* 01111000 Realtime */
    6,  /* 01111001 Realtime */
    6,  /* 01111010 Realtime */
    6,  /* 01111011 Realtime */
    6,  /* 01111100 Realtime */
    6,  /* 01111101 Realtime */
    6,  /* 01111110 Realtime */
    6   /* 01111111 Realtime */
};

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_add
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 将任务加入调度器
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_switch_add(const cmos_task_tcb_T *tcb)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_priority_T priority = cmos_priority_err;
    cmos_task_tcb_list_T *tcb_list = NULL;

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_switch_add with null tcb pointer.");
        return cmos_NULL_E;
    }

    /* step1: 获取该任务优先级*/
    priority = cmos_task_tcb_get_priority(tcb);

    /* step2: 获取该优先级链表 */
    tcb_list = cmos_task_switch_get_tcb_list_by_priority(priority); 
    if(NULL == tcb_list) /* tcb是该优先级头节点 */
    { 
        /* step3: 更新优先级索引 */
        s_priority_index |=  priority;
        cmos_task_switch_init_first_tcb(tcb);  /* step4/1: 加入到该优先级链表 */
        status = cmos_OK_E;
    }
    else
    {
        status = cmos_task_tcb_list_add(&tcb_list, tcb);     /* step4/2: 加入到该优先级链表 */
    }

    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_tcb_list_by_priority
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 获取priority对应的tcb表头
 *
 * 输入参数: priority 优先级
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static cmos_task_tcb_list_T *cmos_task_switch_get_tcb_list_by_priority(cmos_int32_T priority)
{ 
    cmos_int32_T highest_priority = 0;

    highest_priority = cmos_task_switch_get_highest_priority(priority);
    return s_ready_tcb[highest_priority];
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_set_tcb_list_by_priority
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 设置priority对应的tcb表头
 *
 * 输入参数: tcb 任务控制块指针
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static void cmos_task_switch_set_tcb_list_by_priority(cmos_int32_T priority, cmos_task_tcb_list_T *list)
{ 
    cmos_int32_T index = 0;
    /* 需要删除就绪任务列表时 list为null */
    /* if(NULL == list) */

    index = cmos_task_switch_get_highest_priority(priority);
    if((index < 0)
    || (index >= CMOS_PRIORITY_NUMS))
    {
        CMOS_ERR_STR("cmos_task_switch_set_tcb_list_by_priority with err priority.");
        return;
    }

    s_ready_tcb[index] = list;
    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_init_first_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 初始化该优先级tcb链表
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
static cmos_status_T cmos_task_switch_init_first_tcb(const cmos_task_tcb_T *tcb)
{
    cmos_priority_T priority = cmos_priority_err;
    cmos_task_tcb_list_node_T *ptr_tcb_list = NULL;

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_switch_init_first_tcb with null tcb pointer.");
        return cmos_NULL_E;
    }

    /* step1: 获取该任务优先级*/
    priority = cmos_task_tcb_get_priority(tcb);

    /* step2: 分配tcb_list结点 */
    /* TODO: 注意释放 */
    ptr_tcb_list = cmos_task_tcb_list_malloc_node(tcb);
    if(NULL == ptr_tcb_list)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_malloc_node failed.");
        return cmos_MEM_LACK_E;
    }

    /* step3: 存入任务链表数组 */
    cmos_task_switch_set_tcb_list_by_priority(priority, ptr_tcb_list);

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_highest_priority
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 通过优先级获取位图索引
 *
 * 输入参数: priority 优先级
 * 输出参数: 无
 *
 * 返回值  : 最高优先级
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static cmos_int32_T cmos_task_switch_get_highest_priority(cmos_int32_T priority)
{
    return s_priority_bitmap[priority];
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_switch
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 任务切换算法
 *
 * 输入参数: 当前任务psp
 * 输出参数: 无
 *
 * 返回值  : 最高优先级psp
 * 调用关系: 无
 * 其 它   : 所有调用的函数都是O(1)算法
 *
 ******************************************************************************/
cmos_task_tcb_psp_T cmos_task_switch_switch(const cmos_task_tcb_psp_T cur_psp)
{
    cmos_task_tcb_psp_T next_psp = NULL;
    cmos_task_tcb_T *next_tcb = NULL;

#if (CMOS_DEBUG_LEVEL > 0) 
    if(NULL == cur_psp)
    {
        cmos_debug_log("cmos_task_switch_switch get a null pointer.");
        return NULL;
    }
#endif
    
    /* step1: 保存将切出任务的psp到s_running_tcb */
    cmos_task_tcb_set_psp(s_running_tcb, cur_psp);

    /* step2: 获取切入任务tcb */
    next_tcb = cmos_task_switch_get_highest_tcb(); 
#if (CMOS_DEBUG_LEVEL > 0) 
    if(NULL == next_tcb)
    {
        CMOS_ERR_STR("cmos_task_switch_switch get a null pointer.");
        return NULL;
    }
#endif

    /* step3: 更新s_running_tcb */
    cmos_task_switch_set_running_tcb(next_tcb);

    /* step4: 获取最高优先级任务psp */
    next_psp = cmos_task_tcb_get_psp(next_tcb);
#if (CMOS_DEBUG_LEVEL > 0) 
    if(NULL == next_psp)
    {
        CMOS_ERR_STR("cmos_task_switch_switch get a null pointer.");
        return NULL;
    }
#endif

    return next_psp;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_set_running_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151121 
 * 函数功能: 保存当前任务tcb
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline static void cmos_task_switch_set_running_tcb(const cmos_task_tcb_T *tcb)
{
    s_running_tcb = (cmos_task_tcb_T *)tcb;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_highest_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151120 
 * 函数功能: 获取最高优先级任务tcb
 *
 * 输入参数: 无
 * 输出参数: 最高优先级任务tcb
 *
 * 返回值  : 最高优先级psp
 * 调用关系: 无
 * 其 它   : O(1)算法
 *
 ******************************************************************************/ 
static cmos_task_tcb_T *cmos_task_switch_get_highest_tcb(void)
{ 
    cmos_task_tcb_list_T *tcb_list = NULL;
    cmos_task_tcb_T *tcb = NULL;

    tcb_list = cmos_task_switch_get_tcb_list_by_priority(s_priority_index); 
    tcb = cmos_task_tcb_list_get_head_tcb(tcb_list);

    return tcb;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_start
 * 负责人  : 彭鹏
 * 创建日期：20151121 
 * 函数功能: 启动内核
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 无
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_switch_start(void)
{ 
    cmos_task_tcb_T *highest_tcb = NULL;
    cmos_task_tcb_psp_T highest_psp = NULL;

    /* step1: 获取idle任务tcb */
    highest_tcb = cmos_task_switch_get_highest_tcb(); 

    /* step2: 更新s_running_tcb */
    cmos_task_switch_set_running_tcb(highest_tcb);

    /* step3: 将idle psp存入r0 */
    highest_psp = cmos_task_tcb_get_psp(highest_tcb);

    /* step4: 启动systick 内部将重新设置 systick优先级 */ 
    cmos_hal_cortex_cortex_systick_start(CMOS_TICK_TIMES);

    /* step5: 使能调度器 */ 
    cmos_hal_cortex_cortex_enable_switch();

    /* step6: 进入非特权级别 */
    cmos_hal_cortex_cortex_goto_unprivileged();	

    /* step7: 调用任务切换后半部分 */
    void cmos_task_switch_start_s(cmos_task_tcb_psp_T psp); /* pendsv.s中定义 */
    cmos_task_switch_start_s(highest_psp);

    /* 不会运行到此 */
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_delay
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: 延迟当前任务
 *
 * 输入参数: 延迟时间(CMOS_TICK_TIMES)数
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : CMOS_TICK_TIMES一般以ms为单位 该函数延迟任务millisec毫秒
 *
 ******************************************************************************/
cmos_status_T cmos_task_switch_delay(cmos_int32_T millisec)
{ 
    cmos_status_T status = cmos_ERR_E;
    cmos_int32_T index = 0;
    cmos_int32_T highest_priority = 0;
    cmos_task_tcb_list_T *tcb_list = NULL;
    if(millisec <=0 )
    {
        return cmos_PARA_E;
    }

    /* step1: 将当前任务从就绪列表移入 delay链表 */
    status = cmos_task_tcb_list_add(&s_delay_tcb_list, s_running_tcb);
    if(cmos_OK_E != status)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_add failed.");
        return status;
    }

    /* step3: 从就绪表删除当前任务 */
    index = cmos_task_switch_get_highest_priority(s_priority_index);
    status = cmos_task_tcb_list_del(&s_ready_tcb[index], s_running_tcb);
    if(cmos_OK_E != status)
    {
        CMOS_ERR_STR("cmos_task_tcb_list_del failed.");
        return status;
    } 

    /* step4: 更新 s_priority_index */
    if(NULL == tcb_list) /* 该优先级已无就绪任务 */
    {
        /* 将最高优先级置零 */
        highest_priority = (0x1 << index);
        s_priority_index &= ~highest_priority;
    }

    /* step5: 设置millisec */ 
    cmos_task_tcb_set_delay_ms(s_running_tcb, millisec);

    /* step6: 开始调度 */ 
    cmos_hal_cortex_cortex_set_pendsv();

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_update_time
 * 负责人  : 彭鹏
 * 创建日期：20151123 
 * 函数功能: 更新tcb的时间
 *           时间减为零的重新加入s_ready_tcb
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_task_switch_update_tcb_time(void)
{
    cmos_int32_T highest_priority = 0;

#if 1
    /* 需要任务切换(被系统调用抢占) 先不处理时间问题 */
    if(s_running_tcb != cmos_task_switch_get_highest_tcb())
    {
        return cmos_OK_E;
    }
    else
    {
        /* step1: 自减当前任务的时间片 */
        cmos_task_tcb_dec_tick(s_running_tcb); 
        /* 移动如队列尾 */
        if(cmos_task_tcb_zero_tick(s_running_tcb)) /* step2: 移如就绪表尾 */
        {
            cmos_task_tcb_reset_tick(s_running_tcb);
            highest_priority = cmos_task_switch_get_highest_priority(s_priority_index);
            cmos_task_tcb_list_head_move_to_tail(&s_ready_tcb[highest_priority]);
        }
    }
#endif

    /* step2: 处理s_delay_tcb_list(等待)链表的任务 所有的delay_ms域自减 并进行可能的就绪表更新 */
    if(NULL != s_delay_tcb_list)
    {
        cmos_task_tcb_list_walk(s_delay_tcb_list, cmos_task_switch_update_delay_tcb, NULL);
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_update_delay_tcb
 * 负责人  : 彭鹏
 * 创建日期：20151124 
 * 函数功能: 逐个处理s_delay_tcb_list中的tcb
 *
 * 输入参数: tcb s_delay_tcb_list中的tcb
 *           para s_ready_tcb
 * 输出参数: 无
 *
 * 返回值  : 执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static void cmos_task_switch_update_delay_tcb(cmos_task_tcb_T *tcb, void *para)
{ 
    if((NULL == tcb)
    || (NULL != para))
    {
        CMOS_ERR_STR("cmos_task_switch_update_delay_tcb para err.");
        return;
    }

    cmos_status_T status = cmos_ERR_E;
    cmos_task_tcb_dec_delay_ms(tcb); 

    /* 定时到 从s_delay_tcb_list中删除 加入s_ready_tcb */
    if(cmos_task_tcb_zero_delay_ms(tcb))
    { 
        status = cmos_task_tcb_list_del(&s_delay_tcb_list, tcb);
        if(cmos_OK_E != status)
        {
            CMOS_ERR_STR("cmos_task_switch_update_delay_tcb del tcb failed.");
            return;
        } 
        
        status = cmos_task_switch_add(tcb);
        if(cmos_OK_E != status)
        {
            CMOS_ERR_STR("cmos_task_switch_update_delay_tcb del tcb failed.");
            return;
        } 
    }
} 
#endif

