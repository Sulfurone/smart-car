/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "..\Driver\include.h"//各个模块的头文件

extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 初始化完成标志位  */

int core1_main (void)
{

    IfxCpu_enableInterrupts();
    /*
     * 关闭看门狗
     * */
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());


    //等待CPU0 初始化完成
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));


    /* 用户代码 */


#pragma warning 557         // 屏蔽警告
    while(1)//主循环
    {


    }
#pragma warning default     // 打开警告
}
