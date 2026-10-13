#ifndef _DEBUG_H_
#define _DEBUG_H_

#include"include.h"



uint32 Button_Duty(uint32 Button_val);     //按键计数函数
void GUI_Duty(void);


/****************LQ_CAMERA中参数重复定义*****************/
extern unsigned char Pixle[LCDH][LCDW];               //二值化后数据
extern char txt[16];                  //oled显示替换字符

extern uint8 img_y_max;             //扫描纵坐标最近值
extern uint8 img_y_min;             //扫描纵坐标最远值
extern uint8 img_y_control;         //扫描纵坐标控制值
extern unsigned char mid_line[60];
extern uint8 mid_point[60];            //提取的中线
extern uint8 point_len[60];            //实时赛道宽度
extern uint8 street_len;             //直道长度
extern int start_flag;

#endif
