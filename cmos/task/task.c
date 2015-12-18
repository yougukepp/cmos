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
#include "ready.h"
#include "running.h"
#include "blocked.h"
#include "cortex.h"
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
    cmos_task_pool_ready_add(tcb);

    *task_id = (cmos_task_id_T)tcb;
    return cmos_OK_E;

err: 
    *task_id = NULL;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_delay
 * 负责人  : 彭鹏
 * 创建日期：20151213 
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
cmos_status_T cmos_task_delay(cmos_int32_T millisec)
{
    cmos_task_tcb_T *tcb = NULL;

    /* step1: 弹出就绪表 */
    tcb = cmos_task_pool_ready_pop_tcb();
    if(NULL == tcb)
    {
        CMOS_ERR_STR("cmos_task_delay get a null tcb!");
        return cmos_NULL_E;
    }

    /* step2: 加入阻塞表 */ 
    cmos_task_pool_blocked_add(tcb, cmos_blocked_delay_E);

    /* step2: 设置当前任务的延迟值 */
    cmos_task_pool_running_set_delay(millisec);

    /* step3: 调用任务切换 */ 
    cmos_hal_cortex_cortex_set_pendsv();

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_tick_callback
 * 负责人  : 彭鹏
 * 创建日期：20151213 
 * 函数功能: 时钟心跳回调此函数
 *           此函数中需要更新系统中所有任务时间相关的字段
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
void cmos_task_tick_callback(void)
{
    /* step1: 处理当前任务 */
    cmos_task_pool_running_update_tick();

    /* step2: 处理阻塞任务 */
    cmos_task_pool_blocked_update_tick();

    /* step3: 处理就绪任务 */
    cmos_task_pool_ready_update_tick();

    return;
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_self
 * 负责人  : 彭鹏
 * 创建日期：20151217 
 * 函数功能: 获取当前任务id(tcb指针)
 *
 * 输入参数: 无
 * 输出参数: 无
 * 返回值  : 当前任务id
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
inline cmos_task_tcb_T *cmos_task_self(void)
{
    return cmos_task_pool_running_get_tcb();
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_suspend
 * 负责人  : 彭鹏
 * 创建日期：20151217 
 * 函数功能: 挂起tcb指向的任务
 *
 * 输入参数: tcb 待挂起的任务tcb
 * 输出参数: 无
 *
 * 返回值  : 无
 *
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_suspend(cmos_task_tcb_T *tcb)
{
    /* step 1: 从就绪表中移出tcb */
    cmos_task_pool_ready_del(tcb);

    /* step 2: 将tcb移入阻塞表 */
    cmos_task_pool_blocked_add(tcb, cmos_blocked_suspend_E);

    /* step 3: 调度 */
    cmos_hal_cortex_cortex_set_pendsv();
}

/*******************************************************************************
 *
 * 函数名  : cmos_task_resume
 * 负责人  : 彭鹏
 * 创建日期：20151217 
 * 函数功能: 恢复tcb指向的任务
 *
 * 输入参数: tcb 待恢复的任务tcb
 * 输出参数: 无
 *
 * 返回值  : 无
 *
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
void cmos_task_resume(cmos_task_tcb_T *tcb)
{
    /* step 1: 将tcb移出阻塞表 */
    cmos_task_pool_blocked_del(tcb, cmos_blocked_suspend_E);

    /* step 2: 将tcb移入就绪表 */
    cmos_task_pool_ready_add(tcb);

    /* step 3: 调度 */
    cmos_hal_cortex_cortex_set_pendsv();
}

