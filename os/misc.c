#include "typedef.h"

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
*
* 函数名  : assert_failed
* 负责人  : 彭鹏
* 创建日期: 20150321
* 函数功能: 断言失败处理
*
* 输入参数: file - 断言失败 源文件全路径
*           line - 断言失败 源文件行位置
*
* 输出参数: 无
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif

