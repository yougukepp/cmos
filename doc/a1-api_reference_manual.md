# CMOS内核--API手册
本文档按照CMOS内核api的功能分类描述api。CMOS内核可以理解为Cortex-M操作系统\(Cortex-M Operation System\)，所以目前大多数的API和CMSIS保持一致，在有些细节方面对CMSIS的做法不是很认同，所以有自己的改变。
在CMOS成型时,如《CMOS结构框图.vsd》中绘制，上层接口有两套：一套是与CMSIS完全兼容的，另一套CMOS自己专用的API。在内部实现上CMSIS兼容的API调用了CMOS自己专用API。CMOS专用API用于新研发的应用，例如笔者后续想做的飞行控制；CMSIS兼容API用于其他项目(兼容CMSIS)移植。
本文主要介绍CMOS专用API，目前只描述实现了的API，后面实现一个API加一个。

##  1 数据结构
### 1.1 基本类型
    1.1.1 CmosStatus
      - 类  别:  枚举
      - 作  用:  用于标识状态
      - 范  围:
        * CmosOk         正常情况
        * CmosErrorIsr   该函数不能再ISR中调用
    1.1.2 CmosThreadId 
      - 类  别:  线程入口的函数指针)
      - 作  用:  用于标识线程
      - 范  围:  CmosUint32
    1.1.3 CmosPriority
      - 类  别:  枚举
      - 作  用:  用于标识线程优先级
      - 范  围:
        * CmosPriorityIdle          空闲
        * CmosPriorityLow           低
        * CmosPriorityBelowNormal   低于普通
        * CmosPriorityNormal        普通
        * CmosPriorityAboveNormal   高于普通
        * CmosPriorityHigh          高
        * CmosPriorityRealTime      实时
        * CmosPriorityError         错误
### 1.2 结构体
    2.1.2 CmosThread结构
    - mPthread
      - 类  型:  返回值为void参数为void*型函数指针
      - 作  用:  线程的入口函数
    - mArgv
      - 类  型:  void*型指针
      - 作  用:  线程的参数
    - mPriority
      - 类  型:  CmosPriority
      - 作  用: 该线程的优先级
    - mStackSize
      - 类  型:  CmosUint32
      - 作  用:  该线程的线程栈大小
    - mTimeSlice
      - 类  型:
      - 作  用: 同一优先级有多个线程该线程的运行时间(以系统SysTick为单位)

##  2 函数
### 2.1 内核信息和控制
    2.1.1 内核初始化
      - 函数名:  CmosKernelInitialize
      - 功  能:  初始化内核
      - 参  数:  无
      - 返回值:  状态值
      - 接口  :  CmosStatus CmosKernelInitialize(void)
    2.1.2 内核启动
      - 函数名:  CmosKernelStart
      - 功  能:  启动内核，调用该API后，内核调度开始工作。内核自己至少会启动一个Idle线程占用CPU，此函数永不返回。
      - 参  数:  无
      - 返回值:  状态值
      - 接口  :  CmosStatus CmosKernelStart(void)
##  2.2 线程管理
    2.2.1 内核初始化
      - 函数名:  CmosThreadCreate
      - 功  能:  创建线程(或者任务)
      - 参  数:  无
      - 返回值:  线程标识符
      - 接口  :  CmosThreadId CmosThreadCreate(void);
##  2.3 延迟
    2.3.1. 内核初始化
      - 函数名:  CmosDelay
      - 功  能:  当前线程延迟millisec毫秒
      - 参  数:  millisec表示延迟的毫秒数
      - 返回值:  状态值
      - 接口  :  CmosStatus CmosDelay(CmosUint32 millisec);

