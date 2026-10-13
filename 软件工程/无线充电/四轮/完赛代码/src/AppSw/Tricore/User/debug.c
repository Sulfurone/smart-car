#include"debug.h"


char txt[16];                  //oled显示替换字符
int start_flag=0;


//                      下一部分：调试界面部分

/********************按键设置行数****************************/
uint32 Button_Duty(uint32 Button_val)     //按键计数函数
{
  if(!KEY_Read(KEY2))                   //按键加
  {
      delayms(10);
    if(!KEY_Read(KEY2))                 //二次判断
    {
      Button_val++;
      OLED_CLS();                   //清屏
      if(Button_val>10000)                //限幅
        Button_val=10000;
      while(!KEY_Read(KEY2));
    }
  }
  if(!KEY_Read(KEY1))                   //按键减
  {
      delayms(10);
    if(!KEY_Read(KEY1))                 //二次判断
    {
      Button_val--;
      OLED_CLS();                   //清屏
      if(Button_val<2)                  //限幅
        Button_val=1;
      while(!KEY_Read(KEY1));
    }
  }
   return Button_val;
}



/*******************调试界面函数**************************/
void GUI_Duty(void)
{
    unsigned char i;
  /*********系统设置***********/
  unsigned char static GUI_flag=0;     //调试界面变量
  unsigned char static display_y=25;   //调试行数变量
  if(!KEY_Read(KEY0))                  //改变界面
  {
    delayms(10);
    if(!KEY_Read(KEY0))                //二次确认
    {
      OLED_CLS();                   //清屏
      GUI_flag++;
      if(GUI_flag>2)                    //设置界面页数
        GUI_flag=0;
      while(!KEY_Read(KEY0));
    }
  }
  /*********调试界面#0***********/
  if(GUI_flag==0)
  {
      OLED_Show_Frame80();//摄像头边线
      OLED_Road(LCDH, LCDW, (unsigned char *)Pixle);        // OLED动态显示摄像头图像
   for(i=img_y_max;i>=img_y_min;i--)
          {
            //OLED_PutPixel((mid_line[i]+24),i);                //oled画中线
//            OLED_PutPixel((left_point[i]),i);
//            OLED_PutPixel((right_point[i]),i);
          }
    display_y=Button_Duty(display_y);           //按键设置行数
    OLED_Rectangle(1, display_y, 23,display_y,0);   //绘制矩形
    OLED_Rectangle(105, display_y, 127,display_y,0);   //绘制矩形

/*这里可以在oled上显示出各种标志位，方便调元素*/

        sprintf(txt, "%d", garage_flag);
        OLED_P6x8Str(112, 0, txt);


           sprintf(txt, "%d",garage_count);
           OLED_P6x8Str(112, 1, txt);


        sprintf(txt, "%d",white_black);
          OLED_P6x8Str(112, 2, txt);

              sprintf(txt, "%d",black_white);
           OLED_P6x8Str(112, 3, txt);

       sprintf(txt, "%d",garage_delay);
           OLED_P6x8Str(112, 4, txt);


  }
  /*********调试界面#1***********/
  else if(GUI_flag==1)
  {

      if(KEY_Read(DSW0))
      {
      DJ_midpoint=Button_Duty(DJ_midpoint);   //舵机中值整定
      OLED_P6x8Str(0,2, "DJ_MID");
      }
      else
      {
      //speed_SW_flag=Button_Duty(speed_SW_flag);   //速度档位选择
      OLED_P6x8Str(0,2, "DJ_PWM");
      KP_A=Button_Duty(KP_A);



      sprintf(txt,"KP_A:%d",KP_A);  //舵机P值
      OLED_P8x16Str(0,6,(float*)txt);
  }

      sprintf(txt, "%d",DJ_PWM);   //OLED显示舵机PWM
      OLED_P6x8Str(64,2, txt);
    /***速度档位选择***/
      OLED_P6x8Str(0,4,"Speed_SET");              //OLED显示设定速度
      sprintf(txt, "%d", speed_SW_flag);   //显示速度选择标志
      OLED_P6x8Str(64,4, txt);

    if(speed_SW_flag>4)speed_SW_flag=1;
    speed_SW();                                 //速度档位选择函数

  }
  else if(GUI_flag==2)
  {
      OLED_P6x8Str(20,2,"START ENGINE!");         //一键启动
    if(!KEY_Read(KEY2))
    {KEY_start_flag=1;}                         //启动标志值置
    else;
    if(!KEY_start_flag)
        OLED_P6x8Str(24,5,"    Loop    ");       //停车
    else if(KEY_start_flag && KEY_start_time_flag<100)
        OLED_P6x8Str(24,5,"    READY   ");       //准备
    //else if(KEY_start_time_flag>100 && !stop_motors_flag)
    else if(KEY_start_time_flag>100)
        OLED_P6x8Str(24,5,"    Voom!    ");      //启动！
    else
        OLED_P6x8Str(24,5,"    STOP    ");       //停车*/
  }
  else;
  /************END**************/
}


