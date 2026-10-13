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

void main()
{

	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器，勿删除此句代码。
	oled_init();			//初始化oled屏幕


    
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
		oled_p6x8str(0,0,"seekfree.taobao.com");//显示字符串
		oled_uint16(0,1,2333);					//显示一个16位无符号变量
		oled_int16(0,2,-633);					//显示一个16位有符号变量
        oled_printf_int32(0,3,5693,5);          //显示一个变量  去除无效0位
        oled_printf_float(0,4,56.356,5,2);      //显示一个浮点数 去除无效0位  整数位显示5位  小数位显示2位


		delay_ms(100);
    }
}

