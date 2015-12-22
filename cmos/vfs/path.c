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
cmos_bool_T vfs_path_is_valid(const cmos_int8_T *path)
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
* 函数名  : vfs_path_depth
* 负责人  : 彭鹏
* 创建日期: 20151106
* 函数功能: 检查路径的级数
*
* 输入参数: path    待解析的路径
* 输出参数: 无
*
* 返回值  : 0  路径错误
*           >0 级数
*
* 调用关系: 无
* 其 它   : 根算一级
*
******************************************************************************/
cmos_int32_T vfs_path_depth(const cmos_int8_T *path)
{
    if(!vfs_path_is_valid(path))
    {
        CMOS_ERR_STR("vfs_path_depth get a invalid path.");
        return 0;
    }

    const cmos_int8_T *go_path = path;
    cmos_int32_T path_len = 0;
    cmos_int32_T separator_count = 0;
    cmos_int32_T separator_len = 0;

    separator_len = strlen((const char *)CMOS_VFS_SEPARATOR);

    /* 查找CMOS_VFS_SEPARATOR个数 */
    do
    {
        go_path = (const cmos_int8_T *)strstr((const char *)go_path, (const char *)CMOS_VFS_SEPARATOR);
        cmos_bool_T over = ((NULL == go_path) || (NUL == *go_path));
        if(over)
        {
            break;
        }
        separator_count++;
        go_path += separator_len; /* 跳过分隔符 */
    }while(TRUE);

    /* 以非CMOS_VFS_SEPARATOR separator_count++ */
    path_len = strlen((const char *)path);
    go_path = path + path_len - separator_len;
    if(0 != strcmp((const char *)go_path, (const char *)CMOS_VFS_SEPARATOR))
    {
        separator_count++;
    }

    return separator_count;
}

/*******************************************************************************
*
* 函数名  : vfs_path_sub_name
* 负责人  : 彭鹏
* 创建日期: 20151104
* 函数功能: 检查路径是否合法
*
* 输入参数: name        第一个目录名字
*           name_max    name的有效大小
*           path        待解析的目录
*           count       path中的第多少级
*
* 输出参数: 无
*
* 返回值  : 运行状态
*
* 调用关系: 无
* 其 它   : TODO:实现
*
******************************************************************************/
cmos_status_T vfs_path_sub_name(cmos_int8_T *name, cmos_int32_T name_max, 
        const cmos_int8_T *path, cmos_int32_T count)
{
    cmos_int32_T depth = 0;
    char *head = NULL;
    cmos_int32_T name_len = 0; 
    cmos_int32_T i = 0;
    char *tail = NULL;
    char *go_path = (char *)path;

    if((NULL == name)
    || (name_max <= 0))
    {
        CMOS_ERR_STR("vfs name buf must not be >=0.");
        goto ERR1;
    } 
    if(!vfs_path_is_valid(path))
    {
        CMOS_ERR_STR("vfs_path_sub_name get a invalid path.");
        goto ERR2;
    }
    if(count < 0)
    {
        CMOS_ERR_STR("vfs_path_sub_name count should >=0.");
        goto ERR2;
    }

    depth = vfs_path_depth(path);
    if(count >= depth)
    {
        CMOS_ERR_STR("vfs_path_sub_name count > path depth."); 
        goto ERR2;
    } 

    if(0 == count) /* 根 */
    {
        /* 拷贝 */
        head = (char *)path;
        name_len = strlen((const char *)CMOS_VFS_ROOT);
    }
    else /* 非根 */
    {
        /* count为需要走过的spepartor */
        for(i = 0; i < count; i++)
        {
            go_path = strstr((const char *)go_path, (const char *)CMOS_VFS_SEPARATOR);
            go_path++;
        }
        head = go_path; 
        tail = strstr((const char *)go_path, (const char *)CMOS_VFS_SEPARATOR);
        if(NULL == tail) /* 需要的是最后一个子目录 且 不带 CMOS_VFS_SEPARATOR */
        {
            name_len = strlen(head);
        }
				else
        {
            name_len = tail - head;
        }
    }

    /* 拷贝 */
    if(name_max <= name_len)
    {
        CMOS_ERR_STR("vfs_path_sub_name name buf is too small."); 
        goto ERR2;
    }
    else /* 正常拷贝 */
    {
        strncpy((char *)name, (const char *)head, name_len);
        name[name_len] = NUL;
    }
    return cmos_OK_E;

ERR1:
    return cmos_NULL_E;

ERR2:
    memset(name, NUL, name_max);
    return cmos_PARA_E;
}

#if 0
{ 
    char *go_path = (char *)path;
    char *go_path_new = (char *)path;
    cmos_int32_T name_len = 0;
    
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

#endif
