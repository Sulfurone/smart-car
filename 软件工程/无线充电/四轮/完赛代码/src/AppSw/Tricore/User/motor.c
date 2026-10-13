#include"motor.h"
/************************速度控制*******************************/
sint8  speed_need=0;                   //目标速度
sint8  speed_need_Boost=0;             //目标高速
sint8  speed_need_normal=0;            //目标速度
sint8  speed_SW_flag=1;                 //速度选择标志

/**************************编码器********************************/
float feed_fix=10.6;           //编码器修正系数
sint32 Feed_flag=0;             //编码器累计计数
sint32 Feed_speed=0;            //编码器采集速度

/*************************电机控制******************************/
float Speed_P=80,Speed_I=50,Speed_D=0; //MAIN电机PID
float speed_out=0;         //输出实际速度
float speed_error;       //速度误差
uint8 Block_motor_time_flag=0;          //堵转计时标志
uint8 Block_motor_duty_flag=0;          //堵转事件标志
uint8 Block_motor_delay_flag=0;         //堵转弛懈标志



/********************速度档位选择************************/
void speed_SW(void)
{
  if(speed_SW_flag==1)           //速度档位1
  {
    speed_need_Boost=14;         //目标高速
    speed_need_normal=14;        //目标速度
  }
  else if(speed_SW_flag==2)     //速度档位2
  {
    speed_need_Boost=20;         //目标高速
    speed_need_normal=15;        //目标速度
  }
  else if(speed_SW_flag==3)     //速度档位3
  {
    speed_need_Boost=25;         //目标高速
    speed_need_normal=20;        //目标速度
  }
  else if(speed_SW_flag==4)     //速度档位4
  {
    speed_need_Boost=30;         //目标高速
    speed_need_normal=25;        //目标速度
  }
  else;
}



/*******************编码器计数结算************************/
void FEED_COUNT(void)
{
  //Feed_speed=ENC_GetCounter(ENC2_InPut_P33_7);//前进为负值//编码器1
  Feed_speed=ENC_GetCounter(ENC6_InPut_P20_3);//前进为正值//编码器4
  Feed_flag += Feed_speed;
  Feed_speed=(Feed_speed/feed_fix); //前进为正值//后退为负值
}



/*******************转速PID控制函数*************************/
void speed_PID(sint8 speed_in)
{
    if(Lleft2<10&&Lright2<10)  //保护，出赛道停车
        {
           speed_in=0;
           speed_out=0;
        }

  signed int static last_error_1;      //上次偏差
  signed int static last_error_2;      //上上次偏差

  speed_error=(float)speed_in-Feed_speed;      //计算转速误差
  speed_out +=Speed_P*(speed_error-last_error_1)+Speed_I*speed_error+ Speed_D*(speed_error-2*last_error_1+last_error_2);

  last_error_2=last_error_1;            //记录上上次误差
  last_error_1=speed_error;             //记录上次误差

  /*******输出限幅 危险，误动！*********/
  if(speed_out>2500)
     speed_out=2500;
   if(speed_out<-2500)
     speed_out=-2500;


   if(speed_out<0)
   {
       ATOM_PWM_SetDuty(ATOMPWM5, -speed_out, 12500);//驱动电机反转
       ATOM_PWM_SetDuty(ATOMPWM4, 0, 12500);
   }
   else
   {
     ATOM_PWM_SetDuty(ATOMPWM5, 0, 12500);//驱动电机正转
     ATOM_PWM_SetDuty(ATOMPWM4, speed_out, 12500);
   }

}







