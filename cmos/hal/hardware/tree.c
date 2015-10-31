/******************************************************************************
 *
 * 文件名  ： tree.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151030
 * 版本号  ： v1.0
 * 文件描述： 硬件设备树的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdlib.h>
#include <string.h>

#include "cmos_config.h"
#include "tree.h"

#ifdef _X64_PC_
#include "pc.h"
#endif

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* FIXME: 操作设备树需要带锁 */
cmos_hal_hardware_tree_node_T *s_cmos_hardware_tree = NULL; /* 设备树 */

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/
static cmos_status_T node_init(cmos_hal_hardware_tree_node_T *ptr_node,
        const cmos_uint8_T *ptr_name,
        const cmos_int32_T *ptr_value,
        const cmos_hal_hardware_tree_node_T *ptr_first_sun,
        const cmos_hal_hardware_tree_node_T *ptr_next_brother);
static cmos_status_T node_uninit(cmos_hal_hardware_tree_node_T *ptr_node);
static cmos_status_T walk(cmos_hal_hardware_tree_node_T *ptr_tree, cmos_hal_hardware_tree_node_func_T func);
static cmos_status_T print_node(const cmos_hal_hardware_tree_node_T *ptr_node);
static cmos_status_T parse_tree_path(cmos_uint8_T *dev_type, cmos_uint8_T *dev_name, const cmos_uint8_T *path);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : cmos_hal_hardware_tree_print
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 打印设备树
 *
 * 输入参数: 无
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_hardware_tree_print(void)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_hal_hardware_tree_node_func_T func = NULL;

    func = (cmos_hal_hardware_tree_node_func_T)print_node;
    /* 遍历树并对每个结点做指定操作 */
    status = walk(s_cmos_hardware_tree, func);
    if(cmos_OK_E != status)
    { 
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_hardware_tree_init
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 初始化设备树
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_hardware_tree_init(void)
{
    cmos_status_T status = cmos_ERR_E;

    s_cmos_hardware_tree = (cmos_hal_hardware_tree_node_T *)malloc(sizeof(cmos_hal_hardware_tree_node_T));
    status = node_init(s_cmos_hardware_tree, (const cmos_uint8_T *)CMOS_HAL_HARDWARE_TREE_ROOT_NAME, NULL, NULL ,NULL);
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_hardware_tree_add
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 设备树中加入设备
 *
 * 输入参数: path 设备的路径
 *
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_hardware_tree_add(const cmos_uint8_T *path)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_uint8_T dev_type_buf[CMOS_HAL_HARDWARE_TREE_NAME_SIZE] = {0};
    cmos_uint8_T dev_name_buf[CMOS_HAL_HARDWARE_TREE_NAME_SIZE] = {0};

    /* TODO:参数检查 */
    if(NULL == path)
    {
        return cmos_PARA_E;
    }

    /* 解析设备类型和设备名 */
    status = parse_tree_path(dev_type_buf, dev_name_buf, path);
    if(cmos_OK_E != status)
    {
        return status;
    }

    /* TODO: 继续工作 */
#ifdef _X64_PC_
    printf("dev_type:%s\n", dev_type_buf);
    printf("dev_name:%s\n", dev_name_buf);
#endif

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_hardware_tree_del
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 设备树中删除设备
 *
 * 输入参数: path 设备的路径
 *
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_hardware_tree_del(const cmos_uint8_T *path)
{
    cmos_status_T status = cmos_ERR_E;
    /* TODO:参数检查 */

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : cmos_hal_hardware_tree_uninit
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 解初始化设备树
 *
 * 输入参数: 无
 *
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
cmos_status_T cmos_hal_hardware_tree_uninit(void)
{
    cmos_status_T status = cmos_ERR_E;

    status = node_uninit(s_cmos_hardware_tree);
    if(cmos_OK_E != status)
    {
        status = cmos_MEM_LACK_E;
        return status;
    }

    free(s_cmos_hardware_tree);
    s_cmos_hardware_tree = NULL;
    status = cmos_OK_E;

    return status;
}

/*******************************************************************************
 *
 * 函数名  : node_init
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 初始化设备树结点
 *
 * 输入参数: ptr_name 名字
 *           ptr_value 值
 *           ptr_first_sun 首子结点
 *           ptr_name 下一兄弟结点
 *
 * 输出参数: ptr_node 完成初始化的结点
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T node_init(cmos_hal_hardware_tree_node_T *ptr_node,
        const cmos_uint8_T *ptr_name,
        const cmos_int32_T *ptr_value,
        const cmos_hal_hardware_tree_node_T *ptr_first_sun,
        const cmos_hal_hardware_tree_node_T *ptr_next_brother)
{
    cmos_status_T status = cmos_ERR_E;
    cmos_uint8_T *name_buf = NULL;
    cmos_uint32_T name_len = 0;

    /* ptr_first_sun ptr_next_brother 可能为空 */
    if((NULL == ptr_node)
    || (NULL == ptr_name))
    {
        return cmos_PARA_E;
    }

    name_len = strlen((const char *)ptr_name);
    name_buf = (cmos_uint8_T *)malloc(name_len + 1); /* + NUL */
    if(NULL == name_buf)
    {
        status = cmos_MEM_LACK_E;
        return status;
    }
    strcat((char *)name_buf, (char *)ptr_name);

    ptr_node->ptr_name = name_buf;
    ptr_node->ptr_value = (cmos_int32_T *)ptr_value;
    ptr_node->ptr_first_sun = (cmos_hal_hardware_tree_node_T *)ptr_first_sun;
    ptr_node->ptr_next_brother = (cmos_hal_hardware_tree_node_T *)ptr_next_brother;

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : node_uninit
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 析构设备树结点
 *
 * 输入参数: ptr_node 完成析构的结点
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T node_uninit(cmos_hal_hardware_tree_node_T *ptr_node)
{
    cmos_status_T status = cmos_ERR_E;
    /* TODO: 参数检查 */

    status = cmos_OK_E;
    return status;
}

/*******************************************************************************
 *
 * 函数名  : walk
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 遍历树
 *
 * 输入参数: func 针对每个结点的操作(函数指针)
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T walk(cmos_hal_hardware_tree_node_T *ptr_tree, cmos_hal_hardware_tree_node_func_T func)
{
    cmos_status_T status = cmos_ERR_E; 
    cmos_hal_hardware_tree_node_T *ptr_go = NULL;

    if((NULL == ptr_tree)
    || (NULL == func))
    {
        return cmos_PARA_E;
    }

    ptr_go = ptr_tree;
    /* 1 自己 */
    status = func(ptr_go);
    if(cmos_OK_E != status)
    {
        assert_failed(__FILE__, __LINE__);
        return status;
    }

    /* 2 平级兄弟 */
    if(ptr_go->ptr_next_brother)
    { 
        status = walk(ptr_go->ptr_next_brother, func); 
        if(cmos_OK_E != status)
        {
            assert_failed(__FILE__, __LINE__);
            return status;
        }
    }

    /* 3 下级子 */
    if(ptr_go->ptr_first_sun)
    {
        status = walk(ptr_go->ptr_first_sun, func);
        if(cmos_OK_E != status)
        {
            assert_failed(__FILE__, __LINE__);
            return status;
        }
    }

    return cmos_OK_E;
}

/*******************************************************************************
 *
 * 函数名  : print_node
 * 负责人  : 彭鹏
 * 创建日期：20151030 
 * 函数功能: 打印结点
 *
 * 输入参数: ptr_node 结点指针
 * 输出参数: 无
 *
 * 返回值  : 函数执行状态
 *          
 * 调用关系: 无
 * 其 它   : 无
 *
 ******************************************************************************/
static cmos_status_T print_node(const cmos_hal_hardware_tree_node_T *ptr_node)
{
    if(NULL == ptr_node) /* 空指针 */
    {
        return cmos_PARA_E;
    }

    if(NULL == ptr_node->ptr_name) /* 无名字 不打印 */
    {
        return cmos_OK_E;
    }

    cmos_printf((char *)ptr_node->ptr_name);

    return cmos_OK_E;
}

static cmos_status_T parse_tree_path(cmos_uint8_T *dev_type, cmos_uint8_T *dev_name, const cmos_uint8_T *path)
{
    cmos_status_T status = cmos_ERR_E;
    const cmos_uint8_T *ptr_go = NULL;
    const cmos_uint8_T *ptr_dev_name = NULL;
    cmos_uint8_T head_len = 0;
    cmos_uint32_T dev_type_len = 0;
    cmos_uint32_T dev_name_len = 0;
    cmos_uint32_T dev_len = 0;

    /* TODO:参数检查 */
    if(NULL == path)
    {
        return cmos_PARA_E;
    }

    head_len = strlen(CMOS_HAL_HARDWARE_TREE_ROOT_NAME);
    /* 以 /dev/ 打头 */
    if(0 != strncmp(CMOS_HAL_HARDWARE_TREE_ROOT_NAME, (const char *)path, head_len))
    {
        return cmos_PARA_E;
    }

    ptr_go = path + head_len;

    /* 没有设备类型 设备名 */
    dev_len = strlen((const char *)ptr_go);
    if(0 == dev_len)
    {
        return cmos_PARA_E;
    }

    ptr_dev_name = (const cmos_uint8_T *) strchr((const char *)ptr_go, CMOS_HAL_HARDWARE_TREE_SPLIT_CHAR);
    /* 没有设备名 */
    if(NULL == ptr_dev_name)
    {
        return cmos_PARA_E;
    }

    /* 解析设备类型 */
    /* 设备类型过长 */
    dev_type_len = ptr_dev_name - ptr_go;
    if(CMOS_HAL_HARDWARE_TREE_NAME_SIZE < dev_type_len)
    {
        return cmos_PARA_E;
    }
    strncat((char *)dev_type, (const char *)ptr_go, dev_type_len);
    dev_type[dev_type_len] = NUL;

    /* 解析设备名 */
    ptr_go = ptr_dev_name + 1; /* +1 去掉 '/' */
    /* 预防溢出 */
    dev_name_len = strlen((const char *)ptr_go);
    if(CMOS_HAL_HARDWARE_TREE_NAME_SIZE < dev_name_len)
    {
        return cmos_PARA_E;
    }
    strncat((char *)dev_name, (const char *)ptr_go, dev_name_len);
    dev_name[dev_name_len] = NUL;

    status = cmos_OK_E;
    return status;
}

