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

void main()
{
	DisableGlobalIRQ();	//关闭总中断
	
	board_init();		//初始化内部寄存器
	ips114_init();		//初始化1.1.4寸ips屏幕
	
	
	EnableGlobalIRQ();	//开启总中断
    while(1)
	{
		ips114_clear(WHITE);									//清屏
        ips114_showstr(0,0,"seekfree.taobao.com"); 				//显示字符串
        ips114_showuint16(0,1,666);                				//显示一个16位无符号整数
        ips114_showint32(0,2,666,3);               				//显示一个32位有符号数  并去除无效0
        ips114_showint32(0,3,-666,3);              				//显示一个32位有符号数  并去除无效0
        ips114_showfloat(0,4,56.35,3,1);           				//显示一个浮点数        并去除整数部分无效0
        //以上函数x坐标设置是以像素为单位   y坐标是以字符为单位


		
		//以下为色彩测试，红绿蓝全屏各显示一秒。
		delay_ms(2000);									//延时1秒
		ips114_clear(RED);
		delay_ms(1000);									//延时1秒
		ips114_clear(GREEN);
		delay_ms(1000);									//延时1秒
		ips114_clear(BLUE);
		delay_ms(1000);									//延时1秒

    }
}