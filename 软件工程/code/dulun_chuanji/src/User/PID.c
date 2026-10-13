#include "PID.h"

//Target 期望值
//Feedback 反馈值



void Pid_Position(float Target,float Feedback,PID* Pid)
{

         //更新误差
    Pid->Err=Target-Feedback;   //计算误差

    Pid->P_out=Pid->Kp*Pid->Err;
    Pid->D_out=Pid->Kd*(Pid->Err-Pid->Err_last);
    Pid->Err_Int+=Pid->Err;
    if(Pid->Err_Int > +Pid->Integral_lim) Pid->Err_Int=+Pid->Integral_lim; //积分限幅
    if(Pid->Err_Int < -Pid->Integral_lim)  Pid->Err_Int=-Pid->Integral_lim;

    Pid->I_out=Pid->Ki*Pid->Err_Int;
    Pid->Out=Pid->P_out+Pid->D_out+Pid->I_out;
    Pid->Err_last=Pid->Err;

    
    if(Pid->Out > +Pid->Out_lim) Pid->Out=+Pid->Out_lim; //输出限幅
    if(Pid->Out < -Pid->Out_lim) Pid->Out=-Pid->Out_lim;


}

void Pid_Incre(float Target,float Feedback,PID* Pid)   //增量式PID
{
    Pid->Err_l2st=Pid->Err_last;
    Pid->Err_last=Pid->Err;
    Pid->Err=Target-Feedback;
    float Pout=Pid->Kp*(Pid->Err-Pid->Err_last);
    float Iout=Pid->Ki*Pid->Err;
    float Dout=Pid->Kd*(Pid->Err - 2*Pid->Err_last + Pid->Err_l2st);

    Pid->Out=Pout+Iout+Dout;
}

