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


// *************************** 例程硬件连接说明 ***************************
// 接入无线转串口模块
//      模块管脚            单片机管脚
//      RX                  查看 SEEKFREE_BLUETOOTH_CH9141.h 中 BLUETOOTH_CH9141_RX_PIN 宏定义 默认 UART4_TX_P03
//      TX                  查看 SEEKFREE_BLUETOOTH_CH9141.h 中 BLUETOOTH_CH9141_TX_PIN 宏定义 默认 UART4_RX_P02
//      RTS                 查看 SEEKFREE_BLUETOOTH_CH9141.h 中 BLUETOOTH_CH9141_RTS_PIN 宏定义 默认 P07
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源
// *************************** 例程测试说明 ***************************


//蓝牙是需要配对的，如果没有配对请先使用配对上位机进行配对
//蓝牙是需要配对的，如果没有配对请先使用配对上位机进行配对
//蓝牙是需要配对的，如果没有配对请先使用配对上位机进行配对

#define LED P52
uint8 send_buf[] = {0x11, 0x22, 0x33};
uint8 read_buf[10];

uint32 dat_len = 0;

uint8 test_str[] = "seekfree.taobao.com\n";

int main(void)
{
    DisableGlobalIRQ();					//关闭总中断
	
	//sys_clk可选值:30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600。
    //设置系统频率，此频率需要跟STC-ISP软件中的 <输入用户程序运行的IRC频率>选项的频率一致。
    //如果频率设置不对，将会导致串口的数据不正常,PWM的工作不正常等等。
    sys_clk = 30000000;     //设置系统频率为30MHz


    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    delay_ms(300);  //延时300ms，等待主板其他外设上电成功

    bluetooth_ch9141_init();



    EnableGlobalIRQ();
    
    // STC禁止在中断里面发送串口数据。如果在中断里发送串口数据就会卡死。
    // STC禁止在中断里面发送串口数据。如果在中断里发送串口数据就会卡死。
    // STC禁止在中断里面发送串口数据。如果在中断里发送串口数据就会卡死。
    // 发送buf
    bluetooth_ch9141_send_buff(test_str, sizeof(test_str)-1);
    while(1)
    {
        // 翻转LED
        LED = !LED;
        // 读取fifo中的内容
        dat_len = bluetooth_ch9141_read_buff(read_buf, 10);
        // 如果读取到数据
        if(dat_len != 0)
        {
            // 将读取到的fifo发送出去
            bluetooth_ch9141_send_buff(read_buf, (uint16)dat_len);
        }
        delay_ms(500);
    }
}


