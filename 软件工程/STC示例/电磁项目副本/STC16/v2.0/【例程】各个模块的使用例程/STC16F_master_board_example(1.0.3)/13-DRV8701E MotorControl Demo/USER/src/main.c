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
 
uint8  dir = 0;
int16 duty = 0;


#define DIR_1 P64
#define DIR_2 P60
#define PWM_1 PWMA_CH4P_P66
#define PWM_2 PWMA_CH2P_P62

void main()
{
	
	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器


    pwm_init(PWM_1, 17000, 0); //初始化PWM1  使用P60引脚  初始化频率为17Khz
	pwm_init(PWM_2, 17000, 0); //初始化PWM2  使用P62引脚  初始化频率为17Khz
	
	gpio_mode(P6_4, GPO_PP);
	gpio_mode(P6_2, GPO_PP);

	EnableGlobalIRQ();		//开启总中断
	while (1)
    {
        //此处编写需要循环执行的代码
 
		if(dir)                                 
        {
            duty++;
            if(PWM_DUTY_MAX<=duty)   dir = 0;
        }
        else                                    
        {
            duty--;
            if(-PWM_DUTY_MAX>=duty)   dir = 1;
        }
		
        delay_ms(1);
       
        if(duty >= 0) //正转
        {
			DIR_1 = 0;
			pwm_duty(PWM_1, duty);
			
			DIR_2 = 0;
			pwm_duty(PWM_2, duty);   
        }
        else         //反转
        {
			DIR_1 = 1;
			pwm_duty(PWM_1, -duty);
			
			DIR_2 = 1;
			pwm_duty(PWM_2, -duty);  		
        }
	}	
}