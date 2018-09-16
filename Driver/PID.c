#include "PID.h"


typedef struct 
{
	float goal_point;
	float	last_Error;
	float	pre_Error;
	
	float  Kp;				//�������ı����Ŵ�ϵ��
	float  Ki;				//�������Ļ���ϵ��		Ki=(Kp*T)/Ti 	TiΪ�������Ļ���ʱ��
	float  Kd;				//��������΢��ϵ��		Kd=(Kp*Td)/T 	TdΪ��������΢��ʱ��
}PID;

static PID speed_pid={0,0,0,Proportion,Integral,Derivative};


float	PID_Calc(PID *pid,float read_point)		//����ʽPID����
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


float PID_Out(float goal_point,float read_point)	//����Ӧ������Ϊ��ֵ
{
	speed_pid.goal_point=goal_point;
	
	return(read_point+PID_Calc(&speed_pid,read_point));
	
}


