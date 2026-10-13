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
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8H8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-06-01
 ********************************************************************************************************************/

#include "headfile.h"


//board.h文件中FOSC的值设置为0,则程序自动识别系统频率

/*board.h文件中FOSC的值设置不为0，则系统频率为FOSC的值，
在使用stc-isp工具下载程序的时候需要将IRC频率设置为FOSC的值*/

/*在board_init中,已经将P54引脚设置为复位，
如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可*/

uint8  dir = 0;
int16 duty = 0;


void main()
{

	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器，勿删除此句代码。

    pwm_init(PWM1P_P60, 17000, 0); //初始化PWM1  使用P60引脚  初始化频率为17Khz
	pwm_init(PWM2P_P62, 17000, 0); //初始化PWM2  使用P62引脚  初始化频率为17Khz
	
	pwm_init(PWM3P_P64, 17000, 0); //初始化PWM3  使用P64引脚  初始化频率为17Khz
	pwm_init(PWM4P_P66, 17000, 0); //初始化PWM4  使用P66引脚  初始化频率为17Khz
    
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
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
            pwm_duty(PWM1P_P60, duty);       
            pwm_duty(PWM2P_P62, 0);     
                     
			pwm_duty(PWM3P_P64, duty);       
            pwm_duty(PWM4P_P66, 0);  			
        }
        else         //反转
        {
            pwm_duty(PWM1P_P60, 0);          
            pwm_duty(PWM2P_P62, -duty);  
                     
			pwm_duty(PWM3P_P64, 0);          
            pwm_duty(PWM4P_P66, -duty);			
        }
		
    }
}

