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

#define BAT_VOL_PIN ADC_P15

uint16 ad_result = 0;           //引脚电压
uint16 battery_voltage = 0;     //电池电压
uint32 temp;

void main()
{

	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器，勿删除此句代码。
    
	
	adc_init(BAT_VOL_PIN, ADC_SYSclk_DIV_32);
	
//	lcd_init();							//1.8寸TFT初始化
	ips114_init();						//初始化1.14寸IPS屏幕
//	oled_init_spi();					//OLED初始化
	
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
        ad_result = adc_once(BAT_VOL_PIN, ADC_12BIT);
		temp = (((uint32)ad_result * 5000) / 4096);  //计算出当前adc引脚的电压 计算公式为 ad_result*VCC/ADC分辨率    VCC单位为mv
		battery_voltage =  temp * 4;//根据引脚电压  和分压电阻的阻值计算电池电压 计算公司为   引脚电压*(R2+R3)/R3   R3为接地端电阻
		
		//在TFT上显示，需要初始化1.8寸TFT屏幕，才能使用。
//		lcd_showstr(0, 0, "voltage:");
//		lcd_showuint16(8*8, 0, battery_voltage);
		
		//在1.14IPS屏幕上显示，需要初始化1.14寸ips屏幕，才能使用。
		ips114_showstr(0, 0, "voltage:");
		ips114_showuint16(8*8, 0, battery_voltage);
		ips114_showstr(0, 1, "ADC = ");
		ips114_showuint16(8*8, 1, ad_result);
		
		//在OLED屏幕上显示，需要初始化OLED屏幕，才能使用。
//		oled_p6x8str_spi(0, 0, "voltage:");
//		oled_uint16_spi(8*8, 0, battery_voltage);
		
		//延时100ms
		delay_ms(100);
    }
}

