/******************************************************************************
 *
 * 文件名  ： path.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151104
 * 版本号  ： v1.0
 * 文件描述： vfs中path的实现
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 *
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "cmos_config.h"

#include <ctype.h>
#include <string.h>

#include "path.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : vfs_path_is_valid
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 检查路径是否合法
*
* 输入参数: path    路径
* 输出参数: 无
*
* 返回值  : TRUE    合法
*           FALSE   非法
*
* 调用关系: 无
* 其 它   : TODO:实现
*
******************************************************************************/
cmos_bool_T vfs_path_is_valid(const cmos_uint8_T *path)
{
    int c = 0;
    char *go_path = (char *)path;
    if(NULL == go_path)
    {
        return FALSE;
    }
    if(CMOS_VFS_ROOT[0] != go_path[0])
    {
        return FALSE;
    }

    if(CMOS_VFS_PATH_MAX < strlen(go_path))
    {
        return FALSE;
    }

    /* 扫描 */
    while(NUL != *go_path)
    {
        c = *go_path;
        if(!(isalnum(c) /* 合法path只可为 字母 数字 '_' '/' */
        || ('_' == c)
        || ('/' == c)))
        {
            cmos_debug_log("%s: have invalid char:%c(0x%02x)\n", path, c, c);
            return FALSE;
        }
        go_path++;
    }

    return TRUE;
}

/*******************************************************************************
*
* 函数名  : vfs_path_is_valid
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 检查路径是否合法
*
* 输入参数: name        第一个目录名字
*           name_max    name的有效大小
*           path        待解析的目录
* 输出参数: 无
*
* 返回值  : 运行状态
*
* 调用关系: 无
* 其 它   : TODO:实现
*
******************************************************************************/
cmos_status_T vfs_path_head_pop(cmos_uint8_T *name, cmos_uint32_T name_max, const cmos_uint8_T *path)
{ 
    char *go_path = (char *)path;
    char *go_path_new = (char *)path;
    cmos_uint32_T name_len = 0;
    if(NULL == name)
    {
        CMOS_ERR_STR("vfs name must not be NULL.");
        return cmos_NULL_E;
    } 
    if(NUL == *path)
    {
        return cmos_PARA_E;
    }
    
    go_path += CMOS_VFS_SEPARATOR_LEN;
    go_path_new = strstr(go_path, CMOS_VFS_SEPARATOR);
    if(NULL == go_path_new) /* 已经是叶子结点 */
    {
        name_len = strlen(go_path);
        if( 0 == name_len) /* 尾部带有separator的格式 */
        { 
            return cmos_PARA_E;
        }
    }
    else /* 后面还有目录 */
    {
        name_len = go_path_new - go_path;
        if(name_len >= name_max)
        {
            return cmos_BUF_SMALL_E;
        }
    } 
    strncpy((char *)name, go_path, name_len);
    name[name_len] = NUL; 

    return cmos_OK_E;
}

