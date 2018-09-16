#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"                  // Device header
#include "stdint.h"
#include "gpio.h"

//�жϺ�����stm32f10x_it.h�б�д


/*

inti_mode()����˵��

EXTI_Line       
	
	�ж�/�¼���  /  ����Դ
	EXTI_Line0 					P0  PΪ(A,B,C,D,E,F,G,H,I)  A0 B0 ...
	EXTI_Line1 					P1  PΪ(A,B,C,D,E,F,G,H,I)  A1 B1 ...
	EXTI_Line2 					P2  PΪ(A,B,C,D,E,F,G,H,I)  A2 B2 ...
	EXTI_Line3			
	EXTI_Line4			
	EXTI_Line5 			
	EXTI_Line6 			
	EXTI_Line7 			    '''''
	EXTI_Line8 			
	EXTI_Line9			
	EXTI_Line10			
	EXTI_Line11			
	EXTI_Line12 			
	EXTI_Line13			
	EXTI_Line14 			
	EXTI_Line15 					P15 PΪ(A,B,C,D,E,F,G,H,I)  A15 B15 ...
	EXTI_Line16 					PVD���
	EXTI_Line17					RTC�����¼�
	EXTI_Line18 				  USB�����¼�
	EXTI_Line19 					��̫�������¼�

EXTIMode_TypeDef   
	
	�ж�ģʽ
	EXTI_Mode_Interrupt  �����ж�
	EXTI_Mode_Event    	 �����¼�

	�¼����ǿ����巢��������һ������,������Ӳ���Զ��������¼������Ľ��,��Ȼ��Ӧ������������Ҫ�����ú�,��������DMA����,ADת����;
	�жϣ��ǿ����ʵ��ֱ�ӽ����жϳ���
	�򵥾������ⲿI/O����ADת��,�������ⲿ��Ʒ������;���ʹ�ô�ͳ���ж�ͨ��,��ҪI/O���������ⲿ�ж�,�ⲿ�жϷ����������ADת��,ADת������жϷ�������ύ�����;
						Ҫ��ʹ���¼�ͨ��,I/O���������¼�,Ȼ����������ADת��,ADת������жϷ�������ύ�����;���֮��,���߲�Ҫ�������AD����,������Ӧ�ٶ�Ҳ����;
						Ҫ��ʹ���¼�����DMA����,����ȫ�����������Ϳ������ĳЩ���������ˡ�

EXTITrigger_TypeDef   
  
	�жϴ�������
  EXTI_Trigger_Rising    �����ش������͵�ƽ��Ϊ�ߵ�ƽ��
  EXTI_Trigger_Falling   �½��ش���
  EXTI_Trigger_Rising_Falling   �������߶�����

EXTI_LineCmd   
	
	ʹ��
	DISABLE  �ر�
	ENABLE   ����
*/




/*
inti_turn()����˵��


NVIC_IRQChannel
	
	�ж�Դ
	EXTI0_IRQn  		EXTI0���ж�
	EXTI1_IRQn			EXTI1���ж�
	EXTI2_IRQn			EXTI2���ж�
	EXTI3_IRQn			EXTI3���ж�
	EXTI4_IRQn			EXTI4���ж�
	EXTI9_5_IRQn		[EXTI5,EXTI9]���ж�
	EXTI15_10_IRQn	[EXTI10,EXTI5]���ж�

NVIC_IRQChannelPreemptionPriority

	��ռ���ȼ�	
	��Ϊleon_init();��������NVIC_PriorityGroup_2���飬����������Ϊ[0,3]

NVIC_IRQChannelSubPriority

	�����ȼ�
	ͬ�ϣ�Ҳ��[0,3]

	���ȼ����жϣ����ȼ�ԽС���ȼ�Խ�ߣ��ȱ���ռ���ȼ�������ͬ���ٱ������ȼ�����Ҳ��ͬ������Ӳ���ž�����ϵͳ�δ�ʱ�����ȼ�15��Ϊ11��11������ռ���ȼ�Ϊ3�������ȼ�ҲΪ3

NVIC_IRQChannelCmd

	ʹ��
	DISABLE  �ر�
	ENABLE   ����

*/


class EXTI_GPIO
{
	public:
	void inti_pin(GPIO *exti_gpio);
	//   �ж����ų�ʼ�����ⲿ�ж�����   ��ʼ������������������
	void inti_souce(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);  //GPIO_PortSourceGPIO[A-G]  GPIO_PinSource[0-15]   ���� inti_souce(GPIO_PortSourceGPIOE,GPIO_PinSource1)
	//   �ж�����Դ��ʼ�����ⲿ�ж�����
	void inti_mode(uint32_t EXTI_Line , EXTIMode_TypeDef EXTI_Mode , EXTITrigger_TypeDef EXTI_Trigger , FunctionalState EXTI_LineCmd);  
	//  �жϳ�ʼ��   �ж�/�¼���	          EXTIģʽ	                            ��������                        EXTIʹ��
	void inti_turn(uint8_t NVIC_IRQChannel , uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd);  
	//�ж�˳���ʼ��        �����ж�Դ											������ռ���ȼ�													���������ȼ�									�ж�ͨ��ʹ��
	
	
};



#endif
