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

uint16 duty;
void main()
{
	
	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器

    //特别提示这里的舵机居中是在没有安装在车模上的时候，才能使用这个居中方式。
    //如果舵机已经安装在车模上，切勿使用本程序，因为舵机由于机械限位，导致可活动范围小很多。

	//pwm初始化频率为50hz 舵机居中
	duty = 1.5*10000/20;

	pwm_init(PWMB_CH1_P74, 50, duty);
	
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
		//计算舵机位置舵机位置   （0.5ms - 2.5ms）ms/20ms * 10000（10000是PWM的满占空比时候的值）
        //舵机最小值为250，   最大值为1250。
		
		//修改CMT_PRECISON的值，可以修改舵机最大最小值。
		//例：将CMT_PRECISON设置为1000u，则舵机最小值为25，最大值为125。
		//CMT_PRECISON的值在MK60_cmt.h的文件中定义。
		
        duty++;
        if(1250 < duty) duty = 250;
        
        //控制舵机达到指定位置
        pwm_duty(PWMB_CH1_P74, duty);
        if(250 == duty) delay_ms(1000); //当从1250的位置   回转到250的位置时   应该使用较长的延时等到舵机达到指定位置
        else            delay_ms(5);
    }
}