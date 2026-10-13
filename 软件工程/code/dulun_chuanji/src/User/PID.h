

#ifndef SRC_USER_PID_H_
#define SRC_USER_PID_H_

typedef struct
{
  float Kp;
  float Ki;
  float Kd;
  float Err;
  float Err_last;
  float Err_l2st;  //前前次误差
  float Integral_lim; //积分限幅
  float Err_Int;  //误差积分
  float P_out;
  float I_out;
  float D_out;
  float Out;
  float Out_lim;
} PID;
void Pid_Position(float Target,float Feedback,PID* Pid);
void Pid_Incre(float Target,float Feedback,PID* Pid);

#endif
