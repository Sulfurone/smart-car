#include"event.h"
/*此页代码用不到*/



/******************识别与判断标志***************************/
/*丢线标志*/
uint8 all_lost=0;               //全丢标志
uint8 lost_left[60];            //左丢线标志
uint8 lost_right[60];           //右丢线标志
/*起停*/
uint8 KEY_start_flag=0;         //一键启动标志
uint16 KEY_start_time_flag=0;   //启动时间标志
uint16 stop_flag=0;//停车标志
uint16 stop_time_flag=0;//停车时间标志
uint16 stop_realy=0;//真实停车标志


/*三叉*/
uint8 Tri_count=0;              //三叉计数标志
uint8 Tri_flag=0;               //三叉初始识别标志
uint8 Tri_delay_flag=0;         //三叉弛懈标志
uint8 Tri_retest_flag=0;        //三叉重复检测标志
uint8 Tri_retest_flag1=0;       //三叉重复检测标志1
uint16 Tri_time_flag=0;         //三叉计时变量
uint8 Tri_duty_flag=0;          //三叉准确识别标志
uint8 Tri_out_flag=0;           //出三叉标志
uint8 Tri_delay_flag1=0;   //防止重复识别三叉
/*坡道标志*/
uint8 uphill_flag=0;            //坡道识别标志
uint8 uphill_delay_flag=0;      //坡道弛懈标志
uint8 uphill_duty_flag=0;       //坡道准确识别标志
uint8 uphill_time_flag=0;       //坡道计时变量
float point_K=0;               //远点斜率*10


/*十字*/
uint8 lost_flag=0;              //全丢线初始识别标志
uint8 lost_delay_flag=0;        //十字弛懈标志
uint16 lost_car_time_flag=0;    //十字计时变量
uint8 lost_duty_flag=0;         //全丢线准确识别标志(十字)
/*环岛预判断*/
uint8 cur_L_ready_flag=0;         //左环岛预判断初始识别标志
uint8 cur_L_ready_delay_flag=0;   //左环岛预判断弛懈标志
uint16 cur_L_ready_time_flag=0;   //左环岛预判断计时变量
uint8 cur_L_ready_rest_flag=0;    //左环岛预判断复位变量
uint8 cur_R_ready_flag=0;         //右环岛预判断初始识别标志
uint8 cur_R_ready_delay_flag=0;   //右环岛预判断弛懈标志
uint16 cur_R_ready_time_flag=0;   //右环岛预判断计时变量
uint8 cur_R_ready_rest_flag=0;    //右环岛预判断复位变量
/*环岛准确识别*/
uint8 cur_L_real_flag=0;          //左环岛准确判断识别标志
uint8 cur_L_real_delay_flag=0;    //左环岛准确弛懈识别标志
uint8 cur_L_real_rest_flag=0;     //左环岛准确复位识别标志
uint16 cur_L_real_time_flag=0;    //左环岛准确弛懈识别标志
uint16 cur_L_real_time_flag1=0;   //左环岛准确弛懈识别标志1

uint8 cur_R_real_flag=0;          //右环岛准确判断识别标志
uint8 cur_R_real_delay_flag=0;    //右环岛准确弛懈识别标志
uint8 cur_R_real_rest_flag=0;     //右环岛准确复位识别标志
uint16 cur_R_real_time_flag=0;    //右环岛准确弛懈识别标志
uint16 cur_R_real_time_flag1=0;   //右环岛准确弛懈识别标志1






//                      下一部分：事件管理部分
/*******************************************************************************/
void EVENT_Duty(void)
{
  /************任务时钟*****************/
  Tri_time_flag++;

//  if(uphill_time_flag>200)uphill_time_flag=200;else;
  lost_car_time_flag++;
  cur_R_ready_time_flag++;
  cur_R_real_time_flag++;
  cur_L_ready_time_flag++;
  cur_L_real_time_flag++;
  Tri_delay_flag1++;
  /************停车计时*****************/
  if(stop_flag)
      stop_time_flag++;
  if(stop_time_flag>200)
  {
      stop_flag=0;//中断清零
      stop_time_flag=0;//计时清零
  }
  if(stop_time_flag == 1)
  {
      stop_realy++;//起跑线触发次数
  }
  /************启动计时*****************/
  if(KEY_start_flag)KEY_start_time_flag++;
  else;
  if(KEY_start_time_flag>5000) KEY_start_time_flag=5000;
  else;
  /************坡道部分*****************/
    if(uphill_flag || uphill_delay_flag)
    {
      uphill_delay_flag=1;
      uphill_duty_flag=1;
      uphill_time_flag++;
      if(uphill_time_flag>100)
        uphill_delay_flag=0;
      else;
    }
    else
    {
      uphill_delay_flag=0;
      uphill_duty_flag=0;
      uphill_time_flag=0;
    }
    /*************三叉部分*****************/
    if((Tri_flag &&point_len[15]<30&& !uphill_duty_flag && !lost_duty_flag) || Tri_delay_flag)//三叉标志，非坡道，非十字
    {
      Tri_out_flag=0;
      Tri_delay_flag=1;
      Tri_duty_flag=1;

      if(Tri_flag==0)                                             //三叉识别消失
        Tri_retest_flag=1;
      else;
      if(Tri_retest_flag&&Tri_flag&& Tri_delay_flag1>200)                               //三叉再识别
        Tri_retest_flag1=1;
      else;
      if(Tri_flag==0 && Tri_retest_flag1 && Tri_time_flag>40)     //三叉再消失
      {
         Tri_delay_flag=0;
         Tri_out_flag=1;
        // if(KEY_start_time_flag>10)
         Tri_count++;//else;
      }
      else;
    }
    else
    {
      Tri_retest_flag=0;
      Tri_retest_flag1=0;
      Tri_duty_flag=0;
      Tri_time_flag=0;
      Tri_delay_flag1=0;
    }


  /*************十字部分****************/
       if(lost_flag&&point_len[15]>30&& !uphill_duty_flag &&!Tri_delay_flag|| lost_delay_flag)//十字识别,非三叉，非坡道
  {
    lost_delay_flag=1;
    lost_duty_flag=1;
    if((lost_flag==0&&lost_car_time_flag>15)||lost_car_time_flag>30)
    {
      lost_delay_flag=0;
    }else;
  }
  else
  {
    lost_car_time_flag=0;
    lost_duty_flag=0;
  }
  /*************环岛预识别****************/
  /*右预识别*/
  if(cur_R_ready_flag||cur_R_ready_delay_flag)
  { cur_R_ready_delay_flag=1;
  //ImageAddingLine(1, 10, 15, 10, 40);
   // if((!lost_right[20]&&!lost_right[23])&&!lost_left[23]&&!lost_left[25]&&street_len>40&&cur_R_ready_time_flag>10)
       if(!lost_left[23]&&!lost_left[25]&&street_len>40&&cur_R_ready_time_flag>10)
    {cur_R_ready_rest_flag=1;}else;

    if(cur_R_ready_time_flag>300||lost_left[23]||lost_left[25])
    {cur_R_ready_delay_flag=0;cur_R_ready_rest_flag=0;}
    else if(cur_R_ready_rest_flag&&lost_right[23]&&lost_right[25]&&!lost_left[23]&&!lost_left[25]&&street_len>40&&MagneticField>270)
    { cur_R_ready_delay_flag=0;cur_R_ready_rest_flag=0;cur_R_real_flag=1;}else;
  }
  else
  {cur_R_ready_delay_flag=0;
    cur_R_ready_time_flag=0;
    cur_R_ready_rest_flag=0;
  }
  /*左预识别*/
  if(cur_L_ready_flag||cur_L_ready_delay_flag)
  { cur_L_ready_delay_flag=1;
  //ImageAddingLine(1, 10, 15, 10, 40);//补线防止进错弯
    if((!lost_left[23]&&!lost_left[25])&&!lost_right[23]&&!lost_right[25]&&street_len>40&&cur_L_ready_time_flag>10)
    {cur_L_ready_rest_flag=1;}else;

    if(cur_L_ready_time_flag>300||lost_right[23]||lost_right[25])
    {cur_L_ready_delay_flag=0 ;cur_L_ready_rest_flag=0;}
    else if(cur_L_ready_rest_flag&&lost_left[23]&&lost_left[25]&&!lost_right[23]&&!lost_right[25]&&street_len>40&&MagneticField>350)
    { cur_L_ready_delay_flag=0;cur_L_ready_rest_flag=0;cur_L_real_flag=1;}else;
  }
  else
  { cur_L_ready_delay_flag=0;
    cur_L_ready_time_flag=0;
    cur_L_ready_rest_flag=0;
  }
  /*************环岛准确识别****************/
  /*右准确识别*/
  if(cur_R_real_flag||cur_R_real_delay_flag)
  { cur_R_real_flag=0;
    cur_R_real_delay_flag=1;
    if(cur_R_real_time_flag>50&&(lost_left[23]&&lost_left[25]))
      cur_R_real_rest_flag=1;else;
    if(cur_R_real_rest_flag)cur_R_real_time_flag1++;else;
    if(cur_R_real_time_flag1>100)cur_R_real_time_flag=100;else;
    if(cur_R_real_time_flag1>25&&!lost_left[23]&&!lost_left[25]&&!lost_right[23]&&!lost_right[25])
    {cur_R_real_delay_flag=0;}else;
  }
  else
  {cur_R_real_delay_flag=0;
    cur_R_real_time_flag=0;
    cur_R_real_time_flag1=0;
    cur_R_real_rest_flag=0;
  }
  /*左准确识别*/
  if(cur_L_real_flag||cur_L_real_delay_flag)
  { cur_L_real_flag=0;
    cur_L_real_delay_flag=1;
    if(cur_L_real_time_flag>50&&(lost_right[23]&&lost_right[25]))//出环岛检测
      cur_L_real_rest_flag=1;else;
    if(cur_L_real_rest_flag)cur_L_real_time_flag1++;else;
    if(cur_L_real_time_flag1>100)cur_L_real_time_flag1=100;
    if(cur_L_real_time_flag1>25&&!lost_left[23]&&!lost_left[25]&&!lost_right[23]&&!lost_right[25])
    {cur_L_real_delay_flag=0;}else;
  }
  else
  {cur_L_real_delay_flag=0;
    cur_L_real_time_flag=0;
    cur_L_real_time_flag1=0;
    cur_L_real_rest_flag=0;
  }
}





/*********************三叉计数识别**************************/
uint8 Tri_count_duty(void)
{
  if(1)//三叉左右选择
  {
    if(Tri_count==0)return 0;
    else if(Tri_count)return 1;
    else return 0;
  }else
  {
    if(Tri_count==0)return 1;
    else if(Tri_count>=1)return 0;
    else return 1;
  }
}



