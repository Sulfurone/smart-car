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

uint16 data1,data2,data3,data4;

void main()
{

	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器，勿删除此句代码。
    
	adc_init(ADC_P00,0);    //P00引脚		具体通道与引脚对应关系可以查看zf_adc.h文件
	adc_init(ADC_P01,0);    //P01引脚		具体通道与引脚对应关系可以查看zf_adc.h文件
	adc_init(ADC_P05,0);    //P05引脚		具体通道与引脚对应关系可以查看zf_adc.h文件
	adc_init(ADC_P06,0);    //P06引脚		具体通道与引脚对应关系可以查看zf_adc.h文件
    
    //	lcd_init();			//1.8寸TFT初始化
	ips114_init();			//初始化1.14寸IPS屏幕
    //	oled_init_spi();	//OLED初始化
    
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
        data1 = adc_once(ADC_P00,ADC_12BIT);  	//采集ADC_P00电压，精度12位
		data2 = adc_once(ADC_P01,ADC_10BIT); 	//采集ADC_P01电压，精度10位
		data3 = adc_once(ADC_P05,ADC_8BIT);		//采集ADC_P05电压，精度8位
		data4 = adc_once(ADC_P06,ADC_8BIT);		//采集ADC_P06电压，精度8位
		
		//在TFT上显示，需要初始化1.8寸TFT屏幕，才能使用。
//		lcd_showstr(0,0,"data1:");	
//		lcd_showuint16(6*8,0,data1);
//		lcd_showstr(0,1,"data2:");	
//		lcd_showuint16(6*8,1,data2);
//		lcd_showstr(0,2,"data3:");	
//		lcd_showuint16(6*8,2,data3);
//		lcd_showstr(0,3,"data4:");	
//		lcd_showuint16(6*8,3,data4);
		
		//在1.14IPS屏幕上显示，需要初始化1.14寸ips屏幕，才能使用。
		ips114_showstr(0,0,"data1:");	
		ips114_showuint16(6*8,0,data1);
		ips114_showstr(0,1,"data2:");	
		ips114_showuint16(6*8,1,data2);
		ips114_showstr(0,2,"data3:");	
		ips114_showuint16(6*8,2,data3);
		ips114_showstr(0,3,"data4:");	
		ips114_showuint16(6*8,3,data4);

		//在OLED屏幕上显示，需要初始化OLED屏幕，才能使用。
//		oled_p6x8str_spi(0,0,"data1:");	
//		oled_uint16_spi(6*8,0,data1);
//		oled_p6x8str_spi(0,1,"data2:");	
//		oled_uint16_spi(6*8,1,data2);
//		oled_p6x8str_spi(0,2,"data3:");	
//		oled_uint16_spi(6*8,2,data3);
//		oled_p6x8str_spi(0,3,"data4:");	
//		oled_uint16_spi(6*8,3,data4);
		
    }
}

