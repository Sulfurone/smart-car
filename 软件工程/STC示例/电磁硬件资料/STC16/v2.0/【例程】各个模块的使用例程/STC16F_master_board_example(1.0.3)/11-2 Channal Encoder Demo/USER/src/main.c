/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 *关于内核频率的设定，可以查看board.h文件
 *在board_init中,已经将P54引脚设置为复位
 *如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */
 //该例程仅支持带方向的编码器解码

//定义脉冲引脚
#define SPEEDL_PLUSE   CTIM0_P34
#define SPEEDR_PLUSE   CTIM3_P04
//定义方向引脚
#define SPEEDL_DIR     P35
#define SPEEDR_DIR     P53

int16 templ_pluse = 0;
int16 tempr_pluse = 0;

void main()
{
	DisableGlobalIRQ();					//关闭总中断
	board_init();						//初始化寄存器
	ctimer_count_init(SPEEDL_PLUSE);	//初始化定时器0作为外部计数
	ctimer_count_init(SPEEDR_PLUSE);	//初始化定时器3作为外部计数
//	lcd_init();							//1.8寸TFT初始化
	ips114_init();						//初始化1.14寸IPS屏幕
//	oled_init_spi();					//OLED初始化
	EnableGlobalIRQ();					//开启总中断
    while(1)
	{
		
        //读取采集到的编码器脉冲数
        templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
		tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

        //计数器清零
        ctimer_count_clean(SPEEDL_PLUSE);
		ctimer_count_clean(SPEEDR_PLUSE);

        //采集方向信息
        if(1 == SPEEDL_DIR)    templ_pluse = -templ_pluse;
        else                   templ_pluse = templ_pluse;
		if(1 == SPEEDR_DIR)    tempr_pluse = -tempr_pluse;
        else                   tempr_pluse = tempr_pluse;
		
		//在TFT上显示，需要初始化1.8寸TFT屏幕，才能使用。
//		lcd_showstr(0, 0, "pluse_l =");			lcd_showint16(8*10, 0, templ_pluse);
//		lcd_showstr(0, 1, "pluse_r =");			lcd_showint16(8*10, 1, tempr_pluse);

		//在1.14IPS屏幕上显示，需要初始化1.14寸ips屏幕，才能使用。
		ips114_showstr(0, 0, "pluse_l =");		ips114_showint16(8*10, 0, templ_pluse);
		ips114_showstr(0, 1, "pluse_r =");		ips114_showint16(8*10, 1, tempr_pluse);
		
		//在OLED屏幕上显示，需要初始化OLED屏幕，才能使用。
//		oled_p6x8str_spi(0, 0, "pluse_l =");	oled_int16_spi(8*10, 0, templ_pluse);
//		oled_p6x8str_spi(0, 1, "pluse_r =");	oled_int16_spi(8*10, 1, tempr_pluse);
		
		//延时100ms
		delay_ms(100);
    }
}

