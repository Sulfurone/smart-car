/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2021年6月16日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.4及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
摄像头接口                  龙邱神眼或者OV7725模块
● 数据端口：P02.0-P02.7口，共8位，接摄像头的数据端口；
● 时钟像素：外部中断第0组：P00_4；
● 场信号：外部中断第3组：P15_1；
-----------------------------------------------------------------
推荐GPT12模块，共可以实现5路正交解码增量编码器（兼容带方向编码器）信号采集，任意选择四路即可；
P33_7, P33_6   龙邱TC母板编码器1
P02_8, P33_5   龙邱TC母板编码器2
P10_3, P10_1   龙邱TC母板编码器3
P20_3, P20_0   龙邱TC母板编码器4
-----------------------------------------------------------------
电感电压采集模块或者麦克风模块
推荐使用AN0-7，共八路ADC，可以满足chirp声音信号及电磁车电感电压采集；
AN0-3          龙邱TC接四个麦克风模块或者电感
-----------------------------------------------------------------
默认电机接口
使用GTM模块，ATOM四个通道可产生4*8共32路PWM，而且各自频率和占空比可调，推荐使用ATOM0的0-7通道；
第一组双路全桥驱动
P23_1         龙邱TC母板MOTOR1_P
P32_4         龙邱TC母板MOTOR1_N
P21_2         龙邱TC母板MOTOR2_P
P22_3         龙邱TC母板MOTOR2_N
第二组双路全桥驱动
P21_4         龙邱TC母板MOTOR3_P
P21_3         龙邱TC母板MOTOR3_N
P20_8         龙邱TC母板MOTOR4_P
P21_5         龙邱TC母板MOTOR4_N
-----------------------------------------------------------------
默认舵机接口
使用GTM模块，ATOM四个通道可产生4*8共32路PWM，而且各自频率和占空比可调，推荐使用ATOM2；
P33_10        龙邱TC母板舵机1
P33_13        龙邱TC母板舵机2
-----------------------------------------------------------------
 默认屏幕显示接口，用户可以使用TFT或者OLED模块
TFTSPI_CS     P20_14     龙邱TC母板 CS管脚 默认拉低，可以不用
TFTSPI_SCK    P20_11     龙邱TC母板 SPI SCK管脚
TFTSPI_SDI    P20_10     龙邱TC母板 SPI MOSI管脚
TFTSPI_DC     P20_12     龙邱TC母板 D/C管脚
TFTSPI_RST    P20_13     龙邱TC母板 RESET管脚
-----------------------------------------------------------------
OLED_CK       P20_14     龙邱TC母板OLED CK管脚
OLED_DI       P20_11     龙邱TC母板OLED DI管脚
OLED_RST      P20_10     龙邱TC母板OLED RST管脚
OLED_DC       P20_12     龙邱TC母板OLED DC管脚
OLED_CS       P20_13     龙邱TC母板OLED CS管脚 默认拉低，可以不用
----------------------------------------------------------------
默认按键接口
KEY0p         P22_0      龙邱TC母板上按键0
KEY1p         P22_1      龙邱TC母板上按键1
KEY2p         P22_2      龙邱TC母板上按键2
DSW0p         P33_9      龙邱TC母板上拨码开关0
DSW1p         P33_11     龙邱TC母板上拨码开关1
-----------------------------------------------------------------
默认LED接口
LED0p         P10_6      龙邱TC母板核心板上LED0 翠绿
LED1p         P10_5      龙邱TC母板核心板上LED1 蓝灯
LED2p         P20_6      龙邱TC母板上LED0
LED3p         P20_7      龙邱TC母板上LED1
-----------------------------------------------------------------
默认蜂鸣器接口
BEEPp         P33_8      龙邱TC母板上蜂鸣器接口
-----------------------------------------------------------------
定时器
有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
推荐使用CCU6模块，STM用作系统时钟或者延时；
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <stdio.h>
#include "balance.h"
#include "src/APP/LQ_ADC_test.h"
#include "src/APP/LQ_Atom_Motor.h"
#include "src/APP/LQ_CAMERA.h"
#include "src/APP/LQ_EEPROM_TEST.h"
#include "src/APP/LQ_FFT_TEST.h"
#include "src/APP/LQ_GPIO_ExInt.h"
#include "src/APP/LQ_GPIO_KEY.h"
#include "src/APP/LQ_GPIO_LED.h"
#include "src/APP/LQ_GPT_mini512.h"
#include "src/APP/LQ_I2C_9AX.h"
#include "src/APP/LQ_I2C_VL53.h"
#include "src/APP/LQ_ICM20602.h"
#include "src/APP/LQ_OLED096.h"
#include "src/APP/LQ_STM_Timer.h"
#include "src/APP/LQ_TFT18.h"
#include "src/APP/LQ_Tim_InputCature.h"
#include "src/APP/LQ_Tom_Servo.h"
#include "src/APP/LQ_UART_Bluetooth.h"
#include "src/Driver/include.h"
#include "src/Driver/LQ_STM.h"
#include "src/Driver/LQ_UART.h"
#include "src/User/LQ_MotorServo.h"
#include <LQ_IIC_Gyro.h>
#include "LQ_TFT2.h"
#include "LQ_BLDC.h"
#include "LQ_SOFTI2C.h"
#include "LQ_OV7725.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_CCU6.h"
#include "swj.h"
#include "LQ_MotorServo.h"
#include <Ctrl.h>
#include "button.h"
App_Cpu0 g_AppCpu0;                       // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0占用/1释放 TFT
#define DIN P22_3
#define DIN_ON  PIN_Write(DIN, 1);         // 启动（刹车线）
#define DIN_OFF PIN_Write(DIN, 0);         // 停止（刹车线）
extern short PWMD1,PWMD2,PWM_JDH1,PWM_JDH2;
extern int32_t Encoder_C_Sum;  //行进电机编码器的累计值
extern volatile char mutexCpu0TFTIsOk;         // CPU1 0占用/1释放 TFT
extern int Target_Speed;
extern unsigned char Start_Flag;                    //启动标志
extern unsigned char  show_flag;                     //显示标志
extern unsigned char  delay_30,delay_flag;             //30毫秒标志
extern short PWM_SDH1,PWM_SDH2;
extern int count_size,count_sum;
extern int count_A,count_B,count_C,count_D,count_E,count_F,count_G;            //计数器
extern int Flag_A,Flag_B,Flag_C,Flag_D,Flag_E,Flag_F,Flag_G,Flag_H,Flag_I;     //标志位
extern float dongtai_zero;
extern float X_anglespeed_KP  ,  X_anglespeed_KI  ,X_anglespeed_KD;     // PI

extern float X_Balance_KP,X_Balance_KD;

extern Offset_Camera;
extern float R_Velocity_KP  ,  R_Velocity_KI   ;


extern short Motor_A,Motor_B,Motor_C,Motor_D;                                  //电机PWM变量
extern short Encoder_A,Encoder_B,Encoder_C,Encoder_D;                          //编码器的脉冲计数
//extern int32_t Encoder_C_Sum;                                                  //行进电机编码器的累计值
extern short PWM_R,PWM_Y,PWMA_accel,PWMB_accel,PWMC_accel;                       //PWM中间量
extern int Voltage;                                                            //电池电压

//静置后  Pitch_Zero=1.35  不静置 Pitch_Zero=3.82
extern float Roll_Zero, Roll_Zero1;      //Roll_Zero1：记录值
extern float Pitch_Zero,Pitch_Zero1;     //Pitch_Zero1:
extern float Yaw_Zero;                     //XY轴角度零点，与机械有关，影响稳定性
extern float Roll_error,Pitch_error,Yaw_error;
//extern float error_Camera, error_Blance;       //摄像头和电磁误差
extern float error_of_CameraOrBalance;         //转向环误差
extern float R_Balance_KP ,  R_Balance_KI   ,  R_Balance_KD  ;          //AB电机左右倾角4500 15 275
extern float Velocity_KP ,  Velocity_KI ;
extern float P_Balance_KP,P_Balance_KI,P_Balance_KD;                       //2.11
extern float P_Velocity_KP,P_Velocity_KI;                                //2.11

extern float Y_Balance_KP,  Y_Balance_KI,  Y_Balance_KD ;      //AB电机旋转角度
extern float Yaw_control_1,Yaw_control_2;
extern float Yaw_mark;
extern int Move_distance;

extern float Pitch_dongtai;
//extern float Y_anglespeed_KP ,  Y_anglespeed_KI  ,Y_anglespeed_KD;     // PI
//extern float Y_Balance_KP,Y_Balance_KD;
//extern float P_Velocity_KP;
extern int mid_point[120];            //提取的中线
extern float Angle_X_Final,Angle_Y_Final;
extern short Duty_L,Duty_R,Duty_X;
extern float Accel_x,Accel_y,Accel_z,Gyro_x,Gyro_y,Gyro_z;
extern gx,gy,gz;
extern PID Palst;  //角速度环结构体
extern PID Angle;  //角度环结构体
extern PID Speed;  //速度环结构体
extern PID Palst_X;  //行进轮角速度环
extern PID Balance1;  //行进轮角度环直立环
extern PID Vec;        //行进轮速度环
extern PID Turn;       //转向环PID
extern PID zhuanxiang;
extern float Angle_zero;//动量轮零点
extern float Angle_zero2;
unsigned char SW0=0,SW1=0,SW2=0,SW3=0,SW4=0;
unsigned char flag=0;
extern int midline;
extern int midsaidao;
extern float ax,ay,az;
extern int vec_x,vec_x_max;
/*************************************************************************
*  函数名称：int core0_main (void)
*  功能说明：CPU0主函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
char  xt[30];
char txt[30];
int num;
void page1(void)//动量轮改参
{

    sprintf((char*)txt,"P:%.3f",Palst.Kp);
    TFTSPI_P8X16Str(5,0,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"I:%.3f",Palst.Ki);
    TFTSPI_P8X16Str(5,1,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"P:%.3f",Angle.Kp);
    TFTSPI_P8X16Str(5,2,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"D:%.3f",Angle.Kd);
    TFTSPI_P8X16Str(5,3,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"P:%.5f",Speed.Kp);
    TFTSPI_P8X16Str(5,4,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"I:%.5f",Speed.Ki);
    TFTSPI_P8X16Str(5,5,txt,u16RED,u16BLACK);

    if(KEY_Read(KEY0) == 0 && Start_Flag == 0)
    {
        Angle.Kp += 0.1;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 0)
    {
        Angle.Kp -= 0.1;
        while(KEY_Read(KEY1) == 0);
    }

    if(KEY_Read(KEY2) == 0 && Start_Flag == 0)
    {
        Angle.Kd += 1;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 0)
    {
        Angle.Kd -= 1;
        while(KEY_Read(start) == 0);
    }
    if(KEY_Read(KEY0) == 0 && Start_Flag == 1)
    {
        Speed.Kp += 0.001;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 1)
    {
        Speed.Kp -= 0.001;
        while(KEY_Read(KEY1) == 0);
    }
    if(KEY_Read(KEY2) == 0 && Start_Flag == 1)
    {
        Speed.Ki += 0.001;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 1)
    {
        Speed.Ki -= 0.001;
        while(KEY_Read(start) == 0);
    }
}
void page2(void)//行进轮改参
{
    sprintf((char*)txt,"P:%.3f",Palst_X.Kp);
    TFTSPI_P8X16Str(5,0,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"I:%.3f",Palst_X.Ki);
    TFTSPI_P8X16Str(5,1,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"P:%.3f",Balance1.Kp);
    TFTSPI_P8X16Str(5,2,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"D:%.3f",Balance1.Kd);
    TFTSPI_P8X16Str(5,3,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"P:%.5f",Vec.Kp);
    TFTSPI_P8X16Str(5,4,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"I:%.5f",Vec.Ki);
    TFTSPI_P8X16Str(5,5,txt,u16RED,u16BLACK);

    if(KEY_Read(KEY0) == 0 && Start_Flag == 0)
    {
        Balance1.Kp += 0.01;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 0)
    {
        Balance1.Kp -= 0.01;
        while(KEY_Read(KEY1) == 0);
    }

    if(KEY_Read(KEY2) == 0 && Start_Flag == 0)
    {
        Balance1.Kd += 1;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 0)
    {
        Balance1.Kd -= 1;
        while(KEY_Read(start) == 0);
    }
    if(KEY_Read(KEY0) == 0 && Start_Flag == 1)
    {
        Vec.Kp += 0.0001;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 1)
    {
        Vec.Kp -= 0.0001;
        while(KEY_Read(KEY1) == 0);
    }
    if(KEY_Read(KEY2) == 0 && Start_Flag == 1)
    {
        Vec.Ki += 0.00001;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 1)
    {
        Vec.Ki -= 0.00001;
        while(KEY_Read(start) == 0);
    }

}
void page3(void)//中值改参及赛道中线位置
{
    sprintf((char*)txt,"M:%3d",midline);
    TFTSPI_P8X8Str(2,10,txt,u16RED,u16BLACK);

    if(KEY_Read(KEY0) == 0 && Start_Flag == 0)
    {
        Angle_zero += 0.05;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 0)
    {
        Angle_zero -= 0.05;
        while(KEY_Read(KEY1) == 0);
    }

    if(KEY_Read(KEY2) == 0 && Start_Flag == 0)
    {
        Angle_zero2 += 0.05;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 0)
    {
        Angle_zero2 -= 0.05;
        while(KEY_Read(start) == 0);
    }
    if(KEY_Read(KEY0) == 0 && Start_Flag == 1)
    {
        midline += 1;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 1)
    {
        midline -= 1;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY2) == 0 && Start_Flag == 1)
    {
        midsaidao += 1;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 1)
    {
        midsaidao -= 1;
        while(KEY_Read(start) == 0);
    }
}
void page4(void)//转向环改参
{
    sprintf((char*)txt,"P:%.5f",zhuanxiang.Kp);
    TFTSPI_P8X16Str(5,0,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"I:%.5f",zhuanxiang.Ki);
    TFTSPI_P8X16Str(5,1,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"P:%.5f",Turn.Kp);
    TFTSPI_P8X16Str(5,2,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"D:%.5f",Turn.Kd);
    TFTSPI_P8X16Str(5,3,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"I:%.5f",zhuanxiang.Out);
    TFTSPI_P8X16Str(5,4,txt,u16RED,u16BLACK);

    if(KEY_Read(KEY0) == 0 && Start_Flag == 0)
    {
        zhuanxiang.Ki += 0.1;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 0)
    {
        zhuanxiang.Ki -= 0.1;
        while(KEY_Read(KEY1) == 0);
    }

    if(KEY_Read(KEY2) == 0 && Start_Flag == 0)
    {
        zhuanxiang.Kp += 1;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 0)
    {
        zhuanxiang.Kp -= 1;
        while(KEY_Read(start) == 0);
    }
    if(KEY_Read(KEY0) == 0 && Start_Flag == 1)
    {
        Turn.Kp += 0.1;
        while(KEY_Read(KEY0) == 0);
    }
    if(KEY_Read(KEY1) == 0 && Start_Flag == 1)
    {
        Turn.Kp -= 0.1;
        while(KEY_Read(KEY1) == 0);
    }

    if(KEY_Read(KEY2) == 0 && Start_Flag == 1)
    {
        Turn.Kd += 0.1;
        while(KEY_Read(KEY2) == 0);
    }
    if(KEY_Read(start) == 0 && Start_Flag == 1)
    {
        Turn.Kd -= 0.1;
        while(KEY_Read(start) == 0);
    }
}
void page5(void)//启动状态
{


    switch(status_car)
    {
        case idle:          //空闲直立状态

            vec_x = 0;      //目标速度清零
//            Turn.Kp=0;
//            if(KEY_Read(KEY1) == 0) //按键出库
//            {
//                Turn.Kp=-11.4 ;//25
//
//
//            }

            if(KEY_Read(KEY0) == 0) //按键开始启动
            {
                //切换PID参数
                    status_car = turn;

            }
            break;
        case turn:          //速度转换(加减速)

//                if(1)//如果用摄像头加电磁，这里可以将1换成标志位，用来切换设摄像头和电磁
//                    error_of_CameraOrBalance = error_Camera;
//                else
//                    error_of_CameraOrBalance = error_Blance;

/*************************************提速*********************************************************************/
            if(vec_x > vec_x+50)    //速度不到vec_max,提速(缓慢提速)
            {

                if(++num % 15 == 0)
                    vec_x-=5;
            }
            else
            {
                status_car = run;
                num=0;
            }
            break;
        case run:           //循迹状态
            vec_x = vec_x_max;//达到最大速度

    }
}
int core0_main (void)
{

    unsigned char res;
	// 关闭CPU总中断
	IfxCpu_disableInterrupts();

	// 关闭看门狗，如果不设置看门狗喂狗需要关闭
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

	// 读取总线频率
	g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
	g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
	g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
	g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

    // 6050陀螺仪初始化
    IIC_Init();                     //IIC初始化//IIC初始化
    delayms(100);
    res = Gyro_Chose();             //判断陀螺仪型号
    MPU6050_Init();                 //陀螺仪初始化
    MotorInit();                    //电机初始化
    DIN_OFF;                           //刹车
    MotorCtrl3W(0,0,0);             //飞轮 独轮都停止4
    EncInit();                      //编码器初始化
    TFTSPI_Init(2);                 //屏幕初始化为竖屏 倒显示
    TFTSPI_CLS(u16CYAN);            // 清屏蓝绿色
    PID_Init(); //PID参数初始化
	// 按键初始化
	GPIO_KEY_Init();
	// LED灯所用P10.6和P10.5初始化
	GPIO_LED_Init();

	// 串口P14.0管脚输出,P14.1输入，波特率115200
	//UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);

	// 开启CPU总中断
	IfxCpu_enableInterrupts();

	// 通知CPU1，CPU0初始化完成
	IfxCpu_releaseMutex(&mutexCpu0InitIsOk);
	// 切记CPU0,CPU1...不可以同时开启屏幕显示，否则冲突不显示
	mutexCpu0TFTIsOk=0;         // CPU1： 0占用/1释放 TFT
	CCU6_InitConfig(CCU61, CCU6_Channel0, 1000);// 1ms
 	CAMERA_Init(50);
    // 所有含有中断的测试都默认在CPU0中执行，如果需要用CPU1请参考龙邱B站视频。
    // 程序配套视频地址：https://space.bilibili.com/95313236
    // 以下测试函数，内建死循环，用户可调用所用模块的初始化及读写函数来实现自己的任务
	//________________________________________________________________________________
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	  Test_ADC_OLED();       //PASS,测试ADC采样时间  OLED上显示 ADC采样10K次时间
//	  Test_ADC_TFT();        //PASS,测试ADC采样时间 TFT上显示 ADC采样10K次时间
//	  Test_ADC_7mic();       //PASS,测试ADC\UART0、STM延时和闪灯，通过UART0打印 AN0--AN7共8个通道ADC转换数据
//    LQ_Atom_Motor_8chPWM();//PASS,测试GTM_ATOM生成不同频率下的8路PWM
//    LQ_ATom_Servo_2chPWM();//PASS,测试GTM_ATOM、STM延时和闪灯，P33.10和P33.13作为ATOM输出口控制舵机
//    LQ_Tom_Servo_2chPWM(); //PASS,测试GTM_TOM、STM延时和闪灯，P33.10和P33.13作为TOM输出口控制舵机
//    Test_GPIO_Extern_Int();//PASS,测试外部第1组中断P15.8，P10.6和P10.5闪灯
//    Test_GPIO_LED();       //PASS,测试GPIO，P10.6和P10.5闪灯
//    Test_GPIO_KEY();       //PASS,测试外部按键输入，P22.0--2   按下按键 LED亮
//    Test_ComKEY_Tft();     //PASS,测试外部组合按键输入并TFT1.8显示，P22.0--2
//    LQ_GPT_4mini512();     //PASS,测试编码器正交解码,OLED和UART输出
//    LQ_GPT_4mini512TFT();  //PASS,测试编码器正交解码,TFT1.8和UART输出
//    Test_OLED();           //PASS,测试OLED0.96屏使用P20.14--10，显示字符串及动态数据
//    LQ_STM_Timer();        //PASS,测试定时中断、闪灯
//    LQ_TIM_InputCature();  //PASS,测试GTM_TOM_TIM、P20_9作为PWM输出口，P15_0作为TIM输入口，两者短接后，串口P14.0发送到上位机
//    Test_Bluetooth();      //PASS,测试UART0(P14.0RX/P14.1TX)，
//    Test_EEPROM();         //PASS,测试内部EEPROM擦写功能  OLED提示是否写入成功
//    Test_Vl53();           //PASS,测试VL53  IIC接线   P13_1接SCL  P13_2接SDA OLED显示原始数据
//    Test_9AX();            //PASS,测试龙邱九轴 IIC接线   P13_1接SCL  P13_2接SDA OLED显示原始数据
//    Test_MPU6050();        //PASS,测试MPU6050或者ICM20602 IIC接线   P13_1接SCL  P13_2接SDA OLED显示原始数据
//    Test_ICM20602();       //PASS,测试ICM20602 SPI接线   P15_8接SCL  P15_5接SDA  P15_7接SA  P15_2接CS OLED显示原始数据
//   Test_CAMERA();         //PASS,测试龙邱神眼摄像头并在屏幕上显示  LQ_CAMERA.h 中选择屏幕
//    Test_SoftFft();        //PASS,测试ILLD库的软件FFT函数
//    Test_FFT();            //PASS,测试硬件FFT  注意需要芯片后缀带DA的才有硬件FFT功能
//    TestServo();
//   TestEncoder();
//    Test_IIC_Gyro();
//
/************************************************以下为本次更新的例程2022-12-05****************************************************************/
//    TestMotor();           //PASS,测试动量轮电机和行进电机 6S电池供电，内部有对行进电机的限幅（-3500，3500）
//    Test_TFT18();          //PASS,测试TFT1.8彩屏使用P20.14--10，显示字符串及动态数据(为了方面调车 新增竖屏倒屏显示 屏幕初始化函数传入参数为2)
//    Test_TFT2();           //PASS,测试TFT1.8彩屏使用P20.14--10，显示字符串及动态数据(为了方面调车 新增竖屏倒屏显示 屏幕初始化函数传入参数为2)
//	Test_LQDMP();          //PASS,测试6050陀螺仪，TFT1.8屏幕显示，可以通过上位机显示（需要取消相关注释）
/********************************************************************************

  无刷电机接线：
      母板接驱动板：
              PWM :P23.1->pwm1  P32.4->pwm2  P21.2->pwm3  22.3->pwm4  P21.5->pwm5  P20.8->pwm6
              电流检测：ADC0->OA  ADC1->OB  ADC2->OC  （接ADC采集口）
              主板电压检测：ADC7

      驱动板接电机(驱动板：LQ2304/2136  电机：LQ2830无刷电机)：
                  A->黄线  B->黑线  C->红线

      霍尔接线: P13_0 -> HA（绿色数据线）
               P10_1 -> HB（棕色数据线）
               P10_3 -> HC（橙色数据线）
*******************************************************************************/
//	  TestMotorBLDC();
//________________________________________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 所有含有中断的测试都默认在CPU0中执行，如果需要用CPU1请参考龙邱B站视频。
// 程序配套视频地址：https://space.bilibili.com/95313236


    while (1)
    {


        /*****************************按键控制-三个电机的启停**************************************************************************************************************************/
                if(KEY_Read(KEY3) == 0 && Start_Flag == 0){
                    Start_Flag=1;
                    while(KEY_Read(KEY3) == 0);
                }
                if(KEY_Read(KEY3) == 0 && Start_Flag == 1){
                    Start_Flag=2;
                    while(KEY_Read(KEY3) == 0);
                }
                if(KEY_Read(KEY3) == 0 && Start_Flag == 2){
                    Start_Flag=0;
                    while(KEY_Read(KEY3) == 0);
                }
////////////////////上位机显示波形/////////////////////////////
                put_int32(0,Duty_L);     //无刷减速电机波形
                put_float(2, Duty_X);     //行进轮波形
                put_float(1, Angle_X_Final); //动量轮角度
                put_float(3, Pitch_dongtai);  //动态零点
        /*****************************显示控制**************************************************************************************************************************/
                if(Start_Flag==0)
                {
                    DIN_OFF;                                        //刹车
                    MotorCtrl3W(0,0,0);                             //飞轮 独轮都停止
                    status_car = idle;
                }
                else if(Start_Flag==1)
                {
                    DIN_OFF;                                        //刹车
                    MotorCtrl3W(0,0,-Motor_C);                       //飞轮刹车 独轮启动
                }
                else if(Start_Flag==2)
                {
                    DIN_ON;                                         //启动
                    MotorCtrl3W(Motor_A, -Motor_A, -Motor_C);         //飞轮 独轮都启动
                }

                ///////////////拨码开关///////////////////////
                if(KEY_Read(DSW0) == 0 && Start_Flag == 0)
                {
                    SW0 = 0;
                }
                if(KEY_Read(DSW0) == 1 && Start_Flag == 0)
                {
                    SW0 = 1;
                }
                if(KEY_Read(DSW1) == 0 && Start_Flag == 0)
                {
                    SW1 = 0;
                }
                if(KEY_Read(DSW1) == 1 && Start_Flag == 0)
                {
                    SW1 = 1;
               }
                if(KEY_Read(DSW3) == 0 && Start_Flag == 0)
                {
                    SW2 = 0;
                }
                if(KEY_Read(DSW3) == 1 && Start_Flag == 0)
                {
                    SW2 = 1;
                }
                if(KEY_Read(DSW2) == 0 && Start_Flag == 0)
                {
                    SW3 = 0;
                }
                if(KEY_Read(DSW2) == 1 && Start_Flag == 0)
                {
                    SW3 = 1;
               }
                //拨码开关用的时候打开一个其他需为关闭状态
                if(SW0)     page1();//调动量轮
                if(SW1)     page2();//调行进轮
                if(SW2)     page4();//转向环改参
                if(SW3)     page5();//启动状态
                //if(SW3)    page3();

                ///////////屏幕显示参数////////////
                sprintf((char*)xt,"%.3f",dongtai_zero);        //动态零点相加
                TFTSPI_P8X8Str(11,10,xt,u16PURPLE,u16BLACK);
                sprintf((char*)xt,"%2d",mid_point[midline]);        //中线值
                TFTSPI_P8X8Str(11,11,xt,u16PURPLE,u16BLACK);
                sprintf((char*)xt,"mid:%2d",midsaidao);        //自己设定目标中线
                TFTSPI_P8X8Str(0,11,xt,u16PURPLE,u16BLACK);
                sprintf((char*)xt,"gz:%.2f",gz);                //z轴角速度
                TFTSPI_P8X8Str(8,12,xt,u16PURPLE,u16BLACK);
                sprintf((char*)xt,"E:%05d",ECPULSE2);           //编码器
                TFTSPI_P8X8Str(0,12,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"P:%05d",Duty_L);
                TFTSPI_P8X8Str(0,13,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"E:%05d %05d",Encoder_A,Encoder_B);
                TFTSPI_P8X8Str(0,14,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"E:%05d %05d", Encoder_C, Start_Flag);//动量轮编码器、停止标志位
                TFTSPI_P8X8Str(0,15,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"P:%.02f",Angle_X_Final);
                TFTSPI_P8X8Str(0,16,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"%.2f",gx);
                TFTSPI_P8X8Str(9,16,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"R:%.02f",Angle_Y_Final);
                TFTSPI_P8X8Str(0,17,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"%.2f",gy);
                TFTSPI_P8X8Str(9,17,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"z:%.2f %.2f",Angle_zero2, Angle_zero);    //零点
                TFTSPI_P8X8Str(0,18,xt,u16RED,u16BLACK);
//                sprintf((char*)xt,"%05d",mid_point[30]);        //中线
//                TFTSPI_P8X8Str(11,19,xt,u16PURPLE,u16BLACK);
                sprintf((char*)xt,"%03d",vec_x_max);      //目标速度
                TFTSPI_P8X8Str(0,19,xt,u16RED,u16BLACK);
                sprintf((char*)xt,"C:%3d",Offset_Camera);
                TFTSPI_P8X8Str(5,19,xt,u16RED,u16BLACK);

                //显示摄像头图像
                TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *)Bin_Image);
                //TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
                //sendimg_zoom( *Bin_Image,LCDW, LCDH, LCDW/2, LCDH/2);
                //sendimg(*Bin_Image, LCDW,  LCDH);
               // sendimg_binary(*Image_Use, LCDW, LCDH,80);

    }
}
