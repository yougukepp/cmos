/******************************************************************************
 *
 * 文件名  ： 
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150116 
 * 版本号  ： 
 * 文件描述： 
 * 版权说明： Copyright (c) 2000-2020 GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmos_config.h"
#include "pc.h"
#include "tree.h"
#include "list.h"
#include "queue.h"
#include "vfs.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/
static void usage(void);
static cmos_status_T test_tree(void);
static cmos_status_T test_list(void);
static cmos_status_T test_ring_buffer(void);
static void print_node(void *data, void *para);


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
/*******************************************************************************
 *
 * 函数名  : main
 * 负责人  : 彭鹏
 * 创建日期: 无
 * 函数功能: 主函数
 *
 * 输入参数: argc - 参数个数
 *           argv - 命令行参数数组
 *
 * 输出参数: 无
 *
 * 返回值:   0   : 正常退出
 *           其它: 异常退出
 * 调用关系: 无
 * 其 它:    无
 *
 ******************************************************************************/
int main(int argc, char *argv[])
{ 
    cmos_status_T status = cmos_OK_E;
    char *type_str = NULL;
    cmos_int32_T type = 0;

    if(2 != argc)
    {
        usage();
    }

    type_str = argv[1];
    if(0 == strcmp(type_str, "t"))
    {
        type = 0;
    }
    else if(0 == strcmp(type_str, "l"))
    { 
        type = 1;
    }
    else if(0 == strcmp(type_str, "rb"))
    { 
        type = 2;
    }
    else
    {
        type = -1;
    }

    if(0 == type) /* 测试树 */
    { 
        status = test_tree();
        if(cmos_OK_E != status)
        {
            cmos_debug_log("test_tree return %d\n", status);
        }
    }
    else if(1 == type) /* 测试链表 */
    {
        status = test_list();
        if(cmos_OK_E != status)
        {
            cmos_debug_log("test_list return %d\n", status);
        }
    }
    else if(2 == type) /* 测试环形缓冲 */
    {
        status = test_ring_buffer();
        if(cmos_OK_E != status)
        {
            cmos_debug_log("test_ring_buffer return %d\n", status);
        }
    }
    else
    {
        cmos_debug_log("err test type.\n");
    }

    return 0;
}

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : assert_failed
* 负责人  : 彭鹏
* 创建日期: 20151030
* 函数功能: 断言失败调用
*
* 输入参数: file 出错源文件名字
*           line 出错源文件行数
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void assert_failed(char *file, cmos_int32_T line)
{
    printf("%s,%d: assert failed!\n", file, line);
    fflush(stderr);
    fflush(stdout);
    exit(0);
}

/*******************************************************************************
*
* 函数名  : cmos_console_printf
* 负责人  : 彭鹏
* 创建日期: 20151030
* 函数功能: 控制台打印 模式cmos输出行为
*
* 输入参数: 与printf参数含义一致
*
* 输出参数: 无
*
* 返回值  : 函数执行状态
*
* 调用关系: 无
* 其 它   : 系统错误或调试打印需要第一时间输出 采用轮询方式输出
*
******************************************************************************/
cmos_int32_T cmos_console_printf(char *fmt, ...)
{ 
    char printf_buf[1024];
    va_list args;
    cmos_int32_T n = 0;
   
    va_start(args, fmt);
    n = vsprintf(printf_buf, fmt, args);
    va_end(args);

    printf(printf_buf);
    fflush(stdout);

    return n;
}

/*******************************************************************************
*
* 函数名  : cmos_malloc
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: 动态分配内存接口
*
* 输入参数: size 欲分配的字节数
*
* 输出参数: 无
*
* 返回值  : 分配的内存首地址
*
* 调用关系: 无
* 其 它   : 自己实现
*
******************************************************************************/
void *cmos_malloc(cmos_int32_T size)
{
    return malloc(size);
}

/*******************************************************************************
*
* 函数名  : cmos_free
* 负责人  : 彭鹏
* 创建日期: 20151103
* 函数功能: 动态释放内存接口
*
* 输入参数: ptr 欲释放的地址
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 自己实现
*
******************************************************************************/
void cmos_free(void *ptr)
{
    free(ptr);
}

/*******************************************************************************
*
* 函数名  : usage
* 负责人  : 彭鹏
* 创建日期: 20151124
* 函数功能: 打印使用方法
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void usage(void)
{
    printf("./test <t|l>\r\n");
    exit(0);
}

/*******************************************************************************
*
* 函数名  : test_tree
* 负责人  : 彭鹏
* 创建日期: 20151124
* 函数功能: 测试树结构
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static cmos_status_T test_tree(void)
{ 
    cmos_status_T status = cmos_OK_E;
    cmos_debug_log("tree test start.\n");
    status = vfs_init();
    if(cmos_OK_E != status)
    {
        cmos_trace_log("status:0x%08X.", status);
        assert_failed(__FILE__, __LINE__);
    }

    /* 加入i2c设备结点*/
    status = vfs_node_add((const cmos_uint8_T *)CMOS_VFS_DEV_DIR, (const cmos_uint8_T *)("i2c"), vfs_dev, (void *)0x123);
    if(cmos_OK_E != status)
    {
        return status;
    }

    vfs_print();
    cmos_debug_log("tree test done.\n");

    return cmos_OK_E;
}

/*******************************************************************************
*
* 函数名  : test_list
* 负责人  : 彭鹏
* 创建日期: 20151124
* 函数功能: 测试树结构
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static cmos_status_T test_list(void)
{ 
    cmos_status_T status = cmos_OK_E;
    cmos_debug_log("list test start.\n\n");

    cmos_lib_list_T *list = NULL;

    char *ptr1 = "1";
    char *ptr2 = "2";
    char *ptr3 = "3";
    char *ptr_val = NULL;

    cmos_lib_list_walk_func_T list_walk_func = print_node;



    cmos_debug_log("test list as queue.\n");

    cmos_lib_list_init(&list);
    cmos_debug_log("init list(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    cmos_lib_list_push_tail(&list, ptr1);
    cmos_debug_log("push 1 to tail(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");


    cmos_lib_list_push_tail(&list, ptr2);
    cmos_debug_log("push 2 to tail(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    cmos_lib_list_push_tail(&list, ptr3);
    cmos_debug_log("push 3 to tail(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    ptr_val = cmos_lib_list_pop_head(&list);
    cmos_debug_log("pop %s from list(with queue mode) length:%d:\n", ptr_val, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    ptr_val = cmos_lib_list_pop_head(&list);
    cmos_debug_log("pop %s from list(with queue mode) length:%d:\n", ptr_val, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    ptr_val = cmos_lib_list_pop_head(&list);
    cmos_debug_log("pop %s from list(with queue mode) length:%d:\n", ptr_val, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("test list as queue end.\n\n\n\n");




    cmos_debug_log("test list as stack.\n");
    cmos_lib_list_push_tail(&list, ptr1);
    cmos_lib_list_push_tail(&list, ptr2);
    cmos_lib_list_push_tail(&list, ptr3);
    cmos_debug_log("list content with stack mode(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    ptr_val = cmos_lib_list_pop_tail(&list);
    cmos_debug_log("pop %s from list(with stack mode) length:%d:\n", ptr_val, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    ptr_val = cmos_lib_list_pop_tail(&list);
    cmos_debug_log("pop %s from list(with stack mode) length:%d:\n", ptr_val, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    ptr_val = cmos_lib_list_pop_tail(&list);
    cmos_debug_log("pop %s from list(with stack mode) length:%d:\n", ptr_val, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("test list as stack end.\n\n\n\n\n");


    cmos_debug_log("test list as pool begin.\n");
    cmos_lib_list_push_tail(&list, ptr1);
    cmos_lib_list_push_tail(&list, ptr2);
    cmos_lib_list_push_tail(&list, ptr3);
    cmos_debug_log("list content init(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    cmos_lib_list_del_by_data(&list, ptr1);
    cmos_debug_log("list del head %s(length:%d):\n", ptr1, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    cmos_lib_list_del_by_data(&list, ptr3);
    cmos_debug_log("list del tail %s(length:%d):\n", ptr3, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    cmos_debug_log("pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp1\n");
    cmos_lib_list_del_by_data(&list, ptr2);
    cmos_debug_log("pppppppppppppppppppppppppppppppppppppppppppppppppppppppppp2\n");
    cmos_debug_log("list del only node %s(length:%d):\n", ptr2, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

#if 0
    cmos_lib_list_push_tail(&list, ptr1);
    cmos_lib_list_push_tail(&list, ptr2);
    cmos_lib_list_push_tail(&list, ptr3);
    cmos_debug_log("list content init again(length:%d):\n", cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");

    cmos_lib_list_del_by_data(&list, ptr2);
    cmos_debug_log("list del normal node %s(length:%d):\n", ptr2, cmos_lib_list_length(list));
    cmos_lib_list_walk(list, list_walk_func, NULL);
    cmos_debug_log("\n");
#endif

    cmos_debug_log("test list as pool end.\n\n");


    cmos_debug_log("list test done.\n");

    return status;
}

/*******************************************************************************
*
* 函数名  : test_ring_buffer
* 负责人  : 彭鹏
* 创建日期: 20151211
* 函数功能: 测试环形缓冲
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static cmos_status_T test_ring_buffer(void)
{
    cmos_status_T status = cmos_OK_E;
#if 0
    cmos_int32_T i = 0;
    cmos_debug_log("ring_buffer test start.\n");

    cmos_lib_ring_queue_T *queue;

    status = cmos_lib_ring_queue_init(queue, 5);
    if(cmos_OK_E != status) 
    {
        cmos_debug_log("cmos_lib_ring_queue_init failed.\n");
        return status;
    } 
    
    cmos_debug_log("ring queue all add all del start.\n");
    cmos_debug_log("ring queue push:\n");
    for(i = 0; i < 5; i++) 
    {
        c = i + '0';
        status = cmos_lib_ring_queue_push(queue, c);
        if(cmos_OK_E != status) 
        {
            cmos_debug_log("cmos_lib_ring_queue_push failed.\n");
            return status;
        }
        cmos_debug_log("%c", c);
    } 
    
    cmos_debug_log("cmos_lib_ring_queue_pop:\n");
    for(i = 0; i < 5; i++) 
    {
        status = cmos_lib_ring_queue_pop(queue, &c);
        if(cmos_OK_E != status) 
        {
            cmos_debug_log("cmos_lib_ring_queue_push failed.\n");
            return status;
        } 
        cmos_debug_log("%c", c);
    }
    cmos_debug_log("ring queue all add all del end.\n");

    cmos_debug_log("ring_queue test done.\n");
#endif
    return status;
}

/*******************************************************************************
*
* 函数名  : print_node
* 负责人  : 彭鹏
* 创建日期: 20151124
* 函数功能: 打印list的结点
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void print_node(void *data, void *para)
{ 
    char *str = NULL;

    str = (char *)data;
    if(NULL == str)
    {
        cmos_debug_log("get null pointer.\r\n");
        return;
    }

    cmos_debug_log("%c", *str); 
    cmos_debug_log("->", str);
}

