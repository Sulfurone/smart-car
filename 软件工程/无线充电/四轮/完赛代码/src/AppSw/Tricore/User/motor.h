#ifndef _MOTOR_H_
#define _MOTOR_H_

#include"include.h"
/************************速度控制*******************************/
extern sint8  speed_need;                   //目标速度
extern sint8  speed_need_Boost;             //目标高速
extern sint8  speed_need_normal;            //目标速度
extern sint8  speed_SW_flag;                 //速度选择标志

/**************************编码器********************************/
extern float feed_fix;           //编码器修正系数
extern sint32 Feed_flag;             //编码器累计计数
extern sint32 Feed_speed;            //编码器采集速度

/**************************速度PID参数********************************/
extern float speed_out;         //输出实际速度
extern float speed_error;       //速度误差
extern float Speed_P,Speed_I,Speed_D;


void speed_SW(void);
void FEED_COUNT(void);
void speed_PID(sint8 speed_in);

#endif

