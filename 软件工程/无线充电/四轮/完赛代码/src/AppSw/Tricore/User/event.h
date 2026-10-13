#ifndef _EVENT_H_
#define _EVENT_H_

#include"include.h"

/******************识别与判断标志***************************/
/*丢线标志*/
extern uint8 all_lost;               //全丢标志
extern uint8 lost_left[60];            //左丢线标志
extern uint8 lost_right[60];           //右丢线标志
/*起停*/
extern uint8 KEY_start_flag;         //一键启动标志
extern uint16 KEY_start_time_flag;   //启动时间标志
extern uint16 stop_flag;//停车标志
extern uint16 stop_time_flag;//停车时间标志
extern uint16 stop_realy;//真实停车标志

/*三叉*/
extern uint8 Tri_count;              //三叉计数标志
extern uint8 Tri_flag;               //三叉初始识别标志
extern uint8 Tri_delay_flag;         //三叉弛懈标志
extern uint8 Tri_retest_flag;        //三叉重复检测标志
extern uint8 Tri_retest_flag1;       //三叉重复检测标志1
extern uint16 Tri_time_flag;         //三叉计时变量
extern uint8 Tri_duty_flag;          //三叉准确识别标志
extern uint8 Tri_out_flag;           //出三叉标志

/*坡道标志*/
extern uint8 uphill_flag;            //坡道识别标志
extern uint8 uphill_delay_flag;      //坡道弛懈标志
extern uint8 uphill_duty_flag;       //坡道准确识别标志
extern uint8 uphill_time_flag;       //坡道计时变量
extern float point_K;               //远点斜率*10

/*十字*/
extern uint8 lost_flag;              //全丢线初始识别标志
extern uint8 lost_delay_flag;        //十字弛懈标志
extern uint16 lost_car_time_flag;    //十字计时变量
extern uint8 lost_duty_flag;         //全丢线准确识别标志(十字)
/*环岛预判断*/
extern uint8 cur_L_ready_flag;         //左环岛预判断初始识别标志
extern uint8 cur_L_ready_delay_flag;   //左环岛预判断弛懈标志
extern uint16 cur_L_ready_time_flag;   //左环岛预判断计时变量
extern uint8 cur_L_ready_rest_flag;    //左环岛预判断复位变量
extern uint8 cur_R_ready_flag;         //右环岛预判断初始识别标志
extern uint8 cur_R_ready_delay_flag;   //右环岛预判断弛懈标志
extern uint16 cur_R_ready_time_flag;   //右环岛预判断计时变量
extern uint8 cur_R_ready_rest_flag;    //右环岛预判断复位变量
/*环岛准确识别*/
extern uint8 cur_L_real_flag;          //左环岛准确判断识别标志
extern uint8 cur_L_real_delay_flag;    //左环岛准确弛懈识别标志
extern uint8 cur_L_real_rest_flag;     //左环岛准确复位识别标志
extern uint16 cur_L_real_time_flag;    //左环岛准确弛懈识别标志
extern uint16 cur_L_real_time_flag1;   //左环岛准确弛懈识别标志1

extern uint8 cur_R_real_flag;          //右环岛准确判断识别标志
extern uint8 cur_R_real_delay_flag;    //右环岛准确弛懈识别标志
extern uint8 cur_R_real_rest_flag;     //右环岛准确复位识别标志
extern uint16 cur_R_real_time_flag;    //右环岛准确弛懈识别标志
extern uint16 cur_R_real_time_flag1;   //右环岛准确弛懈识别标志1



uint8 Tri_count_duty(void);      //事件管理部分
void EVENT_Duty(void);           //三叉计数识

#endif

