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
 
int main(void)
{
	DisableGlobalIRQ();					//关闭总中断
	board_init();						//初始化内部寄存器
			
//	lcd_init();							//1.8寸TFT初始化
//	ips114_init();						//初始化1.14寸IPS屏幕
//	oled_init_spi();					//OLED初始化
			
    icm20602_init_simspi();    			//六轴陀螺仪初始化
	
	EnableGlobalIRQ();					//开启总中断
    while(1)
	{
		get_icm20602_gyro_simspi(); 		//获取加速度数据
        get_icm20602_accdata_simspi();    	//获取陀螺仪数据
		
		//在TFT上显示，需要初始化1.8寸TFT屏幕，才能使用。
//		lcd_showstr(0, 0, "acc.x=");
//		lcd_showstr(0, 1, "acc.y=");
//		lcd_showstr(0, 2, "acc.z=");
//		lcd_showstr(0, 3, "gyro.x=");
//		lcd_showstr(0, 4, "gyro.y=");
//		lcd_showstr(0, 5, "gyro.z=");
//		lcd_showint16(6*10, 0, icm_acc_x);
//		lcd_showint16(6*10, 1, icm_acc_y);
//		lcd_showint16(6*10, 2, icm_acc_z);
//		lcd_showint16(6*10, 3, icm_gyro_x);
//		lcd_showint16(6*10, 4, icm_gyro_y);
//		lcd_showint16(6*10, 5, icm_gyro_z);

		printf("acc.z=%d\r\n",  icm_acc_z);
		//在1.14IPS屏幕上显示，需要初始化1.14寸ips屏幕，才能使用。
//		ips114_showstr(0, 0, "acc.x=");
//		ips114_showstr(0, 1, "acc.y=");
//		ips114_showstr(0, 2, "acc.z=");
//		ips114_showstr(0, 3, "gyro.x=");
//		ips114_showstr(0, 4, "gyro.y=");
//		ips114_showstr(0, 5, "gyro.z=");
//		ips114_showint16(6*10, 0, icm_acc_x);
//		ips114_showint16(6*10, 1, icm_acc_y);
//		ips114_showint16(6*10, 2, icm_acc_z);
//		ips114_showint16(6*10, 3, icm_gyro_x);
//		ips114_showint16(6*10, 4, icm_gyro_y);
//		ips114_showint16(6*10, 5, icm_gyro_z);
		
		//在OLED屏幕上显示，需要初始化OLED屏幕，才能使用。
//		oled_p6x8str_spi(0, 0, "acc.x=");
//		oled_p6x8str_spi(0, 1, "acc.y=");
//		oled_p6x8str_spi(0, 2, "acc.z=");
//		oled_p6x8str_spi(0, 3, "gyro.x=");
//		oled_p6x8str_spi(0, 4, "gyro.y=");
//		oled_p6x8str_spi(0, 5, "gyro.z=");
//		oled_int16_spi(6*10, 0, icm_acc_x);
//		oled_int16_spi(6*10, 1, icm_acc_y);
//		oled_int16_spi(6*10, 2, icm_acc_z);
//		oled_int16_spi(6*10, 3, icm_gyro_x);
//		oled_int16_spi(6*10, 4, icm_gyro_y);
//		oled_int16_spi(6*10, 5, icm_gyro_z);
		
		delay_ms(100);
	}
}




