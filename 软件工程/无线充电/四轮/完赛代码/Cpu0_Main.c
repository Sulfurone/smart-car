/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

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


/***************************END**********************************/
App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   /** CPU0 初始化完成标志位  */






/*****************执行机构综合控制函数*********************/
void CAR_Drive_duty(void)           //执行机构综合控制函数
{


  if(KEY_Read(DSW0))                //
      DJ_PID(mid_point[25]);//正常舵机控制
  else;

  if(podao_flag)
      DJ_PID(mid_point[30]);

  if(garage_flag)
      DJ_PID(40);


  /************速度控制**************/
  if(0)
  speed_need=speed_need_Boost;
  else
  speed_need=14;//speed_need_normal;       //上电时速度默认为0，按下key0后执行速度挡位选择函数  void speed_SW

    speed_PID(speed_need);

}






/*************************************************************************
*  函数名称：int core0_main (void)
*  功能说明：CPU0主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
int left_lost_flag ;
int right_lost_flag ;


int core0_main (void)
{
	//关闭CPU总中断
	IfxCpu_disableInterrupts();

	//关闭看门狗，如果不设置看门狗喂狗需要关闭
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	//读取总线频率
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);



	//开启CPU总中断
	IfxCpu_enableInterrupts();

	//LED灯所用P10.6和P10.5初始化
	GPIO_LED_Init();
    //adc初始化
    ADC_InitConfig(ADC0, 80000); //初始化
    ADC_InitConfig(ADC1, 80000);
    ADC_InitConfig(ADC2, 80000);
    ADC_InitConfig(ADC3, 80000);
    ADC_InitConfig(ADC4, 80000);
    ADC_InitConfig(ADC5, 80000);
    ADC_InitConfig(ADC6, 80000);
    ADC_InitConfig(ADC7, 80000);
	//串口P14.0管脚输出,P14.1输入，波特率115200
	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
	OLED_Init();
	OLED_CLS();                     //OLED清屏
    CAMERA_Init(50);                //摄像头初始化
    ATOM_PWM_InitConfig(ATOMPWM4, 5000, 12500);    //四路电机pwm初始化
    ATOM_PWM_InitConfig(ATOMPWM5, 5000, 12500);

    ATOM_PWM_InitConfig(ATOMSERVO1, DJ_midpoint, 100);//舵机频率为100HZ，初始值为1.5ms中值

   ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);//编码器4初始化

   ATOM_PWM_InitConfig(IfxGtm_ATOM2_4_TOUT30_P33_8_OUT, 0,4000);//蜂鸣器初始化

   //STM_InitConfig(STM1, STM_Channel_0, 50000);

    left_point[img_y_max]=0;
    right_point[img_y_max]=80;

    //通知CPU1   CPU0初始化完成
    IfxCpu_releaseMutex(&mutexCpu0InitIsOk);



    while(1)//主循环
	{
        if(Camera_Flag == 2)                       //摄像头场中断
           {
              FEED_COUNT();                             //编码器结算

             /* 提取部分使用的数据 */
              Get_Use_Image();

              Get_01_Value(0);                          // 二值化

              Get_ADC();                                   //电磁采集

              Horizontal_line();                         //水平扫线，找边界与中线
              portrait_line();                             //纵向扫线，找特殊元素的拐点

              if(start_flag==0)//起跑标志位
                  {
                              OutInGarage (0, 1);
                              start_flag=1;
                              huandaoshibie++;    //开启环岛识别
                  }

              saidaokuandu();                       //计算25到10行宽度

              left_lost_flag=Lost_line_left();
              right_lost_flag=Lost_line_right();//判断有效行丢线

              misslinecount();                           //丢线行累加

              saidaoyuansu();                          //赛道元素判断，处理



              for(uint8 i=59;i>0;i--)              //oled显示中线
             Pixle[i][(mid_point[i])]=0;

                  CAR_Drive_duty();               //舵机控制与电机控制（转向与速度）
                  GUI_Duty();                           //调试界面


                  if(garage_delay)
                      garage_delay++;           //识别斑马线延迟变量，防止重复识别斑马线


                  if(ruku_flag)
                      OutInGarage (1, 1);           //入库
              /* 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 */
              Camera_Flag = 0;
            }

        LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁
	}
}



