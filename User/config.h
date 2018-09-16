#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header




#ifdef __cplusplus
extern "C"
{
#endif 

//��c��ʽ����Ĵ���

#ifdef __cplusplus
}
#endif 



#ifdef __cplusplus
extern "C"
{
#endif 

//�û��ⲿ����    

#define Move_Speed 200        //�ƶ�ʱ��PWM(0-1000)    
    
    
void LED1_Toggle(void);     //LED1��ת    
void LED2_Toggle(void);     //LED2��ת
void LED_ALL_OFF(void);     //����LED�ر�    
void Move_Stop(void);            //ֹͣ
void Move_Right(void);           //��ƽ��
void Move_Left(void);            //��ƽ��
void Move_Back(void);            //����
void Move_Up(void);              //ǰ��

















//BSP��ʼ������
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;
extern void system_init(void) ;
void OLED_Init(void);       //��ʼ��OLED������ʾ������Ϣ 
    


#ifdef __cplusplus
}
#endif 








#endif
