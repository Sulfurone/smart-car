#ifndef _SERVO_H_
#define _SERVO_H_

#include"include.h"

void DJ_PID(unsigned char midpoint_in);  //舵机PD

extern int san;
extern sint16 midpoint_error;   //中线误差
extern sint16 lastpoint_error;   //上次误差
extern uint32 DJ_midpoint;                //舵机中值
extern uint32 DJ_PWM;                          //输出PWM
extern uint8 lost_flag;              //全丢线初始识别标志
extern uint8 lost_duty_flag;         //全丢线准确识别标志(十字)
extern uint8 cur_L_real_delay_flag;    //左环岛准确弛懈识别标志
extern uint8 cur_R_real_delay_flag;    //右环岛准确弛懈识别标志

extern uint8  KP_A,KP_B,KD;            //MAIN舵机PID
extern float Kd_pu;   //0   4.2
 extern float Kp_pu;  //4.5  2.8
#endif
