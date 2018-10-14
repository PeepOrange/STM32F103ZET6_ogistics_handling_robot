#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header



/*****PID��������******/
#define K_PWM_Encoder                   100.0           //����Ч�ʱ�    Encoder/PWM = K_PWM_Encoder (5msʱ)
#define Encoder_read_num_Proportion		30.0			//��������(Kp)
#define Encoder_read_num_Integral		10.0			//���ֳ���(Ki)	Ki=(Kp*T)/Ti
#define Encoder_read_num_Derivative		0.1			    //΢�ֳ���(Kd)	Kd=(Kp*Td)/T
//���µ�it�ļ��޸�
extern uint16_t LeftUp_PWM;                              //Ĭ�ϵ���ǰ�ֵ�PWM,[-1000,1000]
extern uint16_t LeftBack_PWM;                            //Ĭ�ϵ�����ֵ�PWM,[-1000,1000]
extern uint16_t RightUp_PWM;                             //Ĭ�ϵ���ǰ�ֵ�PWM,[-1000,1000]
extern uint16_t RightBack_PWM;                           //Ĭ�ϵ��Һ��ֵ�PWM,[-1000,1000]
/*****PID��������*****/


#define abs(x)     x>0?x:-x 
#define jud(x)     x>0?1:0

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

    


//BSP��ʼ������
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;        //��ʼ������PWM��IO�ڼ���Timer
extern void system_init(void) ;
void OLED_Init(void);       //��ʼ��OLED������ʾ������Ϣ ������ 90 ������ 2.4.6�ֱ���ʾ����˳������
void  Encoder_Inti(void);       //��ʼ����������IO�ں�Timer
void TIM6_Inti(void);					//������ʱ��6��ÿ�μ���1us����5000�������£���5ms������ʱ�����жϣ��ж����ȼ�Ϊ1��1



#ifdef __cplusplus
}
#endif 








#endif
