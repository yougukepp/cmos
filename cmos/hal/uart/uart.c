/******************************************************************************
 *
 * 文件名  ： uart.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150614 
 * 版本号  ： 1.0
 * 文件描述： uart cmos hal,用于支持cmos控制台模块
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "uart.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f429idiscovery_hardware.h"
#include "console.h"
#include "task.h"
#include "mutex.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static void *uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode);
static cmos_int32_T uart_read(const void *dev_id, void *buf, cmos_int32_T n_bytes);
static cmos_int32_T uart_write(const void *dev_id, const void *buf, cmos_int32_T n_bytes);
static cmos_status_T uart_ioctl(const void *dev_id, cmos_uint32_T request, cmos_uint32_T mode);
static cmos_status_T uart_close(const void *dev_id);

/* 驱动变量 加入到vfs */
const cmos_hal_driver_T g_uart_driver = {
    .open = uart_open,
    .read = uart_read,
    .write = uart_write,
    .ioctl = uart_ioctl,
    .close = uart_close
};

/* STM32F4Cube HAL驱动 */
static UART_HandleTypeDef s_uart_handle;

/* 写互斥锁 */
cmos_ipc_mutex_T *s_mutex_write = NULL;
cmos_task_id_T s_write_task_id = 0;

/* 读互斥锁 */
cmos_ipc_mutex_T *s_mutex_read = NULL;
cmos_task_id_T s_read_task_id = 0;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : cmos_hal_uart_init
* 负责人  : 彭鹏
* 创建日期: 20151109
* 函数功能: cmos hal uart初始化
*
* 输入参数: 波特率和串口号
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: hal_init中和其他硬件的初始化一起调
* 其 它   : 无
*
******************************************************************************/
void cmos_hal_uart_init(void *para)
{ 
    const cmos_hal_uart_init_para_T *init_para = para;

    /* 申请读写互斥量 */
    s_mutex_write = cmos_ipc_mutex_malloc();
    if(NULL == s_mutex_write)
    {
        CMOS_ERR_STR("cmos_ipc_mutex_malloc failed.");
        return;
    }

    s_mutex_read = cmos_ipc_mutex_malloc();
    if(NULL == s_mutex_write)
    {
        CMOS_ERR_STR("cmos_ipc_mutex_malloc failed.");
        return;
    }

    /* 仅实现一个串口 */
    switch(init_para->uart_index)
    {
        case 1:
            {
                s_uart_handle.Instance = USART1;
                break;
            }
        default:
            {
                cmos_err_log("mcu not support %d hardware uart.", init_para->uart_index);
                return;
            }
    }

    s_uart_handle.Init.BaudRate     = init_para->baud_rate;
    s_uart_handle.Init.WordLength   = UART_WORDLENGTH_8B;
    s_uart_handle.Init.StopBits     = UART_STOPBITS_1;
    s_uart_handle.Init.Parity       = UART_PARITY_NONE;
    s_uart_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    s_uart_handle.Init.Mode         = UART_MODE_TX_RX;
    s_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	
    if(HAL_UART_Init(&s_uart_handle) != HAL_OK)
    {
        CMOS_ERR_STR("HAL_UART_Init failed.");
        return;
    }

    return;
}

/* TODO:
   1 实现 参数 管理 */
static void *uart_open(const cmos_uint8_T *path, cmos_uint32_T flag, cmos_uint32_T mode)
{
    /* 由path返回相应的 s_uart_handle */
    return (void *)(&s_uart_handle);
}

static cmos_status_T uart_close(const void *dev_id)
{
    return cmos_ERR_E;
}

static cmos_int32_T uart_read(const void *dev_id, void *buf, cmos_int32_T n_bytes)
{
    return 0;
}

static cmos_int32_T uart_write(const void *dev_id, const void *buf, cmos_int32_T n_bytes)
{ 
#if 1
    /* 轮询 发送 */
    if(HAL_UART_Transmit((UART_HandleTypeDef *)dev_id, (uint8_t*)buf, n_bytes, n_bytes/CMOS_UART_TIMEOUT_DIV)!= HAL_OK)
    {
        assert_failed(__FILE__, __LINE__);
        return 0;
    }	
#else
    /* 1、锁定uart输出 避免多次访问 */ 
    cmos_ipc_mutex_lock(s_mutex_write);

    /* 2、中断 发送 */
    if(HAL_UART_Transmit_IT((UART_HandleTypeDef *)dev_id, (uint8_t *)buf, n_bytes)!= HAL_OK)
    {
        assert_failed(__FILE__, __LINE__);
        return 0;
    }

    /* 3、阻塞 等待传输完成(HAL_UART_TxCpltCallback通知) */
    s_write_task_id = cmos_task_self();
    cmos_task_suspend(s_write_task_id);
#endif

    return n_bytes;
}

static cmos_status_T uart_ioctl(const void *dev_id, cmos_uint32_T request, cmos_uint32_T para)
{
    return cmos_ERR_E;
}

void UART1_IRQHandler(void)
{ 
    /* CONSOLE 使用的串口1 */
    HAL_UART_IRQHandler(&s_uart_handle);
}

/* 串口传输完成回调 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 1、恢复发送任务 */
    cmos_task_resume(s_write_task_id);

    /* 2、解锁发送功能 */
    cmos_ipc_mutex_unlock(s_mutex_write);
}

