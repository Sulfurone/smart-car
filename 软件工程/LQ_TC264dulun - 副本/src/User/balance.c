#include "Balance.h"
#include "../APP/LQ_MPU6050_DMP.h"
#include <Platform_Types.h>
#include <stdio.h>
#include "LQ_PID.h"
#include "../User/LQ_MotorServo.h"
#include "../APP/LQ_GPIO_KEY.h"
#include "../APP/LQ_TFT18.h"
#include "../Driver/LQ_ADC.h"
#include "../Driver/LQ_CCU6.h"
#include "../Driver/LQ_STM.h"
#include "LQ_MotorServo.h"
#include "../APP/LQ_IIC_Gyro.h"
#include "../Driver/LQ_SOFTI2C.h"
#include "../Driver/LQ_STM.h"
#include "../Driver/LQ_GPT12_ENC.h"
#include "../Driver/LQ_GTM.h"
#include "../User/LQ_PID.h"
#include "LQ_MotorServo.h"


//void Get_distance(void);
//void Ultrasonic_module(void)
unsigned char Start_Flag=0;                    //启动标志
unsigned char  show_flag=0;                     //显示标志
unsigned char  delay_30,delay_flag;             //30毫秒标志

Status_car status_car = idle;                   //小车状态 一开始为空闲状态
Element element = nothing;                      //赛道元素 一开始为默认

int count_size,count_sum;
int count_A,count_B,count_C,count_D,count_E,count_F,count_G;            //计数器
int Flag_A,Flag_B,Flag_C,Flag_D,Flag_E,Flag_F,Flag_G,Flag_H,Flag_I;     //标志位

short Motor_A,Motor_B,Motor_C,Motor_D;                                  //电机PWM变量

short Encoder_A,Encoder_B,Encoder_C,Encoder_D;                          //编码器的脉冲计数
int32_t Encoder_C_Sum;                                                     //行进电机编码器的累计值

short PWM_R,PWM_Y,PWMA_accel,PWMB_accel,PWMC_accel;                       //PWM中间量

int Voltage;                                                            //电池电压

//静置后  Pitch_Zero=1.35  不静置 Pitch_Zero=3.82
float Roll_Zero=-2.56, Roll_Zero1=5.63;      //Roll_Zero1：记录值
float Pitch_Zero=1.50,Pitch_Zero1=5.63;     //Pitch_Zero1:
float Yaw_Zero=0.0;                     //XY轴角度零点，与机械有关，影响稳定性

float Roll_error=0,Pitch_error=0,Yaw_error=0;
float error_Camera, error_Blance;       //摄像头和电磁误差
float error_of_CameraOrBalance;         //转向环误差
//extern int OFFSET0, OFFSET1, OFFSET2;   //面积法三段误差值 8-23 24-39 40-56
//extern sint16 temp0_L, temp0_R;        //临时数值
//extern sint16 temp1_L, temp1_R;        //临时数值
//extern sint16 temp2_L, temp2_R;        //临时数值


float R_Balance_KP=2000 ,  R_Balance_KI=15   ,  R_Balance_KD=275  ;          //AB电机左右倾角4500 15 275
float Y_Balance_KP=-1.05  ,  Y_Balance_KI=30.0  ,  Y_Balance_KD=25.0  ;      //AB电机旋转角度
float R_Velocity_KP=5  ,  R_Velocity_KI=2   ;                               //AB电机速度环

float P_Balance_KP=152.5, P_Balance_KI=1.2,P_Balance_KD=19.5;                //C电机前后倾角
//float P_Balance_KP=420,P_Balance_KI=0.5,P_Balance_KD=8;
float P_Velocity_KP=9.15,P_Velocity_KI=0.155;                                //C电机速度环

float Yaw_control_1,Yaw_control_2 = 0;
float Yaw_mark;
int Move_distance=0;

/*************************************************************************
主控函数
**************************************************************************/
void Balance(void)
{
    static int num;
/*****************基本信息采集***************************************************************************/
    Encoder_A = ENC_GetCounter(ENC5_InPut_P10_3);   //左电机 母板上编码器1，小车前进为负值
    Encoder_B = ENC_GetCounter(ENC4_InPut_P02_8);   //右电机 母板上编码器2，小车前进为正值
    Encoder_C = -ENC_GetCounter(ENC2_InPut_P33_7);   //行进电机 母板上编码器3，小车前进为正值
    Encoder_C_Sum += Encoder_C;                     //行进电机编码器累加
    LQ_DMP_Read();                                  //获取姿态信息

/******************状态机控制***********************************************************************************/
    switch(status_car)
    {
        case idle:          //空闲状态
            num++;

            if(KEY_Read(KEY0) == 0) //按键出库
            {
                status_car = garage_out;
                Encoder_C_Sum = 0;
                num=0;
            }
            break;
        case garage_out:    //出库状态  (用不到 直接走)
            status_car = run;
            break;
        case run:           //循迹状态
/*************************************元素判断*****************************************************************/

/*************************************误差*********************************************************************/
            if(KEY_Read(KEY1) == 0) //按键出库
            {
                status_car = idle;
                Encoder_C_Sum = 0;
            }
            break;
        case garage_in:     //入库状态
            break;
        default: while(1);

    }





/************************保持直立以及转向运算*********************************************************************************************************************************/
    PWM_R = R_balance_Control(Pitch, &Pitch_Zero,gyro[0]);                    //A B电机控制左右倾角
//    if(++Flag_I==30)
//    {
//
//        PWM_Y = Y_balance_Control(error_of_CameraOrBalance, 0,gyro[2]);     //A B电机控制Z轴转动
//        Flag_I=0;
//    }
    Motor_A = PWM_R ;                                //最终控制量
    Motor_B =  -PWM_R ;                                //最终控制量

    PWMC_accel = Velocity_Control_C(Encoder_C);                             //C电机速度环正反馈
    Motor_C = -P_balance_Control(Roll, Roll_Zero, gyro[1]) ;    //C电机控制前后倾角

/*************************保护******************************************************************************************************************************/
    Roll_error = Roll - Roll_Zero;
    Pitch_error = Pitch - Pitch_Zero;
    //超过特定角度停止运行
//    if(fabs(Roll_error)>15 || fabs(Pitch_error)>15){ Motor_A =0,Motor_B =0,Motor_C =0,Start_Flag=0;DIN_OFF;}

/***********************电机限幅*****************************************************************************************************************************/
    Motor_A = constrain_short(Motor_A, -10000, 10000);                     //PWM限幅
    Motor_B = constrain_short(Motor_B, -10000, 10000);                     //PWM限幅
    Motor_C = constrain_short(Motor_C, -3500, 3500);                       //PWM限幅

  // MotorCtrl3W(Motor_A, Motor_B, -Motor_C);
 //  MotorCtrl3W(0,0, -Motor_C);
/////************************电机控制****************************************************************************************************************************/
    if(Start_Flag==0)
    {
        DIN_OFF;                                        //刹车
        MotorCtrl3W(0,0,0);                             //飞轮 独轮都停止
    }
    else if(Start_Flag==1)
    {
        DIN_OFF;                                        //刹车
        MotorCtrl3W(0,0,-Motor_C);                       //飞轮刹车 独轮启动
    }
    else if(Start_Flag==2)
    {
        DIN_ON;                                         //启动
        MotorCtrl3W(Motor_A, Motor_B, -Motor_C);         //飞轮 独轮都启动
    }
//
//    if(delay_flag==1)
//    {
//        if(++delay_30==6)   delay_30=0,delay_flag=0;    //给主函数提供30ms的精准延时
//    }
}

/**************************************************************************
X轴平衡PID控制,角度环
**************************************************************************/
float R_balance_Control(float Angle,float *Angle_Zero,float Gyro)
{
//    static unsigned int n;                         //计数，用来改变零点
    float PWM,Bias;
    static float error;
    Bias=Angle-*Angle_Zero;                                              //获取偏差
    error+=Bias;                                                         //偏差累积
    error = constrain_float(error, -120, 120);                            //积分限幅
    PWM=R_Balance_KP*Bias + R_Balance_KI*error + Gyro*R_Balance_KD/10;   //获取最终数值
    PWM = constrain_float(PWM, -9000, 9000);                            //输出限幅

   // if(Start_Flag==0) PWM=0,error=0;                                     //停止时参数清零
    return PWM;
}
/**************************************************************************
Y轴平衡PID控制,角度环
**************************************************************************/
float P_balance_Control(float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Bias=Angle-Angle_Zero;                                               //获取偏差
    error+=Bias;                                                         //偏差累积
    error = constrain_float(error, -30, 30);                            //积分限幅
    PWM=P_Balance_KP*Bias + P_Balance_KI*error + Gyro*P_Balance_KD/10;   //获取最终数值
    //if(Start_Flag==0) PWM=0,error=0;                                     //停止时参数清零
    return PWM;
}
/**************************************************************************
Z轴平衡PID控制,角度环
**************************************************************************/
float Y_balance_Control(float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Gyro = constrain_float(Gyro, -3000, 3000);                            //取值限幅
    Bias=Angle-Angle_Zero;                                               //获取偏差（偏差值需要重新计算，不然会有跳变）
    error+=Bias;                                                         //偏差累积
    error = constrain_float(error, -30, 30);                            //积分限幅
    PWM=Y_Balance_KP*Bias + Y_Balance_KI*error + Gyro*Y_Balance_KD/10;   //获取最终数值
    if(Start_Flag==0) PWM=0,error=0;                                     //停止时参数清零

    return PWM;
}

/**************************************************************************
速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_A(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder;                                                   //速度滤波
    Encoder *= 0.7;                                                              //一阶低通滤波器
    Encoder += Encoder_Least*0.3;                                                //一阶低通滤波器
    Encoder_Integral += Encoder;                                               //积分出位移
    Encoder_Integral = constrain_float(Encoder_Integral, -2600, 2600);        //积分限幅
    Velocity = Encoder * R_Velocity_KP + Encoder_Integral * R_Velocity_KI/100; //获取最终数值
    Velocity = constrain_float(Velocity, -5000, 5000);
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;                 //停止时参数清零
    return Velocity;
}

/**************************************************************************
速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_B(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder;                                                   //速度滤波
    Encoder *= 0.7;                                                              //一阶低通滤波器
    Encoder += Encoder_Least*0.3;                                                //一阶低通滤波器
    Encoder_Integral += Encoder;                                               //积分出位移
    Encoder_Integral = constrain_float(Encoder_Integral, -2600, 2600);        //积分限幅
    Velocity = Encoder * R_Velocity_KP + Encoder_Integral * R_Velocity_KI/100; //获取最终数值
    Velocity = constrain_float(Velocity, -5000, 5000);
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;                 //停止时参数清零
    return Velocity;
}

/**************************************************************************
C电机速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_C(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder- (float)Move_distance;                                  //速度滤波
    Encoder *= 0.7;                                                            //一阶低通滤波器
    Encoder += Encoder_Least*0.3;                                              //一阶低通滤波器
    Encoder_Integral += Encoder - Move_distance;                             //积分出位移
    Encoder_Integral = constrain_float(Encoder_Integral, -1000, 1000);        //积分限幅
    Velocity = Encoder * P_Velocity_KP + Encoder_Integral * P_Velocity_KI;   //获取最终数值
//    Roll_Zero = Roll_Zero1 - Move_distance/500;
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;               //停止时参数清零
    return Velocity;
}



void Balance_car(void)
{
    unsigned char res;
    char  txt[30];
    float BAT=0.0;
    int DMP_num=0;

    // 6050陀螺仪初始化
    IIC_Init();                     //IIC初始化//IIC初始化
    delayms(100);
    res = Gyro_Chose();             //判断陀螺仪型号
    MPU6050_Init();                 //陀螺仪初始化
    MotorInit();                    //电机初始化
    DIN_OFF;                        //刹车
    MotorCtrl3W(0,0,0);             //飞轮 独轮都停止4
    EncInit();                      //编码器初始化
    LQ_DMP_Init();                  //DMP初始化
    TFTSPI_Init(2);                 //屏幕初始化为竖屏 倒显示
    TFTSPI_CLS(u16CYAN);            // 清屏蓝绿色
    do{
        LQ_DMP_Read();
        sprintf((char*)txt,"%06d",gyro[0]);
        TFTSPI_P8X8Str(9,15,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"%06d",gyro[1]);
        TFTSPI_P8X8Str(9,16,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"%06d",DMP_num++);
        TFTSPI_P8X16Str(0,0,txt,u16RED,u16BLACK);

        sprintf((char*)txt,"Don`t move me!!!");
        TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"Don`t move me!!!");
        TFTSPI_P8X16Str(0,4,txt,u16YELLOW,u16BLACK);
    }while(!(abs(gyro[0])<5 && abs(gyro[1])<5 && abs(gyro[2])<5));
    TFTSPI_CLS(u16CYAN);            // 清屏蓝绿色

    ADC_InitConfig(ADC7, 80000);    //ADC采集电压初始化

    // 定时器1通道0初始化,中断函数在CCU6.C中 */
    CCU6_InitConfig(CCU61, CCU6_Channel0, 5000);// 5ms

    while(1)
    {
/*****************************按键控制-三个电机的启停**************************************************************************************************************************/
        if(KEY_Read(start) == 0 && Start_Flag == 0){
            Start_Flag=1;
            while(KEY_Read(start) == 0);
        }
        if(KEY_Read(start) == 0 && Start_Flag == 1){
            Start_Flag=2;
            while(KEY_Read(start) == 0);
        }
        if(KEY_Read(start) == 0 && Start_Flag == 2){
            Start_Flag=0;
            while(KEY_Read(start) == 0);
        }



        //屏幕画面锁定
        if(KEY_Read(KEY2) == 0)
        {
            while(KEY_Read(KEY2)==0);
            while(1)
            {
                if(KEY_Read(KEY2) == 0)
                {
                    while(KEY_Read(KEY2) == 0);
                    break;
                }
            }
        }

        //调整中值
//        if(KEY_Read(KEY1) == 0 && Start_Flag == 0){
//            Pitch_Zero -= 0.05;
//            while(KEY_Read(KEY1) == 0);
//        }
//        if(KEY_Read(KEY1) == 0 && Start_Flag == 1){
//            Pitch_Zero += 0.03;
//            while(KEY_Read(KEY1) == 0);
//        }
//        if(KEY_Read(KEY2) == 0 && Start_Flag == 0){
//            Roll_Zero -= 0.05;
//            while(KEY_Read(KEY2) == 0);
//        }
//        if(KEY_Read(KEY2) == 0 && Start_Flag == 1){
//            Roll_Zero += 0.03;
//            while(KEY_Read(KEY2) == 0);
//        }
/*****************************按键控制结束**************************************************************************************************************************/

/*****************************显示控制**************************************************************************************************************************/
////        TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);    //显示原图像
//        TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *)Pixle);       //显示二值化图像
//        TFTSPI_Draw_Line(0, 8, 93, 8, u16RED);                          //显示3段区间
//        TFTSPI_Draw_Line(0, 24, 93, 24, u16RED);                        //显示3段区间
//        TFTSPI_Draw_Line(0, 40, 93, 40, u16RED);                        //显示3段区间
//        TFTSPI_Draw_Line(0, 56, 93, 56, u16RED);                        //显示3段区间

//        BAT = ADC_Read(ADC7);
//        sprintf((char*)txt,"BAT:%.2f",BAT);
//        TFTSPI_P8X8Str(0,12,txt,u16BLUE,u16BLACK);           //显示电量
        //显示第一段区间的误差值
//        sprintf((char*)txt,"%04d",temp0_L);
//        TFTSPI_P8X8Str(0,11,txt,u16PURPLE,u16BLACK);
//        sprintf((char*)txt,"%04d",temp0_R);
//        TFTSPI_P8X8Str(5,11,txt,u16PURPLE,u16BLACK);
//        sprintf((char*)txt,"%05d",OFFSET0);
//        TFTSPI_P8X8Str(11,11,txt,u16PURPLE,u16BLACK);
//        //显示第二段区间的误差值
//        sprintf((char*)txt,"%04d",temp1_L);
//        TFTSPI_P8X8Str(0,12,txt,u16PURPLE,u16BLACK);
//        sprintf((char*)txt,"%04d",temp1_R);
//        TFTSPI_P8X8Str(5,12,txt,u16PURPLE,u16BLACK);
//        sprintf((char*)txt,"%05d",OFFSET1);
//        TFTSPI_P8X8Str(11,12,txt,u16PURPLE,u16BLACK);
//        //显示第三段区间的误差值
//        sprintf((char*)txt,"%04d",temp2_L);
//        TFTSPI_P8X8Str(0,13,txt,u16PURPLE,u16BLACK);
//        sprintf((char*)txt,"%04d",temp2_R);
//        TFTSPI_P8X8Str(5,13,txt,u16PURPLE,u16BLACK);
//        sprintf((char*)txt,"%05d",OFFSET2);
//        TFTSPI_P8X8Str(11,13,txt,u16PURPLE,u16BLACK);

        sprintf((char*)txt,"e:%05.2f",/*Yaw_Zero, */error_of_CameraOrBalance);
        TFTSPI_P8X8Str(0,14,txt,u16BLUE,u16BLACK);
        sprintf((char*)txt,"%.2f",Pitch_Zero);          //显示左右零点
        TFTSPI_P8X8Str(8,14,txt,u16BLUE,u16BLACK);

        sprintf((char*)txt,"P:%.02f",Pitch);
        TFTSPI_P8X8Str(0,15,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"%06d",gyro[0]);
        TFTSPI_P8X8Str(9,15,txt,u16RED,u16BLACK);

        sprintf((char*)txt,"R:%.02f",Roll);
        TFTSPI_P8X8Str(0,16,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"%06d",gyro[1]);
        TFTSPI_P8X8Str(9,16,txt,u16RED,u16BLACK);

        sprintf((char*)txt,"Y:%.02f",Yaw);
        TFTSPI_P8X8Str(0,17,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"%06d",gyro[2]);
        TFTSPI_P8X8Str(9,17,txt,u16RED,u16BLACK);


//
//        sprintf((char*)txt,"E:%05d %05d",Encoder_A,Encoder_B);
//        TFTSPI_P8X8Str(0,14,txt,u16RED,u16BLACK);
//
       sprintf((char*)txt,"E:%05d %05d", Encoder_C, Start_Flag);
       TFTSPI_P8X8Str(1,0,txt,u16RED,u16BLACK);
//
 //       sprintf((char*)txt,"P:%05d %05d",Motor_A,Motor_B);
 //       TFTSPI_P8X8Str(0,16,txt,u16RED,u16BLACK);
//
//        sprintf((char*)txt,"C:%05d %.2f",Motor_C, Pitch_Zero);
//        TFTSPI_P8X8Str(0,17,txt,u16RED,u16BLACK);


        sprintf((char*)txt,"%03d",Move_distance);      //目标速度
        TFTSPI_P8X16Str(0,9,txt,u16RED,u16BLACK);

        sprintf((char*)txt,"%06d", Encoder_C_Sum);      //行程
        TFTSPI_P8X16Str(4,9,txt,u16RED,u16BLACK);

//        sprintf((char*)txt,"%01d", status_car);         //小车状态 元素
//        TFTSPI_P8X16Str(11,9,txt,u16GREEN,u16BLACK);
//        sprintf((char*)txt,"%01d", element);         //小车状态 元素
//        TFTSPI_P8X16Str(13,9,txt,u16PURPLE,u16BLACK);

        sprintf((char*)txt,"%05d", Start_Flag);
        TFTSPI_P8X16Str(10,4,txt,u16PURPLE,u16BLACK);
        sprintf((char*)txt,"%06d", PWM_R);
        TFTSPI_P8X16Str(5,1,txt,u16PURPLE,u16BLACK);
        sprintf((char*)txt,"%06d", Motor_A);
        TFTSPI_P8X16Str(5,2,txt,u16PURPLE,u16BLACK);

/*****************************显示控制结束**************************************************************************************************************************/

/*****************************上位机显示波形************************************************************************************************************************/
//        ANO_DT_send_int16byte16((short)Pitch,(short)Roll,gyro[0],gyro[1],gyro[2], Pitch_Zero, Roll_Zero,1);

    }
}

