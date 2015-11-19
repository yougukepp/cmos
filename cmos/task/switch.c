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


/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static cmos_task_tcb_list_node_T *cmos_task_switch_get_tcb_list_head(cmos_priority_T priority);
static void cmos_task_switch_init_first_tcb(const cmos_task_tcb_T *tcb);

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
static cmos_task_tcb_list_node_T *s_tcb_table_by_priority[CMOS_PRIORITY_NUMS] = {NULL};

/*******************************************************************************
 *
 * 每个位置表示该s_priority_index对应的系统最高优先级
 * tools/generate_bitmap.py生成
 *
 ******************************************************************************/


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
    cmos_priority_T priority = cmos_priority_err;
    cmos_task_tcb_list_node_T *head = NULL;

    /* step1: 获取该任务优先级*/
    priority = cmos_task_tcb_get_priority(tcb);

    /* step2: 获取该优先级链表 */
    head = cmos_task_switch_get_tcb_list_head(priority); 
    

    if(NULL == head) /* tcb是该优先级头节点 */
    { 
        /* step3: 更新优先级索引 */
        s_priority_index |=  priority;
        cmos_task_switch_init_first_tcb(tcb);  /* step4/1: 加入到该优先级链表 */
    }
    else
    {
        cmos_task_tcb_list_add(head, tcb);     /* step4/2: 加入到该优先级链表 */
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_switch_get_tcb_list_head
 * 负责人  : 彭鹏
 * 创建日期：20151119 
 * 函数功能: 获取本优先级tcb列表头
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
static cmos_task_tcb_list_node_T *cmos_task_switch_get_tcb_list_head(cmos_priority_T priority)
{
    return NULL;
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
static void cmos_task_switch_init_first_tcb(const cmos_task_tcb_T *tcb)
{
    return;
}

