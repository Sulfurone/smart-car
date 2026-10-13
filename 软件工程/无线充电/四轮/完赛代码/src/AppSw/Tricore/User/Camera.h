#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"include.h"
#define white 0xff
#define black 0

extern uint8 above_arr[LCDW];
/*调控参量*/
extern uint8 img_y_max;             //扫描纵坐标最近值
extern uint8 img_y_min;             //扫描纵坐标最远值
extern uint8 img_y_control;         //扫描纵坐标控制值
/*传递参量*/
extern uint8 img_x;                 //扫描横坐标
extern uint8 img_y;                 //扫描纵坐标
extern uint8 start_point;           //扫描起始点
extern uint8 mid_point[60];            //提取的中线
extern unsigned char mid_line[60];
extern uint8 mid_point_His[10];        //历史的中线
extern uint8 left_point[60];           //左边界
extern uint8 right_point[60];          //右边界
extern uint8  leftState ;               //左边界单调
extern uint8  rightState ;             //右边界单调
extern uint8 init_len[60];             //初始化赛道宽度
extern uint8 point_len[60];            //实时赛道宽度
extern uint8 street_len;             //直道长度
extern uint8 len_His[10];              //直道长度历史数组
/*环岛补线*/
extern float L_Cur_K;                //左环岛补线斜率
extern float R_Cur_K;                //由环岛补线斜率
extern int lx11,ly11,lx12,ly12;
extern uint8 lx1,ly1,rx1,ry1,lx2,ly2,rx2,ry2;
extern int lx11,ly11,lx12,ly12;
extern int left_lost_flag;
extern int right_lost_flag ;
extern uint8 mend_flag;
extern uint8 right_count;
extern uint8 left_count;
extern uint16 mid_white;
extern uint16 mid_white1;
extern uint16 mid_white2;
extern uint16 zuo_white;
extern uint16 you_white;
extern uint8 shang_white;
extern uint8   cross_white[LCDW];
extern uint8   ru_num;
extern uint8   cross_zuowhite[LCDH];
extern uint8   cross_youwhite[LCDH];
extern uint8   cross_hangwhite[LCDH];
extern uint8 podao_flag;
extern int shizi_flag;
extern int shizi_count;
extern int sancha_flag;
extern int  sanchastate;
extern uint8 sancha_count;
extern uint16 Left_huandao1  ;      //下面会用到值去判断,为什么不直接去判断,直接判断在编译过程中就会出现地址分配异常
extern uint16 Left_huandao2  ;
extern uint16 Left_huandao3  ;
extern uint16 Right_huandao1;
extern uint16 Right_huandao2 ;
extern uint16 Right_huandao3 ;
extern uint8 Right_huandao4;

extern int sumR;
extern uint8 Rightzhiline;
extern uint8 Leftzhiline;

extern int lefthuandao_flag;
extern int leftstate;
extern uint8 garage_count ;
extern uint8 white_black ;
extern uint8 black_white  ;
extern uint8 garage_flag;
extern uint8 zebra_count;
extern uint8 ruku_flag;

extern int podaostate;
extern int huandaoshibie;
extern int chekushibie;

extern uint8 righthuandao_flag;
extern uint8 rightstate;

extern uint8 garage_delay;

extern float B,A;

void len_init(void);                   //赛道宽度初始化
void street_duty(void);                //赛道直道长度测量
uint8 PointL_DUTY(uint8 L_Start,uint8 L_y);               //左边界提取
uint8 PointR_DUTY(uint8 R_Start,uint8 R_y);              //右边界提取
void mid_Point_DUTY(uint8 Mid_len,uint8 Mid_y);          //边线处理函数 补线算法

void crossroads_dispose(void);
void portrait_line(void);
void Horizontal_line(void);
int Lost_line_left(void);
int Lost_line_right(void);
void roadabout_dispose();
void roadjunction_dispose();
void saidaoyuansu();
void misslinecount(void);
void saidaokuandu(void);
void podao_dispose(void);
void garage__dispose(void);
void Straight_line_judgment(void);
void zhongxianlianxv(void);
int regression(int startline,int endline);
#endif
