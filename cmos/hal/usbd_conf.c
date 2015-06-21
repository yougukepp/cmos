/******************************************************************************
 *
 * 文件名  ： usb_conf.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150617 
 * 版本号  ： 1.0
 * 文件描述： stm32 usb 设备库 与PCD的接口
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"
#include "usbd_conf.h"
#include "console.h"
#include "usbd_core.h"


/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
PCD_HandleTypeDef g_pcd_handle;

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/********************* 底层回调(PCD -> USB Device Library) *********************/
/* TODO: 完善注释 */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *g_pcd_handle)
{
    USBD_LL_SetupStage(g_pcd_handle->pData, (uint8_t *)g_pcd_handle->Setup);
}

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *g_pcd_handle, uint8_t epnum)
{
    USBD_LL_DataOutStage(g_pcd_handle->pData, epnum, g_pcd_handle->OUT_ep[epnum].xfer_buff);
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *g_pcd_handle, uint8_t epnum)
{
    USBD_LL_DataInStage(g_pcd_handle->pData, epnum, g_pcd_handle->IN_ep[epnum].xfer_buff);
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *g_pcd_handle)
{
    USBD_LL_SOF(g_pcd_handle->pData);
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *g_pcd_handle)
{ 
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

    /* Set USB Current Speed */
    switch(g_pcd_handle->Init.speed)
    {
        case PCD_SPEED_HIGH:
            speed = USBD_SPEED_HIGH;
            break;

        case PCD_SPEED_FULL:
            speed = USBD_SPEED_FULL;
            break;

        default:
            speed = USBD_SPEED_FULL;
            break;
    }
    USBD_LL_SetSpeed(g_pcd_handle->pData, speed);

    /* Reset Device */
    USBD_LL_Reset(g_pcd_handle->pData);
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *g_pcd_handle)
{
    USBD_LL_Suspend(g_pcd_handle->pData);
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *g_pcd_handle)
{
    USBD_LL_Resume(g_pcd_handle->pData);
}

void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *g_pcd_handle, uint8_t epnum)
{
    USBD_LL_IsoOUTIncomplete(g_pcd_handle->pData, epnum);
}

void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *g_pcd_handle, uint8_t epnum)
{
    USBD_LL_IsoINIncomplete(g_pcd_handle->pData, epnum);
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *g_pcd_handle)
{
    USBD_LL_DevConnected(g_pcd_handle->pData);
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *g_pcd_handle)
{
    USBD_LL_DevDisconnected(g_pcd_handle->pData);
}

/******************** 底层回调(USB Device Library --> PCD) *********************/
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
{

    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    /* Set LL Driver parameters */
    g_pcd_handle.Instance = USB_OTG_HS;
    g_pcd_handle.Init.dev_endpoints = 6;
    g_pcd_handle.Init.use_dedicated_ep1 = 0;
    g_pcd_handle.Init.ep0_mps = 0x40;

    /* Be aware that enabling DMA mode will result in data being sent only by
     * multiple of 4 packet sizes. This is due to the fact that USB DMA does
     * not allow sending data from non word-aligned addresses.
     * For this specific application, it is advised to not enable this option
     * unless required. */
    g_pcd_handle.Init.dma_enable = 0;

    g_pcd_handle.Init.low_power_enable = 0;

    g_pcd_handle.Init.phy_itface = PCD_PHY_EMBEDDED; 
    g_pcd_handle.Init.Sof_enable = 0;
    g_pcd_handle.Init.speed = PCD_SPEED_HIGH;
    g_pcd_handle.Init.vbus_sensing_enable = 1;
    /* Link The driver to the stack */
    g_pcd_handle.pData = pdev;
    pdev->pData = &g_pcd_handle;
    /* Initialize LL Driver */
    HAL_PCD_Init(&g_pcd_handle);

    HAL_PCDEx_SetRxFiFo(&g_pcd_handle, 0x200);
    HAL_PCDEx_SetTxFiFo(&g_pcd_handle, 0, 0x80);
    HAL_PCDEx_SetTxFiFo(&g_pcd_handle, 1, 0x174);

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_DeInit(pdev->pData);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_Start(pdev->pData);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_Stop(pdev->pData);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev,
        uint8_t ep_addr,
        uint8_t ep_type,
        uint16_t ep_mps)
{

    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_Open(pdev->pData,
            ep_addr,
            ep_mps,
            ep_type);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);

    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_Close(pdev->pData, ep_addr);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_Flush(pdev->pData, ep_addr);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_SetStall(pdev->pData, ep_addr);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_ClrStall(pdev->pData, ep_addr);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK; 
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    cmos_uint8_T is_stall = 0;
    PCD_HandleTypeDef *g_pcd_handle = pdev->pData;

    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);

    if((ep_addr & 0x80) == 0x80)
    {
        is_stall = g_pcd_handle->IN_ep[ep_addr & 0x7F].is_stall;
    }
    else
    {
        is_stall = g_pcd_handle->OUT_ep[ep_addr & 0x7F].is_stall;
    }
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);

    return is_stall;

}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_SetAddress(pdev->pData, dev_addr);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK; 
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, 
        uint8_t ep_addr,
        uint8_t *pbuf,
        uint16_t size)
{
    cmos_int32_T i = 0;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size);

    cmos_debug_log("%s,data:0x%02x, %d: ", __func__, ep_addr, size);
    for(i=0;i<size;i++)
    {
        cmos_debug_log("0x%02x,", pbuf[i]);
    }
    cmos_debug_log("\r\n");

    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, 
        uint8_t ep_addr,
        uint8_t *pbuf,
        uint16_t size)
{
    cmos_int32_T i = 0;
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size);

    cmos_debug_log("%s,data:0x%02x, %d: ", __func__, ep_addr, size);
    for(i=0;i<size;i++)
    {
        cmos_debug_log("0x%02x,", pbuf[i]);
    }
    cmos_debug_log("\r\n");
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
    return USBD_OK;
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    cmos_uint32_T rx_data_size = 0;

    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    rx_data_size = HAL_PCD_EP_GetRxCount(pdev->pData, ep_addr);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);

    return rx_data_size;
}

void USBD_LL_Delay(uint32_t Delay)
{
    cmos_trace_log("IN %s,%d,%s", __FILE__, __LINE__, __func__);
    HAL_Delay(Delay);
    cmos_trace_log("OUT %s,%d,%s", __FILE__, __LINE__, __func__);
}

