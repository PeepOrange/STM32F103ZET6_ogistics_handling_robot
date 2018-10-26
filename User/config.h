#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header



/*****PID��������******/
#define K_PWM_Encoder                   0.01            //����Ч�ʱ�    Encoder/PWM = K_PWM_Encoder (Reset_Timeʱ)
#define Encoder_read_num_Proportion	    25		        //��������(Kp)
#define Encoder_read_num_Integral		15	            //���ֳ���(Ki)	Ki=(Kp*T)/Ti
#define Encoder_read_num_Derivative		1		        //΢�ֳ���(Kd)	Kd=(Kp*Td)/T
#define Goal_PWM                        300             //Ŀ��PWM(-1000,1000)
#define Correct_Back_PWM                -300            //����ת��ĸ���һ���PWM
#define Correct_Up_PWM                  300             //����ת�������һ���PWM
#define Reset_Time                      10000           //ˢ��PID����ʱ�䣬��λus [0,65535]
#define Test_Dir                        UP            //���Է���
#define Test_down_Steering_angle        15            //�¶���Ƕ�[10,20]
#define Test_up_Steering_angle          15            //�϶���Ƕ�[10,20]
#define Test_mid_Steering_angle         15            //�ж���Ƕ�[10,20]
#define Test_hand_Steering_angle        15            //צ����Ƕ�[10,20]


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
extern char Pos_X;
extern char Pos_Y;    
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
void Senor_Init(void);    
void Run_Up(void);  
void Run_Left(void);
void Run_Right(void);
void Run_Back(void);    
void Run_Stop(void);
void Up_Position(void);
void Left_Position(void);
void Right_Position(void);
void Back_Position(void);

    
    
//BSP��ʼ������
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;                 //��ʼ������PWM��IO�ڼ���Timer
extern void system_init(void) ;
void OLED_Init(void);                 //��ʼ��OLED������ʾ������Ϣ 
void  Encoder_Inti(void);             //��ʼ����������IO�ں�Timer
void TIM6_Inti(void);					//������ʱ��6 
void Steering_engine_Init(void);       //������ʱ��7 0.1msһˢ



#ifdef __cplusplus
}
#endif 








#endif
