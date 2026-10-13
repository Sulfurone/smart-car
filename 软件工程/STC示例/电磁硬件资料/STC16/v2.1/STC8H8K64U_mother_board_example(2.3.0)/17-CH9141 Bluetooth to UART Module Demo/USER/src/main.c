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

//=======蓝牙灯光说明=========
//1，蓝灯闪烁表示表示当前为从机模式并且未连接
//2，蓝灯不亮表示当前为主机模式并且未连接
//3，蓝灯常亮表示已连接
//4，如果蓝牙连接成功了，进行透传的时候蓝灯会闪烁

//=========主机程序==========
//需要与从机程序配套测试
//需要在初始化函数参数中写入对应地址，如：bluetooth_ch9141_init(MASTER_MODE，"80:90:2C:E4:C2:84,000000");
//查看模块地址方式
//1.使用在线调试	（主机或从机方式都能查看）
//	初始化中bluetooth_ch9141_get_mac_address()函数下一语句加个断点，运行到断点后查看mac_address数组，得到的17位字符即为地址
//2. 使用手机	 （下载从机代码后使用）
//	上电运行后，打开手机BLE调试软件，能搜索到模块的名字，名字下方便是地址
//	模块的名字为初始化中bluetooth_ch9141_set_name函数的参数
//	如果使用手机查看蓝牙的mac地址，则输入的时候请将mac倒置一下
//	例如手机查看到的mac地址为61:62:63:64:65:67，则使用本函数连接的时候应该写
//bluetooth_ch9141_init(MASTER_MODE，"67:65:64:63:62:61,000000");
//地址填写完成后运行代码，如果从机运行中，主机会自动连接从机

//！！运行效果：
//主机持续发送数据，从机接收到数据后会将数据发送回来，主机接收到从机返回的数据后将数据显示到屏幕上
uint8 test1[] = "seekfree bluetooth to uart test\n";
uint8 test2[] = "seekfree.taobao.com\n";
uint8 send_value = 'a';


void main()
{

	DisableGlobalIRQ();		//关闭总中断
	board_init();			//初始化内部寄存器，勿删除此句代码。
    
    delay_ms(300);  //延时300ms，等待主板其他外设上电成功

	//如果是主机，第一个参数写MASTER_MODE，第二个参数写从机的地址和密码
	//如果是从机，第一个参数写SLAVE_MODE, 第二个参数写NULL
    bluetooth_ch9141_init(MASTER_MODE, "22:7D:19:26:3B:38,000000");

    
	EnableGlobalIRQ();		//开启总中断
    while(1)
	{
        bluetooth_ch9141_send_buff(test1, sizeof(test1)-1);//由于sizeof计算字符串的长度包含了最后一个0，因此需要减一
        bluetooth_ch9141_send_buff(test2, sizeof(test2)-1);
        delay_ms(500);
        if(uart_flag)// 接收到数据后 这个变量将会在 bluetooth_ch9141_uart_callback 中置为1
        {
            printf("receive = %c\r\n", uart_data);
            uart_flag = 0;
        }
        
    }
}

