#ifndef SWJ_H_
#define SWJ_H_
#endif /* SWJ_H_ */
#include <IfxCpu.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <stdio.h>

#include "src/APP/LQ_ADC_test.h"
#include "src/APP/LQ_Atom_Motor.h"
#include "src/APP/LQ_CAMERA.h"
#include "src/APP/LQ_MPU6050_DMP.h"
#include "src/APP/LQ_EEPROM_TEST.h"
#include "src/APP/LQ_FFT_TEST.h"
#include "src/APP/LQ_GPIO_ExInt.h"
#include "src/APP/LQ_GPIO_KEY.h"
#include "src/APP/LQ_GPIO_LED.h"
#include "src/APP/LQ_GPT_mini512.h"
#include "src/APP/LQ_I2C_9AX.h"
#include "src/APP/LQ_I2C_VL53.h"
#include "src/APP/LQ_ICM20602.h"
#include "src/APP/LQ_OLED096.h"
#include "src/APP/LQ_STM_Timer.h"
#include "src/APP/LQ_TFT18.h"
#include "src/APP/LQ_Tim_InputCature.h"
#include "src/APP/LQ_Tom_Servo.h"
#include "src/APP/LQ_UART_Bluetooth.h"
#include "src/Driver/include.h"
#include "src/Driver/LQ_STM.h"
#include "src/Driver/LQ_UART.h"
#include "src/User/LQ_MotorServo.h"

#include <LQ_IIC_Gyro.h>
#include "LQ_TFT2.h"


//串口

void put_int32(uint8 name, int dat);
void put_float(uint8 name, float dat);
void sendimg( uint8 *image, uint8 width, uint8 height);
//void sendimg_line( uint8 *image, uint8 width, uint8 height,uint8 *line);
void sendimg_zoom(uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height);
void sendimg_binary(uint8* image, uint8 width, uint8 height,uint8 otu);
void sendimg_binary_zoom(uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height,uint8 otu);
void sendimg_binary_CHK(uint8* image, uint8 width, uint8 height,uint8 otu,uint8 chk);
void sendline_clear( uint8 color,uint8 width, uint8 height);
void sendline_xy( uint8 *line_zx,uint8 *line_yx, uint32 len);
void sendline( uint8 color,uint8 *buff, uint32 len);
void sendline2( uint8 color,uint8 *linex,uint8 *liney, uint32 len);
//wifi
void sendimg_WIFI(uint8* image, uint8 width, uint8 height);
void sendimg_WIFI_binary_CHK(uint8* image, uint8 width, uint8 height,uint8 otu,uint8 chk);
void put_int32_WIFI(uint8 name, int dat);
void put_float_WIFI(uint8 name, float dat);
//void sendimg_WIFI_ZOOM( uint8* image, uint8 width, uint8 height, uint8 dis_width, uint8 dis_height);

//内部调用
uint16 swj_CRC(uint8 *Buf,uint8 CRC_sta, uint8 CRC_CNT);
//int32 ByteToInt(int8 *ndsk);
float ByteToFloat(unsigned char* byteArry);
