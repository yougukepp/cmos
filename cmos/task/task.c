/******************************************************************************
 *
 * 文件名  ： task.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151117 
 * 版本号  ： v1.1
 * 文件描述： 任务实现
 * 版权说明： Copyright (c) 2000-2020   烽火通信科技股份有限公司
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "stm32f429idiscovery_hardware.h"
#include "tcb.h"
#include "switch.h"
#include "task.h"
#include "mem.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* 空闲的任务栈顶 */
static cmos_task_tcb_psp_T s_user_stack_base = CMOS_TASK_STACK_BASE;

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_task_create
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
cmos_status_T cmos_task_create(cmos_task_id_T *task_id, 
        const cmos_task_attribute_T *task_attribute)
{
    cmos_int32_T stack_size = 0;
    cmos_task_tcb_T *tcb = NULL;
    cmos_status_T status = cmos_ERR_E;

    /* step0: 参数检查 */
    if((NULL == task_id)
    || (NULL == task_attribute))
    {
        CMOS_ERR_STR("task id and task attribute should not to be null.");
        status = cmos_PARA_E;
        goto err;
    }

    /* TODO: 任务删除的时候释放 */
    /* step1: 分配 tcb(任务控制块) */
    tcb = cmos_malloc(sizeof(cmos_task_tcb_T));
    if(NULL == tcb)
    {
        status = cmos_MEM_LACK_E;
        goto err;
    }

    /* step2: 初始化任务tcb */
    status = cmos_task_tcb_init(tcb, task_attribute, s_user_stack_base);
    if(cmos_OK_E != status)
    {
        goto err;
    }

    /* step3: 更新用户空间未用栈的顶 */
    stack_size = cmos_task_tcb_get_stack_size(tcb);

    stack_size >>= 2; /* 字节转为字 */
    s_user_stack_base -= stack_size;

    /* step4: 通知调度模块有新线程 */ 
    status = cmos_task_state_to_ready(tcb);
    if(cmos_OK_E != status)
    {
        goto err;
    }

    *task_id = (cmos_task_id_T)tcb;
    return cmos_OK_E;

err: 
    *task_id = NULL;
    return status;
}

