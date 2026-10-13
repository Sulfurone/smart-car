#include"servo.h"
/**************************舵机*********************************/
uint8  KP_A=16,KP_B=0,KD=11;            //MAIN舵机PID
//uint32 DJ_midpoint=1440;                //舵机中值
uint32 DJ_midpoint=1400;                //舵机中值
uint32 DJ_PWM;                          //输出PWM
sint16 midpoint_error;   //中线误差
sint16 lastpoint_error;   //上次误差


int san = 0;
int error_nihe;
int error;
int  lasterror;
float Kd_pu=0;//0   4.2
 float Kp_pu=4;  //Kp_pu=2.99;
 float steer;


/***********************DJ_PID**************************/
void DJ_PID(unsigned char midpoint_in)  //舵机PD
{



      san=midpoint_in-40;


      error = san;

      error_nihe = (int)(Kp_pu*error+Kd_pu*(error-lasterror));

      steer = DJ_midpoint -error_nihe;

      if(san>7)
      {
          Kp_pu =13;
          Kd_pu=2.99;
      }

      if(san<-7)
      {
          Kp_pu=13;
          Kd_pu=2.99;
      }

//      if(ruku_flag)
//          steer=DJ_midpoint+210;
//
//      if(podaostate)
//          steer=DJ_midpoint;

  if(steer<DJ_midpoint-200)            //输出限幅 默认500
      steer=DJ_midpoint-200;
  else if(steer>DJ_midpoint+220)
      steer=DJ_midpoint+220;
  else;
  ATOM_PWM_SetDuty(ATOMSERVO1, steer, 100);//驱动舵机
}
