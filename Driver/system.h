#ifndef __system_H
#define __system_H 			   
#include "stm32f10x.h" 


//Sysclkϵͳʱ��72M
//HCLK(AHB)72M  HCLK1(APB1)36M  HCLK2(APB2)72M
//cpu��Ƶ��     ��������I2C1/2   �������� GPIO
//ϵͳ�δ�ʱ�����ȼ�15
//�ж��������Ϊ	NVIC_PriorityGroup_2	��ռ���ȼ�Ϊ[0,3]	�����ȼ�Ϊ[0,3]

void system_init(void);//ϵͳ��ʼ��
void SysRestart(void); //ϵͳ���� 
void delay_us(__IO uint32_t us);   //���ö�ʱ����ʱ1us  
void delay_ms(__IO uint32_t ms);   //���ö�ʱ����ʱ1ms



#endif
