#include "PID.h"






float	PID_Out(PID *pid)		//增量式PID调节
{
	float this_Error;
	float FeedBack;
	
	this_Error=pid->goal_point- pid->read_point;
	
	FeedBack=(
	(pid->Kp+pid->Ki+pid->Kd)*this_Error
 -(pid->Kp+2*pid->Kd)*pid->last_Error
 +(pid->Kd)*pid->pre_Error);
       
	pid->pre_Error=pid->last_Error;
	pid->last_Error=this_Error;    
    
	return (FeedBack+pid->read_point);
}




