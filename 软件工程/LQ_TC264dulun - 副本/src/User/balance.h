/*
 * balance.h
 *
 *  Created on: 2022年12月20日
 *      Author: Xiaxinyi
 */

#ifndef SRC_USER_BALANCE_H_
#define SRC_USER_BALANCE_H_

extern volatile char mutexCpu0TFTIsOk;         // CPU1 0占用/1释放 TFT
extern int Target_Speed;


typedef enum {
    idle = 0,       //0:空闲-原地直立
    garage_out,     //1:出库
    run,            //2:正常循迹
    garage_in       //3:入库
}Status_car;

typedef enum {
    nothing=0,      //0:平常路段
    straight,       //1:直道
    round_L,        //2:左环岛
    round_L_Out,      //3:出环岛
    round_R,        //4:右环岛
    cross,          //5:十字
    zebra           //6:斑马线
}Element;
extern Element element;                         //赛道元素状态
extern Status_car status_car;                   //小车的状态：出库 循迹 入库

extern float error_Camera, error_Blance;       //摄像头和电磁误差
#define DIN P22_3
#define DIN_ON  PIN_Write(DIN, 1);         // 启动（刹车线）
#define DIN_OFF PIN_Write(DIN, 0);         // 停止（刹车线）



float R_balance_Control(float Angle,float *Angle_Zero,float Gyro);
float P_balance_Control(float Angle,float Angle_Zero,float Gyro);
float Y_balance_Control(float Angle,float Angle_Zero,float Gyro);
//float Yaw_balance_Control(float Gyro,float Gyro_control);
float Velocity_Control_A(int encoder);
float Velocity_Control_B(int encoder);
float Velocity_Control_C(int encoder);

void Balance_car(void);
void Balance(void);

float yijielvbo_P(float angle_m,float gyro_m);




#endif /* SRC_USER_BALANCE_H_ */
