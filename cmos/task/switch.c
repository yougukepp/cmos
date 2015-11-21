/******************************************************************************
 *
 * 文件名  ： switch.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 任务调度模块
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 算法说明:
 *            1. 需要一个优先级索引
 *            2. 位图表
 *            3. 每个优先级一个tcb链表
 *            TODO: s_打头的变量封装到statis读写函数
 *
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "tcb.h"
#include "tcb_list.h"
#include "switch.h"
#include "console.h"
#include "cortex.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_task_tcb_list_T *cmos_task_switch_get_tcb_list_head(cmos_int32_T priority);
static cmos_status_T cmos_task_switch_init_first_tcb(const cmos_task_tcb_T *tcb);
static void cmos_task_switch_set_tcb_list_head(cmos_priority_T priority, cmos_task_tcb_list_T *list);
static cmos_int32_T cmos_task_switch_get_highest_priority(cmos_uint8_T priority);
static cmos_task_tcb_T *cmos_task_switch_get_highest_tcb(void);
static void cmos_task_switch_set_running_tcb(const cmos_task_tcb_T *tcb);

/********************************** 变量实现区 *********************************/
/* 当前运行任务的tcb 用于任务恢复 */
cmos_task_tcb_T *s_running_tcb = NULL;

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
static cmos_uint8_T s_priority_index = 0x00;

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
static cmos_task_tcb_list_T *s_tcb_table_by_priority[CMOS_PRIORITY_NUMS] = {NULL};

/*******************************************************************************
 *
 * 每个位置表示该s_priority_index对应的系统最高优先级
 * tools/generate_bitmap.py生成
 *
 * 每行的内容依次为: 最高优先级(即s_tcb_table_by_priority索引) s_priority_index值 cmos_priority_T值
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
    cmos_task_tcb_list_T *head = NULL;

    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_switch_add with null tcb pointer.");
        return cmos_NULL_E;
    }

    /* step1: 获取该任务优先级*/
    priority = cmos_task_tcb_get_priority(tcb);

    /* step2: 获取该优先级链表 */
    head = cmos_task_switch_get_tcb_list_head(priority); 
    if(NULL == head) /* tcb是该优先级头节点 */
    { 
        /* step3: 更新优先级索引 */
        s_priority_index |=  priority;
        cmos_task_switch_init_first_tcb(tcb);  /* step4/1: 加入到该优先级链表 */
        status = cmos_OK_E;
    }
    else
    {
        status = cmos_task_tcb_list_add(head, tcb);     /* step4/2: 加入到该优先级链表 */
    }

    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_tcb_list_head
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
static cmos_task_tcb_list_T *cmos_task_switch_get_tcb_list_head(cmos_int32_T priority)
{ 
    cmos_int32_T highest_priority = 0;

    highest_priority = cmos_task_switch_get_highest_priority(priority);
    return s_tcb_table_by_priority[highest_priority];
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_tcb_list_head
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
static void cmos_task_switch_set_tcb_list_head(cmos_priority_T priority, cmos_task_tcb_list_T *list)
{ 
    cmos_int32_T index = 0;
    if(NULL == list)
    {
        CMOS_ERR_STR("cmos_task_switch_set_tcb_list_head with null list.");
        return;
    }

    index = cmos_task_switch_get_highest_priority(priority);
    if((index < 0)
    || (index >= CMOS_PRIORITY_NUMS)
    || (NULL != s_tcb_table_by_priority[index]))
    {
        CMOS_ERR_STR("cmos_task_switch_set_tcb_list_head with err priority.");
        return;
    }

    s_tcb_table_by_priority[index] = list;
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
    cmos_task_switch_set_tcb_list_head(priority, ptr_tcb_list);

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
static cmos_int32_T cmos_task_switch_get_highest_priority(cmos_uint8_T priority)
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
    if(NULL == next_psp)
    {
        CMOS_ERR_STR("cmos_task_switch_switch get a null pointer.");
        return NULL;
    }

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
static void cmos_task_switch_set_running_tcb(const cmos_task_tcb_T *tcb)
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
    cmos_task_tcb_list_T *head = NULL;
    cmos_task_tcb_T *tcb = NULL;

    head = cmos_task_switch_get_tcb_list_head(s_priority_index); 
    tcb = cmos_task_tcb_list_get_head_tcb(head);

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
    cmos_task_tcb_T *idle_tcb = NULL;
    cmos_task_tcb_psp_T idle_psp = NULL;

    /* step1: 获取idle任务tcb */
    idle_tcb = cmos_task_switch_get_highest_tcb(); 

    /* step2: 更新s_running_tcb */
    cmos_task_switch_set_running_tcb(idle_tcb);

    /* step3: 将idle psp存入r0 */
    idle_psp = cmos_task_tcb_get_psp(idle_tcb);

    /* step4: 调用任务切换后半部分 */
    void cmos_task_switch_start_s(cmos_task_tcb_psp_T psp); /* pendsv.s中定义 */
    cmos_task_switch_start_s(idle_psp);

    /* 不会运行到此 */
}

