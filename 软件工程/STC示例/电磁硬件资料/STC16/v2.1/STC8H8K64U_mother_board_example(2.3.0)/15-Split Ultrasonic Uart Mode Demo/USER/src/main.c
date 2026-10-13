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


uint16 distance;
//该函数在isr.c文件中的UART3_Isr函数进行回调
void Split_Ultrasonic_callback(uint8 buf)
{
    static uint8 dat[3];
    static uint8 num = 0;
    
    dat[num] = buf;
    if(dat[0] != 0xa5)  num = 0;//检查头帧是否正确，不正确就重新接收
    else                num++;

    if(num==3)                  //接收完成，开始处理数据
    {
        num = 0;
        //将数据的高八位与低八位进行组合，得到距离信息单位MM
        distance = dat[1]<<8 | dat[2];
    }
}

void main()
{

	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器，勿删除此句代码。


    //数据接收在isr.c文件中的UART3_Isr函数进行数据接收
    uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 115200, TIM_2);
    
    
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
        delay_ms(100);
        printf("%dmm\r\n", distance); 
    }
}

