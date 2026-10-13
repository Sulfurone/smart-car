/*
 * button.c
 *
 *  Created on: 2023年4月14日
 *      Author: Xiaxinyi
 */
#include "button.h"
#include "LQ_TFT18.h"
#include "LQ_GPIO_KEY.h"
#include "Ctrl.h"
//char  txt[30];
extern int mid_point[120];            //提取的中线
extern unsigned char Start_Flag;                    //启动标志
extern float Angle_X_Final,Angle_Y_Final;
extern short Duty_L,Duty_R,Duty_X;
extern float Accel_x,Accel_y,Accel_z,Gyro_x,Gyro_y,Gyro_z;
extern gx,gy;
extern PID Palst;  //角速度环结构体
extern PID Angle;  //角度环结构体
extern PID Speed;  //速度环结构体
extern PID Palst_X;  //行进轮角速度环
extern PID Balance1;  //行进轮角度环直立环
extern PID Vec;        //行进轮速度环
extern PID Turn;       //转向环PID
extern float Angle_zero;//动量轮零点
extern float Angle_zero2;
extern unsigned char SW0,SW1,SW2,SW3;
//void page1(void)
//{
//    //TFTSPI_CLS(u16CYAN);            // 清屏蓝绿色
//    //PID_Init(); //PID参数初始化
//    sprintf((char*)txt,"P:%.3f",Palst.Kp);
//    TFTSPI_P8X16Str(5,0,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"I:%.3f",Palst.Ki);
//    TFTSPI_P8X16Str(5,1,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"P:%.3f",Angle.Kp);
//    TFTSPI_P8X16Str(5,2,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"D:%.3f",Angle.Kd);
//    TFTSPI_P8X16Str(5,3,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"P:%.3f",Speed.Kp);
//    TFTSPI_P8X16Str(5,4,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"I:%.3f",Speed.Ki);
//    TFTSPI_P8X16Str(5,5,txt,u16RED,u16BLACK);
//
//    if(KEY_Read(KEY0) == 0 && Start_Flag == 0)
//    {
//        Palst.Kp += 1;
//        while(KEY_Read(KEY0) == 0);
//    }
//    if(KEY_Read(KEY1) == 0 && Start_Flag == 0)
//    {
//        Palst.Kp -= 1;
//        while(KEY_Read(KEY1) == 0);
//    }
//
//    if(KEY_Read(KEY2) == 0 && Start_Flag == 0)
//    {
//        Palst.Ki += 0.5;
//        while(KEY_Read(KEY2) == 0);
//    }
//    if(KEY_Read(KEY0) == 0 && Start_Flag == 1)
//    {
//        Palst.Ki -= 0.5;
//        while(KEY_Read(KEY0) == 0);
//    }
//    if(KEY_Read(KEY1) == 0 && Start_Flag == 1)
//    {
//        Angle.Kp += 0.1;
//        while(KEY_Read(KEY1) == 0);
//    }
//    if(KEY_Read(KEY2) == 0 && Start_Flag == 1)
//    {
//        Angle.Kp -= 0.1;
//        while(KEY_Read(KEY2) == 0);
//    }
//
//}
//void page2(void)
//{
//    //TFTSPI_CLS(u16CYAN);            // 清屏蓝绿色
//    PID_Init(); //PID参数初始化
//    sprintf((char*)txt,"P:%.3f",Palst_X.Kp);
//    TFTSPI_P8X16Str(5,0,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"I:%.3f",Palst_X.Ki);
//    TFTSPI_P8X16Str(5,1,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"P:%.3f",Balance1.Kp);
//    TFTSPI_P8X16Str(5,2,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"D:%.3f",Balance1.Kd);
//    TFTSPI_P8X16Str(5,3,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"P:%.3f",Vec.Kp);
//    TFTSPI_P8X16Str(5,4,txt,u16RED,u16BLACK);
//    sprintf((char*)txt,"I:%.3f",Vec.Ki);
//    TFTSPI_P8X16Str(5,5,txt,u16RED,u16BLACK);
//
//    if(KEY_Read(KEY0) == 0 && Start_Flag == 0)
//    {
//        Balance1.Kp += 0.01;
//        while(KEY_Read(KEY0) == 0);
//    }
//    if(KEY_Read(KEY1) == 0 && Start_Flag == 0)
//    {
//        Balance1.Kp -= 0.01;
//        while(KEY_Read(KEY1) == 0);
//    }
//
//    if(KEY_Read(KEY2) == 0 && Start_Flag == 0)
//    {
//        Balance1.Kd += 1;
//        while(KEY_Read(KEY2) == 0);
//    }
//    if(KEY_Read(KEY0) == 0 && Start_Flag == 1)
//    {
//        Balance1.Kd -= 1;
//        while(KEY_Read(KEY0) == 0);
//    }
//    if(KEY_Read(KEY1) == 0 && Start_Flag == 1)
//    {
//        Vec.Kp += 0.001;
//        while(KEY_Read(KEY1) == 0);
//    }
//    if(KEY_Read(KEY2) == 0 && Start_Flag == 1)
//    {
//        Vec.Kp -= 0.001;
//        while(KEY_Read(KEY2) == 0);
//    }
//
//}



