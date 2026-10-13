#ifndef __IMU_H
#define __IMU_H

#include "math.h"
#include "LQ_IIC_Gyro.h"
#include "LQ_SOFTI2C.h"
void Euler(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);
void Gyro_Init();  //³õÊ¼»¯ÍÓÂÝÒÇ
float junzhi(void);

#endif








