/********************************************************************************
*
* 文件名  ： tyepdef.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150321
* 版本号  ： v1.0
* 文件描述： 通用类型定义文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CMOS_TYPE_DEF_H_
#define _CMOS_TYPE_DEF_H_

/************************************ 头文件 ***********************************/
#include <stdint.h>

/************************************ 宏定义 ***********************************/
/* bool 宏 */
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

/* 空字符 */
#ifndef NUL
#define NUL ('\0')
#endif

/*********************************** 类型定义 **********************************/
typedef unsigned char       cmos_uint8_T;
typedef unsigned short      cmos_uint16_T;
typedef unsigned int        cmos_uint32_T;
typedef unsigned long long  cmos_uint64_T;
typedef char                cmos_int8_T;
typedef short               cmos_int16_T;
typedef int                 cmos_int32_T;
typedef long long           cmos_int64_T;
typedef unsigned char       cmos_bool_T;

/* CMOS函数状态返回 */
typedef enum{
  cmos_OK_E             =  0,

  cmos_DEV_TIMEOUT_E    =  0x7FFFFFFB,
  cmos_DEV_BUSY_E       =  0x7FFFFFFC,
  cmos_DEV_ERR_E        =  0x7FFFFFFD,
  cmos_ERR_E            =  0x7FFFFFFE,
  cmos_RESERVED_E       =  0x7FFFFFFF  /* int正数 最高位为 0 */
}cmos_status_T;


/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CMOS_TYPE_DEF_H_ */

