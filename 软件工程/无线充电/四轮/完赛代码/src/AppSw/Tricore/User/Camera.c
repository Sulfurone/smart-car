#include"Camera.h"
#include <include.h>
/***********************摄像头有关参数***************************/
/*调控参量*/
uint8 img_y_max=29;             //扫描纵坐标最近值
uint8 img_y_min=10;             //扫描纵坐标最远值
uint8 img_y_control=25;         //扫描纵坐标控制值
/*传递参量*/
uint8 img_x=40;                 //扫描横坐标
uint8 img_y=30;                 //扫描纵坐标
uint8 start_point=40;           //扫描起始点
uint8 mid_point[60];            //提取的中线
unsigned char mid_line[60];
uint8 mid_point_His[10];        //历史的中线
uint8 left_point[60];           //左边界
uint8 right_point[60];          //右边界
 uint8  leftState = 0;
 uint8  rightState = 0;
uint8 init_len[60];             //初始化赛道宽度
uint8 point_len[60];            //实时赛道宽度
uint8 street_len;             //直道长度


uint8 above_arr[LCDW];

uint8   cross_white[LCDW];
uint8   ru_num;
uint8   cross_zuowhite[LCDH];
uint8   cross_youwhite[LCDH];
uint8   cross_hangwhite[LCDH];


int lefthuandao_flag;     //进入左环岛检测标志位
uint8 righthuandao_flag;
int shizi_flag;          //进入十字标志位
int shizi_count=0;

int huandaoshibie=1;//环岛识别********************************************
int shizishibie=0;//十字识别
int chekushibie=0;


uint8 podao_flag;        //进入坡道标志位
int sancha_flag;         //进入三叉路口标志位
uint8 garage_flag;       //车库标志位

int podaostate=0;
int  leftstate=0;         //左环岛阶段标志位
uint8 rightstate=0;
int  shizistate=0;        //十字阶段标志位
int  sanchastate=0;       //三叉路口标志位


uint16 Left_huandao1  ;      //下面会用到值去判断,为什么不直接去判断,直接判断在编译过程中就会出现地址分配异常
uint16 Left_huandao2  ;
uint16 Left_huandao3  ;
uint16 Right_huandao1 ;
uint16 Right_huandao2 ;
uint16 Right_huandao3 ;
uint8 Right_huandao4;
uint16 mid_white;
uint16 mid_white1;
uint16 mid_white2;
uint16 zuo_white;
uint16 you_white;

uint8 Rightzhiline;
uint8 Leftzhiline;
uint8 white_black ;
uint8 black_white  ;
uint8 garage_count=0 ;
uint8 garage_delay=0;
float old_Yaw;
float zhuanjiaozhi;
uint8 right_count;
uint8 left_count;
uint8 san_white;
uint8 biaozhun;
uint8 shang_white;
///拐点数
uint8 lx1,ly1,rx1,ry1,lx2,ly2,rx2,ry2,lx3,ly3,rx3,ry3,lx4,ly4,rx4,ry4;
int lx5,ly5,rx5,ry5,lx6,ly6,rx6,ry6,lx7,ly7,rx7,ry7,lx8,ly8,rx8,ry8,lx13,ly13,rx13,ry13,lx14,ly14,rx14,ry14;
int lx9,ly9,rx9,ry9,lx10,ly10,rx10,ry10,lx11,ly11,rx11,ry11,lx12,ly12,rx12,ry12;
uint8 mend_flag;
uint8 sancha_count=0;
uint8 zebra_count=0;
uint8 ruku_flag=0;//*****************************************************

//                   ！下一部分：图像处理部分！

/************************************************************

【函数名称】lost_line_right
【功    能】右侧图像丢线检查函数
【参    数】无
【返 回 值】-1为未丢线 其他为丢线起始行
【实    例】lost_line_right();
【注意事项】无

************************************************************/

int Lost_line_right(void)
{
  uint8 i;
  for(i=25;i>5;i--)
  if(right_point[i]==79)     return i;
  return -1;
}

/************************************************************

【函数名称】lost_line_left
【功    能】左侧图像丢线检查函数
【参    数】无
【返 回 值】-1为未丢线 其他为丢线起始行
【实    例】lost_line_left();
【注意事项】无

************************************************************/

int Lost_line_left(void)
{
  uint8 i;
  for(i=25;i>5;i--)
  if(left_point[i]==0) return i;
  return -1;
}


/************************************************************

【函数名称】Horizontal_line
【功    能】水平巡线函数
【参    数】无
【返 回 值】无
【实    例】Horizontal_line();
【注意事项】无

************************************************************/

void Horizontal_line(void)
{
  uint8 i,j;
  if(Pixle[LCDH-1][LCDW/2]==0)
  {
    if(Pixle[LCDH-1][5]==0xff)
     mid_point[LCDH]=5;
    else if(Pixle[LCDH-1][LCDW-5]==0xff)
     mid_point[LCDH]=LCDW-5;
    else
     mid_point[LCDH]=LCDW/2;
  }
  else
   mid_point[LCDH]=LCDW/2;
  for(i=LCDH-1;i>0;i--)
  {
    for(j=mid_point[i+1];j>=0;j--)
    {
      if(Pixle[i][j]==0||j==0)
      {
        left_point[i]=j;
        break;
      }
    }
    for(j=mid_point[i+1];j<=LCDW-1;j++)
    {
      if(Pixle[i][j]==0||j==LCDW-1)
      {
        right_point[i]=j;
        break;
      }
    }
   mid_point[i]=(left_point[i]+right_point[i])/2;
   if(Pixle[i-1][mid_point[i]]==0||i==0)
    {
   for(j=i;j>0;j--)
   {
   mid_point[j]=mid_point[i];
    left_point[j]=mid_point[i];
    right_point[j]=mid_point[i];
   }
      break;
    }
  }
}

/************************************************************
【函数名称】portrait_line
【功    能】纵向巡线函数
【参    数】无
【返 回 值】无
【实    例】portrait_line();
【注意事项】无
***********************************************************/


void portrait_line(void)
{
  uint8 i,j;
  for(i=0;i<LCDW;i++)
  {
    for(j=LCDH-1;j>=0;j--)
    {
      if(Pixle[j][i]==0||j==0)
      {
        above_arr[i]=j;
        break;
      }
    }
  }
}

void connect_line(uint8 x1,uint8 y1,uint8 x2,uint8 y2)//补线函数，特殊元素需要补线（即手动补一个边界）
{

   short i,j,swap;
  float k;
  if(y1>y2)
  {
    swap = x1;
    x1 = x2;
    x2 = swap;
    swap = y1;
    y1 = y2;
    y2 = swap;
  }
  if(x1==x2)
  {
    for(i=y1;i<y2+1;i++)
//      a[i]=y1++;
    Pixle[i][x1]=0;
  }
  else if(y1==y2)
  {
    for(i=x1;i<x2+1;i++)
//      a[i]=x1++;
    Pixle[y1][i]=0;
  }
  else
  {
    k = ((float)x2-(float)x1)/((float)y2-(float)y1);
    for(i=y1;i<=y2;i++)
    Pixle[i][(short)(x1+(i-y1)*k)]=0;


  }


}

/************************************************************

【函数名称】Straight_line_judgment
【功    能】直线判断函数
【参    数】arr为传入数组
【返 回 值】1为直线 0为非直线
【实    例】Straight_line_judgment(left_arr);
【注意事项】只判断20-55
并不是很好用
************************************************************/
int sumR=0;


void Straight_line_judgment(void)
{
  int i;

  for(i=30;i>15;i--)
      {
         if((right_point[i]-right_point[i-1]<3)&&right_point[i]-right_point[i-1]>0)
          sumR++;
      }
      if(sumR>14)
      {  Rightzhiline=1;
          sumR=0;   }
      else
          Rightzhiline=0;

}


void cunbaidian(void)//每一列的白色点的数量，每一行的白色点的数量，用来判断赛道元素
{
  for(uint8 lie=0;lie<LCDW;lie++)
  {
    cross_white[lie]=0;
        ru_num=0;
  }
    //  //存白点 列
    for(uint8 lie=0;lie<LCDW;lie++)
  {
    for(uint8 hang=LCDH-1;hang>0;hang--)//40
    {
      if(Pixle[hang][lie]==1)
      {
        cross_white[lie]++;
      }
    }

    }

/////////////////行左 白点

      for(uint8 hang=0;hang<LCDH;hang++)
  {
    cross_zuowhite[hang]=0;
  }

  for(uint8 hang=LCDH-1;hang>0;hang--)
  {
    for(uint8 lie=0;lie<LCDW/2;lie++)
    {
      if(Pixle[hang][lie]==1)
      {
        cross_zuowhite[hang]++;
      }
    }
    }

/////////////////行右 白点

      for(uint8 hang=0;hang<LCDH;hang++)
  {
    cross_youwhite[hang]=0;
  }

  for(uint8 hang=LCDH-1;hang>0;hang--)
  {
    for(uint8 lie=LCDW-1;lie>LCDW/2;lie--)
    {
      if(Pixle[hang][lie]==1)
      {
        cross_youwhite[hang]++;
      }
    }
    }


//////////////////////////////行白点
      for(uint8 hang=0;hang<LCDH;hang++)
  {
    cross_hangwhite[hang]=0;
  }

      //存白点        行
  for(uint8 hang=LCDH-1;hang>0;hang--)
  {
    for(uint8 lie=0;lie<LCDW;lie++)
    {
      if(Pixle[hang][lie]==1)
      {
        cross_hangwhite[hang]++;
      }
    }
    if(hang<50)
    {
      if(cross_hangwhite[hang]>110)
        ru_num++;
    }
  }
}

void misslinecount(void)
{
    right_count = 0;
    left_count  = 0;

    for(uint8 i=28;i>10;i--)
    {
        if(left_point[i]<1)
        {
            left_count++;
        }
        if(right_point[i]>75)
        {
            right_count++;
        }
    }
}
/************************************************************

【函数名称】saidaokuandu
【功    能】赛道宽度测量
【参    数】无
【返 回 值】无
【实    例】
【注意事项】无

************************************************************/
void saidaokuandu(void)
{
    for(img_y=29;img_y>=10;img_y--)
    {
        point_len[img_y]=right_point[img_y]-left_point[img_y];

    if(left_point[img_y]<5)          lost_left[img_y]=1;           //左丢线判断
    else lost_left[img_y]=0;

    if(right_point[img_y]>75)      lost_right[img_y]=1;        //右丢线判断
    else lost_right[img_y]=0;
  }
    }
/************************************线性回归计算中线斜率************************************/
// y = Ax+B
float B,A;
int regression(int startline,int endline)
{
    float SumUp=0,SumDown=0,avrX=0,avrY=0;
  int i=0,SumX=0,SumY=0,SumLines = 0;

  SumLines=endline-startline;   // startline 为开始行， //endline 结束行 //SumLines

  for(i=startline;i<endline;i++)
  {
    SumX+=mid_point[i];
    SumY+=i; //这里Middle_black为存放中线的数组
  }
  avrX=SumX/SumLines;     //X的平均值
  avrY=SumY/SumLines;     //Y的平均值
  SumUp=0;
  SumDown=0;
  for(i=startline;i<endline;i++)
  {
    SumUp+=(mid_point[i]-avrY)*(i-avrX);
    SumDown+=(i-avrX)*(i-avrX);
  }
  if(SumDown==0)
    B=0;
  else
    B=(int)(SumUp/SumDown);
    A=(SumY-B*SumX)/SumLines;  //截距
    //return B;  //返回斜率
}

/************************************************************

【函数名称】garage__dispose
【功    能】入库处理函数
【参    数】无
【返 回 值】无
【实    例】garage__dispose();
【注意事项】无

************************************************************/


void garage__dispose(void)//识别斑马线，识别黑白跳变达到阈值即为斑马线标志
{
    white_black = 0 ;
    black_white = 0 ;
    //garage_count= 0 ;

    ///////////车库在左边，入库
    for(uint8 hang = 35;hang>15; hang-- )
     {
         for(uint8 lie = right_point[left_lost_flag+5];lie>10;lie--)
          {
                    if(Pixle[hang][lie]==1&&Pixle[hang][lie-2]==0)
                    {
                        white_black++;
                    }

                    if(Pixle[hang][lie]==0&&Pixle[hang][lie-2]==1)
                    {
                        black_white++;
                    }
            }

            if(white_black>15&&black_white>15)
            {
                garage_count++;
                     if(garage_count>4)
                        {
                            garage_flag = 1;

                            garage_delay=1;

                            shizi_flag = 0;//关闭十字识别
                            lefthuandao_flag=0;
                            leftstate=0 ;
                          }
              }
     }




             if(garage_flag&&garage_delay>20)
                {
                    zebra_count++;
                    garage_count=0 ;
                    garage_flag=0;
                    garage_delay=0;
                    chekushibie=0;//关闭车库识别
                    huandaoshibie=1;//开启识别圆环
                    Horizontal_line();
                    }

           if(zebra_count&&garage_flag)
                            ruku_flag=1;


}


void saidaoyuansu()
{
    cunbaidian();


    san_white = (cross_hangwhite[20]+cross_hangwhite[21]+cross_hangwhite[22])/3 ;  //
    zuo_white = (cross_zuowhite[20]+cross_zuowhite[21]+cross_zuowhite[22])/3;
    you_white = (cross_youwhite[20]+cross_youwhite[21]+cross_youwhite[22])/3;
    biaozhun  =  cross_hangwhite[1];
    mid_white =  cross_white[40];          //区分十字和三叉路口
    mid_white1=cross_white[41];
    mid_white2=cross_white[39];
    shang_white = cross_hangwhite[10];//10
    Right_huandao2=cross_white[65];//    Right_huandao2=cross_white[70];
    Right_huandao3=cross_white[10];

    if((!lefthuandao_flag)&&(!righthuandao_flag)&&(!sancha_flag)&&(!shizi_flag)&&(!podao_flag))
    {
        if(chekushibie)
        garage__dispose();

    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    if((!lefthuandao_flag)&&(!righthuandao_flag)&&(!sancha_flag)&&(!shizi_flag)&&(!garage_flag)&&(!podao_flag))  //元素都没有识别到，开始识别元素
{
    if(right_count>10||left_count>10)  // 两边有丢线情况 进入元素识别
    {

        ////////////////////////////////////坡道
             if(point_len[10]>40&&point_len[20]>60)
             {
                 podao_flag = 1;
                 podaostate=1;
                 shizishibie++;
                 chekushibie++;
             }
        ////////////////////////////////////十字
        if(right_lost_flag !=-1&&left_lost_flag!=-1&&mid_white>47&&right_count>10&&left_count>10)
        {
            if(shizishibie>0)
                shizi_flag = 1;

        }

        ////////////////////////////////////左环岛元素

//        if((right_lost_flag==-1||right_lost_flag>45)&&left_lost_flag!=-1)  //左边丢线，右边不丢
//            {
//                 if(mid_white>48&&Right_huandao2<44&&right_count<5)//区别十字,斜十字
//                    {
//                        if(left_lost_flag<40)
//                        {
//                            if(left_count>10&&left_count<25&&Right_huandao2>34)                 //区分小S弯
//                            {
//                                if((right_point[20]-right_point[15]<13)&&(right_point[25]-right_point[20]<13))      // 两点距离 ，证明右边是直线则是环岛
//                                {
//                                    if(huandaoshibie>0)//过三叉之后开启环岛识别
//                                    {
//                                        lefthuandao_flag=1;                   //进入左环岛标志位
//                                        leftstate=1 ;                          //左环岛状态1
//                                    }
//                                }
//                            }
//                        }
//                    }
//             }

        ////////////////////////////////////右环岛元素

        if((left_lost_flag==-1||left_lost_flag>24)&&right_lost_flag!=-1)  //右边丢线，左边不丢
    {
                if(right_lost_flag<40)
                {
                    if(right_count>10&&right_count<25)                 //区分小S弯
                    {
                        if((left_point[10]-left_point[15]<11)&&(left_point[15]-left_point[20]<11))      // 两点距离 ，证明右边是直线则是环岛
                        {
                            if(huandaoshibie>0)
                            {
                                if(left_point[20]<20&&left_point[25]<20)//防止出车库识别环岛
                                {
                                  righthuandao_flag=1;                   //进入左环岛标志位
                                  rightstate=1 ;                          //左环岛状态1
                                }
                            }
                        }
                    }
                }
     }



        //////////////////////////////////////三叉路口
          if(right_count>10&&left_count>10&&(mid_white<=45||mid_white1<=45||mid_white2<=45)&&shang_white<32)  //两边丢线且中间白点数区分十字
                {
                    sancha_flag = 1;
                    sanchastate = 1;

                }

  }
}
    else{
                shizi_flag=0;
            }
    podao_dispose();
    crossroads_dispose();
    roadabout_dispose();
    roadjunction_dispose();


}

/************************************************************

【函数名称】podao_dispose
【功    能】podao处理函数
【参    数】无
【返 回 值】无
【实    例】podao_dispose();
【注意事项】无

************************************************************/
void podao_dispose(void)
{
    if(podao_flag)
       {
            switch(podaostate)
                    {
                        case 1:
                            if(point_len[10]>17&&point_len[10]<24)//直道宽度19和59
                                podaostate=2;
                        break;


                        case 2:
                            {


                                podaostate=0;
                                }
                          break;
                                    }
            if(point_len[15]>40&&point_len[15]<50&&point_len[20]>40&&point_len[20]<70)

                 podao_flag=0;
                 podaostate=0;

                                   }
           }
/************************************************************

【函数名称】crossroads_dispose
【功    能】十字处理函数
【参    数】无
【返 回 值】无
【实    例】crossroads_dispose();
【注意事项】无

************************************************************/

extern int left_lost_flag ;
extern int right_lost_flag ;

void crossroads_dispose(void)
{
  int i,max=1,maxi,min=LCDW,mini;
 //如果左右丢线同时发生则视为十字条件达成
  if(shizi_flag)
  {
    if(left_lost_flag>40)//——————————————————————————————————————————————左下点
    {
      lx1 = 3;
      ly1 = 59;
    }
    else
    {
      for(i=45;i>10;i--)
      {
        if(left_point[i]>max&&i>left_lost_flag)
        {
          max=left_point[i];
          maxi=i;
        }
      }
      lx1 = max;
      ly1 = maxi;
    }
    if(right_lost_flag>56)//——————————————————————————————————————————————右下点
    {
      rx1 = LCDW-3;
      ry1 = 59;
    }
    else
    {
      for(i=45;i>10;i--)
      {
        if(right_point[i]<min&&i>right_lost_flag)
        {
          min=right_point[i];
          mini=i;
        }
      }
      rx1 = min;
      ry1 = mini;
    }
    for(i=4;i<LCDW/2-3;i++)//————————————————————————————————————————————————左上点
    {
      if(above_arr[i]>above_arr[i+2]&&above_arr[i+2]>above_arr[i+4]&&above_arr[i]<left_lost_flag&&above_arr[i]<ly1)
      {
        lx2 = i;
        ly2 = above_arr[i];
        connect_line(3,59,lx2,ly2);;//connect_line(lx1,ly1,lx2,ly2);
        //mend_flag=1;
        Horizontal_line();
        break;
      }
    }
    for(i=LCDW-3;i>LCDW/2-3;i--)//————————————————————————————————————————————————右上点
    {
      if(above_arr[i]>above_arr[i-2]&&above_arr[i-2]>above_arr[i-4]&&above_arr[i]<right_lost_flag&&above_arr[i]<ry1)
      {
        rx2 = i;
        ry2 = above_arr[i];
        connect_line(77,59,rx2,ry2);
        //mend_flag=1;
        Horizontal_line();
        break;
      }
    }
  }
//  if(right_lost_flag ==-1&&left_lost_flag==-1&&right_count<10&&left_count<10)
//       {
//         shizi_flag = 0;
//
//      }
}


/************************************************************

【函数名称】roadabout_dispose
【功    能】环岛元素处理函数
【参    数】无
【返 回 值】无
【实    例】roadabout_dispose();
【注意事项】无

************************************************************/




void roadabout_dispose(void)
{
      Left_huandao1 = cross_white[2];
      Left_huandao2 = cross_white[4];
      Left_huandao3 = cross_white[6];
      Right_huandao1 = cross_white[70];



        if(lefthuandao_flag==1)
        {

            switch(leftstate)
            {
                case 1:                            //第一次丢线，左下角拐点在40行左右开始补线
                    if(Left_huandao1<40)            //一直到左边圆环线到50行，进入第二阶段
                    {
                        leftstate = 2 ;
                    }
                break;

                case 2:

                        if(Left_huandao1>40)            //左下脚为圆环
                        {
                            leftstate = 4 ;
                            huandaoshibie=0;
                            }

                break;


                case 4:                                                         //环岛
                    if(Right_huandao1<15)
                    {
                        leftstate = 5 ;
                    }
                break;

                case 5:                                       //出环岛补线，右下拉到左上
                    if(Right_huandao1>29)
                    {
                        leftstate = 6;

                    }
                break;

                case 6:
                    if(right_lost_flag==-1)
                    {
                        leftstate = 0;
                        lefthuandao_flag = 0;

                    }

            }


            if(leftstate==1)     //找左前和左后拐点相连
                {
                    int max=-1,maxi,min=LCDW,mini;
                        if(left_lost_flag>56)//——————————————————————————————————————————————左下点
               {
                lx4 = 3;
                ly4 = 59;
               }
               else
               {
                 for( uint8 i=59;i>0;i--)
                  {
                    if(left_point[i]>max&&i>left_lost_flag)
                    {
                      max=left_point[i];
                      maxi=i;
                    }
                 }
                     lx4 = max;
                     ly4 = maxi;

               }


                for( uint8 i=1;i<LCDH-3;i++)//————————————————————————————————————————————————左上圆环点
            {
             if(left_point[i]<left_point[i+1]&&left_point[i+1]>left_point[i+2]&&left_point[i+1]>lx4)
              {
               ly5 = i+1;
               lx5 = left_point[i+1];
               break;
              }
            }

                         connect_line(lx4,ly4-20,lx5,ly5);
                         Horizontal_line();


            }

            //入口补线
        if(leftstate==4)
        {
           for(uint8 i=4;i<58;i++)//————————————————————————————————————————————————左上点
                {
                     if(above_arr[i]>above_arr[i+1]&&above_arr[i+1]>above_arr[i+2]&&above_arr[i]<left_lost_flag)//
                          {
                           lx7 = i;
                           ly7 = above_arr[i];
                           break;
                          }
                    }
                     connect_line(lx7+15,ly7,79,40);
                     Horizontal_line();
                                     //左上点连接到右下点
      }



        if(leftstate==6)
        {
            int min=LCDW-2,mini;

         if(right_lost_flag>56)//——————————————————————————————————————————————右下点
            {
              lx8 = LCDW-3;
              ly8 = 59;
            }
    else
            {
              for(uint8 i=55;i>25;i--)
              {
                if(right_point[i]<right_point[i+1]&&i>right_lost_flag)
                {
                  min=right_point[i];
                  mini=i;
                }
              }
              lx8 = min;
              ly8 = mini;
                        }

            connect_line(40,20,70,55);
            Horizontal_line();
        }
    }


     ////右环岛


            if(righthuandao_flag==1)
        {

            switch(rightstate)
            {
                case 1:                            //第一次丢线，右下角拐点在40行左右开始补线
                    if(Right_huandao1<41)            //一直到左边圆环线到50行，进入第二阶段
                    {
                        rightstate = 2 ;
                    }
                break;

                case 2:
                    if(Right_huandao1>47)            //右下脚为圆环
                    {
                        rightstate = 4 ;
                        huandaoshibie=0;
                    }
                break;


                case 4:                                                         //环岛
                    if(Left_huandao1<28)
                    {
                        rightstate = 5 ;
                    }
                break;

                case 5:                                       //出环岛补线，右下拉到左上
                    if(Left_huandao1>38)
                    {
                        rightstate = 6;
                    }
                break;

                case 6:
                    if(left_lost_flag==-1)          //出环岛
                    {

                        rightstate=7;
                    }
                    break;

                case 7:
                        {
                            if(right_lost_flag==-1)
                            {  rightstate=0;
                                righthuandao_flag=0;
                            }
                        }

            }


        if(rightstate==4)
        {

                 for(uint8 i=79;i>20;i--)//————————————————————————————————————————————————右上点
                    {
                     if(above_arr[i]>above_arr[i-1]&&above_arr[i-1]>above_arr[i-2]&&above_arr[i]<right_lost_flag)//
                      {
                       rx7 = i;
                       ry7 = above_arr[i];
                       break;
                      }
                        }
                     connect_line(rx7-15,ry7,1,40);//右上点连接到左下点
                     Horizontal_line();

      }



        if(rightstate==6)
        {
            int max=0,maxi=LCDW-1;

         if(left_lost_flag>56)//——————————————————————————————————————————————左下点
    {
      rx8 = 3;
      ry8 = 59;
    }
    else
    {
      for(uint8 i=59;i>0;i--)
      {
        if(left_point[i]>max&&i>left_lost_flag)
        {
          max=left_point[i];
          maxi=i;
        }
      }
      rx8 = max;
      ry8 = maxi;
            connect_line(30,15,1,40);
            Horizontal_line();

    }



        }


    }
}




///************************************************************
//
//【函数名称】roadjunction_dispose
//【功    能】三叉路口处理函数
//【参    数】无
//【返 回 值】无
//【实    例】roadjunction_dispose();
//【注意事项】无

//************************************************************/

void roadjunction_dispose(void)
{

    if(sancha_flag)
    {

        int max=1,maxi,min=LCDW,min_0=1,mini;
        switch(sanchastate)
        {
//            case 1:
//            if(shang_white<38)//mid_white<38
//            {
//             sanchastate = 2;
//            }
//            break;



            case 1:
            if(right_lost_flag==-1||left_lost_flag==-1)
            {
                sanchastate = 4;
            }
            break;

            case 4:
            if(mid_white>48)
          {
             sanchastate = 0;
             sancha_flag = 0;
             sancha_count++;
          }
            break;

      }

        if(sanchastate==1)  //  识别到三叉路口，右下角连到右上拐点
        {
            if(sancha_count<2)
            {
                            if(right_lost_flag>55)//——————————————————————————————————————————————右下点
                             {
                                  lx11 = LCDW-3;
                                  ly11 = 59;
                                                  }
                     else
                            {
                              for(uint8 i=59;i>35;i--)
                                  {
                                      if(right_point[i]<min&&i>right_lost_flag)
                                          {
                                              min=right_point[i];
                                              mini=i;
                                                          }
                                                              }
                              lx11 = min;
                              ly11 = mini;
                                                                     }

                            for(uint8 i=40;i<75;i++)//————————————————————————————————————————————————上点
                                  {
                                      if(above_arr[i-4]<above_arr[i+1]&&above_arr[i+1]>above_arr[i+5]&&above_arr[i+1]<ly11)
                                          {
                                              lx12 = i+1;
                                              ly12 = above_arr[i+1];
                                               break;
                                                                                      }
                                                                                          }
                                                                                                    }
            if(sancha_count>=2)
            {                     if(left_lost_flag>55)//——————————————————————————————————————————————右下点
                                             {
                                                  lx11 = 3;
                                                  ly11 = 59;
                                                                  }
                                     else
                                            {
                                              for(uint8 i=59;i>35;i--)
                                                  {
                                                      if(left_point[i]>min_0&&i>left_lost_flag)
                                                          {
                                                              min_0=left_point[i];
                                                              mini=i;
                                                                          }
                                                                              }
                                              lx11 = min_0;
                                              ly11 = mini;
                                                                                     }

                                            for(uint8 i=20;i<65;i++)//————————————————————————————————————————————————上点
                                                  {
                                                      if(above_arr[i-4]<above_arr[i+1]&&above_arr[i+1]>above_arr[i+6]&&above_arr[i+1]<ly11)
                                                          {
                                                              lx12 = i+1;
                                                              ly12 = above_arr[i+1];
                                                               break;
                                                                                                      }
                                                                                                          }
                                                                                                               }
                if(sancha_count==4)sancha_count=0;
                connect_line(lx11,ly11,lx12,ly12);
                Horizontal_line();

                                                                                               }

                                                                                                                                                 }
                                                                                                                                                }


