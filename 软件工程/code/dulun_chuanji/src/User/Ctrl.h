

#ifndef SRC_USER_CTRL_H_
#define SRC_USER_CTRL_H_

#include "LQ_MotorServo.h"
#include "LQ_GPT12_ENC.h"
#include "IMU.h"
#include "LQ_ICM20602.h"
#include "LQ_CCU6.h"
#include "PID.h"

extern float Accel_x,Accel_y,Accel_z,Gyro_x,Gyro_y,Gyro_z;

extern float Pitch1,Roll1;
//角速度环变量
void PID_Init();
void Balance_All();
void Palst_loop();
void Angle_loop();
void Speed_loop();
void Vec_Ctrl();
void Palst_X_loop();
void Balance_Ctrl();
void Turn_loop();
void ZhuanXiang();
float dongtai_Control(float Angle);
#endif 
