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
 
uint8 test1[] = "seekfree wireless to uart test\n";
uint8 test2[] = "seekfree.taobao.com\n";



int main(void)
{
	DisableGlobalIRQ();							//关闭总中断
	board_init();								//初始化内部寄存器
	
    //无线转串口模块相关引脚定义在 wireless.h文件中
    seekfree_wireless_init();
	
	
	EnableGlobalIRQ();							//开启总中断
    while(1)
	{
        seekfree_wireless_send_buff(test1,sizeof(test1)-1);//由于sizeof计算字符串的长度包含了最后一个0，因此需要减一
        seekfree_wireless_send_buff(test2,sizeof(test2)-1);
        delay_ms(100); 
	}
}


