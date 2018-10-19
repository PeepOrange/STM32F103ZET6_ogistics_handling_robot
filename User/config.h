#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header



/*****PID��������******/
#define K_PWM_Encoder                   0.02           //����Ч�ʱ�    Encoder/PWM = K_PWM_Encoder (Reset_Timeʱ)
#define Encoder_read_num_Proportion		20.0			//��������(Kp)
#define Encoder_read_num_Integral		10.0			//���ֳ���(Ki)	Ki=(Kp*T)/Ti
#define Encoder_read_num_Derivative		0.1			    //΢�ֳ���(Kd)	Kd=(Kp*Td)/T
#define Goal_LeftUp_PWM                 200             //��ǰ��Ŀ��PWM(-1000,1000)
#define Goal_LeftBack_PWM               200             //�����Ŀ��PWM(-1000,1000)
#define Goal_RightUp_PWM                200             //��ǰ��Ŀ��PWM(-1000,1000)
#define Goal_RightBack_PWM              200             //�Һ���Ŀ��PWM(-1000,1000)
#define Reset_Time                      10000           //ˢ��PID����ʱ�䣬��λus [0,65535]
/*****PID��������*****/






#ifdef __cplusplus
extern "C"
{
#endif 

//��c��ʽ����Ĵ���
#include "PID.h"
extern PID  LeftUp_PID_Mortor;
extern PID  LeftBack_PID_Mortor;
extern PID  RightUp_PID_Mortor;
extern PID  RightBack_PID_Mortor;
extern int16_t LeftUp_PWM;                              
extern int16_t LeftBack_PWM;                            
extern int16_t RightUp_PWM;                             
extern int16_t RightBack_PWM;                           
#ifdef __cplusplus
}
#endif 






#ifdef __cplusplus
extern "C"
{
#endif 
    
//�û�����    
void LED1_Toggle(void);     //LED1��ת    
void LED2_Toggle(void);     //LED2��ת
void LED_ALL_OFF(void);     //����LED�ر�    
void PID_PWM_Adujust(int16_t PWM1,int16_t PWM2,int16_t PWM3,int16_t PWM4);       //��ǰ  ���   ��ǰ  �Һ�      PID����PWM
void Oled_PID(float UpLeft,float UpRight,float BackLeft,float BackRight);
    
    

//BSP��ʼ������
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;        //��ʼ������PWM��IO�ڼ���Timer
extern void system_init(void) ;
void OLED_Init(void);       //��ʼ��OLED������ʾ������Ϣ 
void  Encoder_Inti(void);       //��ʼ����������IO�ں�Timer
void TIM6_Inti(void);					//������ʱ��6



#ifdef __cplusplus
}
#endif 








#endif
