#include <Ctrl.h>
#include "balance.h"
#include "LQ_GPIO_KEY.h"

float Angle_zero =2.5  ;//2号车
float Angle_zero2 =4;//行进轮

//float Angle_zero =-3.5;//1号车
//float Angle_zero2 =1;

PID Palst;  //角速度环结构体
PID Angle;  //角度环结构体
PID Speed;  //速度环结构体
PID Palst_X;  //行进轮角速度环
PID Palst_Z;  //转向角速度环
PID Balance1;  //行进轮角度环直立环
PID Vec;        //行进轮速度环
PID Turn;       //转向环PID
PID zhuanxiang;
PID Speed_Z;
int vec_x =0;
int vec_x_max =-1000;
float Encode_Num;
float dongtai_KP=0.08,dongtai_KD=0;
int Encoder1,Encoder2;
unsigned char g_t,a_t,v_t,x_t,s_t;   //角速度环 角度环 速度环时间变量
short Duty_L,Duty_R,Duty_X;
extern float Accel_x,Accel_y,Accel_z,Gyro_x,Gyro_y,Gyro_z;
float Pitch_dongtai;
extern float Angle_X_Final,Angle_Y_Final;
extern short Motor_C;
extern float gx,gy,gz;
extern unsigned char Start_Flag;
int Offset_Camera;
extern int mid_point[120];            //提取的中线
int midline=40;
int midsaidao=48;
float dongtai_zero;

void Balance_All()  //串级总体控制
{

    g_t++;
    a_t++;
    v_t++;
    x_t++;
    s_t++;
    if(g_t==2)
    {
        g_t=0;
        Euler();
        Offset_Camera = mid_point[midline]-midsaidao;
        dongtai_zero= dongtai_Control(Offset_Camera);
        Pitch_dongtai = Angle_zero+dongtai_zero;
        Palst_loop();                   //动量轮角速度环PID  //2ms周期
        Palst_X_loop();                 //行进轮 直立环PID


//        Duty_L=Palst.Out- Turn.Out;           //左轮占空比
//        Duty_R=Palst.Out+ Turn.Out;           //右轮占空比
        Duty_L=Palst.Out- zhuanxiang.Out;           //左轮占空比
        Duty_R=Palst.Out+ zhuanxiang.Out;           //右轮占空比
        Duty_X = Palst_X.Out ;//+ Vec.Out;

        if(Duty_L < -0)Duty_L-=400;     //死区
        else if(Duty_L > 0)Duty_L +=400;//死区
        if(Duty_R < -0)Duty_R-=400;     //死区
        else if(Duty_R > 0)Duty_R +=400;//死区
        if(Duty_X < -0)Duty_X-=40;     //死区
        else if(Duty_X > 0)Duty_X +=40;//死区
    }
    if(x_t==4)
    {

            Balance_Ctrl();               //行进轮速度环
            x_t=0;
    }


    if(a_t==8)
    {

        Angle_loop();               //动量轮角度环    8ms周期
        a_t=0;
//        Balance_Ctrl();               //行进轮速度环

    }

    if(v_t==20)
    {

        Speed_loop();           //动量轮速度环     20ms周期
        Vec_Ctrl();
        //Turn_loop();            //转向环
        ZhuanXiang();
        v_t=0;
    }
    if(s_t==30)
    {
        Turn_loop();            //转向环

        s_t=0;
    }

    if(Start_Flag==0)
    {
        DIN_OFF;                                        //刹车
        MotorCtrl3W(0,0,0);                             //飞轮 独轮都停止
    }
    else if(Start_Flag==1)
    {
        DIN_OFF;                                        //刹车
        MotorCtrl3W(0,0,Duty_X);                       //飞轮刹车 独轮启动
    }
    else if(Start_Flag==2)
    {
        DIN_ON;                                         //启动
        MotorCtrl3W(Duty_L, -Duty_R,Duty_X);         //飞轮 独轮都启动
       // MotorCtrl3W(0,0,Duty_X);         //飞轮 独轮都启动
    }

}

void Palst_loop() //角速度环   PI
{
    Pid_Position(Angle.Out,gx,&Palst);   //输入为角度环输出


}





void Angle_loop()  //角度环    PD
{
    Pid_Position(Speed.Out+Pitch_dongtai,Angle_X_Final,&Angle);   //输入为速度环输出减去机械零点
}
void Speed_loop()  //速度环    PI
{
    float Encoder_Least;
    static float Encoder;
    ECPULSE1 = ENC_GetCounter(ENC4_InPut_P02_8); // 动量轮反馈 母板编码器左
    Encoder_Least = ECPULSE1;
    Encoder *= 0.7;             //一阶互补滤波
    Encoder += Encoder_Least*0.3;
    Pid_Position(0,Encoder,&Speed);               //期望值为0  输入为编码器采集值
}


/*****************************************************行进轮控制*************/
void Palst_X_loop() //行进轮角速度环   PI
{
    Pid_Position(Balance1.Out,gy,&Palst_X);   //输入为角度环输出

}
void Balance_Ctrl()   //直立环
{
    Pid_Position(Vec.Out+Angle_zero2,Angle_Y_Final,&Balance1);

}

void Vec_Ctrl()  //速度环
{
    float Encoder_Least2;
    static float Encoder2;
    ECPULSE2 = ENC_GetCounter(ENC2_InPut_P33_7);
    Encoder_Least2= ECPULSE2;
    Encoder2 *= 0.7;                             //一阶互补滤波
    Encoder2 += Encoder_Least2*0.3;
    Pid_Position(vec_x,Encoder2,&Vec);


}
float dongtai_Control(float Angle)
{
    float PWM;
    signed int static last_error_1;      //上次偏差
    signed int static last_error_2;      //上上次偏差

    PWM=Angle*dongtai_KP + (Angle-2*last_error_1+last_error_2)*dongtai_KD;   //获取最终数值

    last_error_2=last_error_1;            //记录上上次误差
    last_error_1=Angle;             //记录上次误差
    if(PWM<-3)  PWM=-2;
    if(PWM>3)   PWM=2;
    return PWM;

}


//传入形参为赛道中线与车身偏差
void Turn_loop()
{
    Pid_Position(mid_point[midline],midsaidao,&Turn);
  // Pid_Position(midsaidao,midsaidao,&Turn);

}
void ZhuanXiang()
{
    Pid_Position(Turn.Out,gz,&zhuanxiang);
}
void Turn_Speed()  //速度环    PI
{
    float Encoder_Least;
    static float Encoder;
    ECPULSE1 = ENC_GetCounter(ENC4_InPut_P02_8); // 动量轮反馈 母板编码器左
    Encoder_Least = ECPULSE1;
    Encoder *= 0.7;             //一阶互补滤波
    Encoder += Encoder_Least*0.3;
    Pid_Position(0,Encoder,&Speed_Z);               //期望值为0  输入为编码器采集值
}


void PID_Init() //PID参数初始化
{
    /******************************************************二号车**************************************************************/
    //左右平衡参数
  /*****角速度环参数**********/

    Palst.Kp=500;   //太小了电机没有力，大了发烫严重>=8  +15
    Palst.Ki=0.25;   //对干扰的修正，0.5以内，大了容易修正过度，往反方向倒+  0.8
//    Palst.Kp=100 ;   // >=8  +15
//    Palst.Ki=5;   //    +  0.8
    Palst.Kd=0;   //
    Palst.Integral_lim=3000;   //3000
    Palst.P_out=0;
    Palst.I_out=0;
    Palst.D_out=0;
    Palst.Out=0;
    Palst.Out_lim=9000;     //5000

    /******角度环参数**********/
    Angle.Kp=11; //跟恢复速度有关，小了立不住 大了左右摆     //7
    Angle.Ki=0; //
    Angle.Kd=30; //越大恢复力越强，大于100之后抽搐
//    Angle.Kp=0.5; //+       //7
//    Angle.Ki=0; //
//    Angle.Kd=60; //+
    Angle.Integral_lim=0;
    Angle.Out=0;
    Angle.Out_lim=8000;

    /**********速度环参数*********/
    Speed.Kp=0.045; //0.045大了会疯转
    Speed.Ki=0.001  ; //0.001
    Speed.Kd=0; //
    Speed.Integral_lim=200;
    Speed.Out=0;
    Speed.Out_lim=3000;

    //前进后退参数

    Palst_X.Kp=13; //-0
    Palst_X.Ki=5.5 ; //恢复力/速度，太小立不住，小的话前后晃动幅度大，大的话前后频繁抖动，在5左右
    Palst_X.Kd=0; //
    Palst_X.Integral_lim=400;
    Palst_X.Out=0;
    Palst_X.Out_lim=3000;


    /******角度环参数**********/
    Balance1.Out=0;
    Balance1.Out_lim=3000;
    Balance1.Kp=-0.78;//+
    Balance1.Ki=0; //
    Balance1.Kd=-90;
    /**********速度环参数*********/
    //Vec.Kp=0.011; //-800速度可上坡
    Vec.Kp=0.012; //-800速度可上坡
    Vec.Ki=0.00099; //+

//    Vec.Kp=0; //-600速度可上坡
//    Vec.Ki=0; //+
    Vec.Kd=0; //
    Vec.Integral_lim=3000;
    Vec.Out=0;
    Vec.Out_lim=3000;

    /**********************************************************一号车*********************************************************/
//    //左右平衡参数
//  /*****角速度环参数**********/
//
//    Palst.Kp=160;   // >=8  +15
//    Palst.Ki=6;   //    +  0.8
////    Palst.Kp=100 ;   // >=8  +15
////    Palst.Ki=5;   //    +  0.8
//    Palst.Kd=0;   //
//    Palst.Integral_lim=3000;   //3000
//    Palst.P_out=0;
//    Palst.I_out=0;
//    Palst.D_out=0;
//    Palst.Out=0;
//    Palst.Out_lim=9000;     //5000
//
//    /******角度环参数**********/
//    Angle.Kp=11; //+       //7
//    Angle.Ki=0; //
//    Angle.Kd=20; //+
////    Angle.Kp=0.5; //+       //7
////    Angle.Ki=0; //
////    Angle.Kd=60; //+
//    Angle.Integral_lim=0;
//    Angle.Out=0;
//    Angle.Out_lim=8000;
//
//    /**********速度环参数*********/
////    Speed.Kp=0.04; //
////    Speed.Ki=0.002; //
//    Speed.Kp=0.045; //
//    Speed.Ki=0.001  ; //
//    Speed.Kd=0; //
//    Speed.Integral_lim=200;
//    Speed.Out=0;
//    Speed.Out_lim=3000;
//
//    //前进后退参数
//
//    Palst_X.Kp= 12; //-0
//    Palst_X.Ki=9; //-
//    Palst_X.Kd=0; //
//    Palst_X.Integral_lim=400;
//    Palst_X.Out=0;
//    Palst_X.Out_lim=3000;
//
//
//    /******角度环参数**********/
//    Balance1.Out=0;
//    Balance1.Out_lim=3000;
//    Balance1.Kp=-0.78;//+
//    Balance1.Ki=0; //
//    Balance1.Kd=-90;
//    /**********速度环参数*********/
//    //Vec.Kp=0.011; //-800速度可上坡
//    Vec.Kp=0.012; //-800速度可上坡
//    Vec.Ki=0.00099; //+
//
////    Vec.Kp=0; //-600速度可上坡
////    Vec.Ki=0; //+
//    Vec.Kd=0; //
//    Vec.Integral_lim=3000;
//    Vec.Out=0;
//    Vec.Out_lim=3000;

 /*****转向环参数****************/       //pd
    Turn.Kp=0 ;//25
    Turn.Ki=0;
    Turn.Kd=0;
//    Turn.Kp=-6.6;//25
//    Turn.Ki=0;
//    Turn.Kd=-13;
    Turn.Out=0;
    Turn.Out_lim=200;
    //pi角速度环
//    zhuanxiang.Kp=-40;//25
//    zhuanxiang.Ki=-0.4;
    zhuanxiang.Kp=0;//25
    zhuanxiang.Ki=0;
    zhuanxiang.Kd=0;
    zhuanxiang.Integral_lim=3000;
    //zhuanxiang.Out=0;
    zhuanxiang.Out_lim=2000;
//速度环
    Speed_Z.Kp=0;//25
    Speed_Z.Ki=0;
//    zhuanxiang.Kp=0;//25
//    zhuanxiang.Ki=0;
    Speed_Z.Kd=0;
    Speed_Z.Integral_lim=3000;
    //zhuanxiang.Out=0;
    Speed_Z.Out_lim=9000;




}

