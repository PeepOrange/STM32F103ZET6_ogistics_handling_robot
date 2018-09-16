#include "PID.h"


typedef struct 
{
	float goal_point;
	float	last_Error;
	float	pre_Error;
	
	float  Kp;				//控制器的比例放大系数
	float  Ki;				//控制器的积分系数		Ki=(Kp*T)/Ti 	Ti为控制器的积分时间
	float  Kd;				//控制器的微分系数		Kd=(Kp*Td)/T 	Td为控制器的微分时间
}PID;

static PID speed_pid={0,0,0,Proportion,Integral,Derivative};


float	PID_Calc(PID *pid,float read_point)		//增量式PID调节
{
	float this_Error;
	float FeedBack;
	
	this_Error=pid->goal_point-read_point;
	
	FeedBack=(
	(pid->Kp+pid->Ki+pid->Kd)*this_Error
 -(pid->Kp+2*pid->Kd)*pid->last_Error
 +(pid->Kd)*pid->pre_Error);
	
	pid->pre_Error=pid->last_Error;
	pid->last_Error=this_Error;
	
	return FeedBack;
}


float PID_Out(float goal_point,float read_point)	//返回应当调节为的值
{
	speed_pid.goal_point=goal_point;
	
	return(read_point+PID_Calc(&speed_pid,read_point));
	
}


