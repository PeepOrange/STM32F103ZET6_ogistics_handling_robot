#ifndef __uart_H
#define __uart_H
#include "stm32f10x.h"                  // Device header
#include "gpio.h"

#define USART_Mode_ALL   USART_Mode_Rx|USART_Mode_Tx

/*
inti_mode  //�첽ͨ��ֻҪ�˺�����ʼ������  ��ͬ��ͨ��Ҫ�ڵ���	USART_ClockInitTypeDef �ṹ���ʼ��
	����ͨѶģʽ��ʼ��
	USART_BaudRate ������
	����ֵ�� 2400��9600��19200��115200
	
	USART_WordLength	�ֳ�
	����żУ��λ USART_WordLength_8b	8bit
	����żУ��λ USART_WordLength_9b	9bit
	
	USART_StopBits	ֹͣλ
	USART_StopBits_1    1λ		����
	USART_StopBits_0_5  0.5λ 
	USART_StopBits_2		2λ	
	USART_StopBits_1_5	1.5λ
	
	USART_Parity		У��λ
	USART_Parity_No			��У��
	USART_Parity_Even		żУ��
	USART_Parity_Odd		��У��
	
	USART_Mode		USARTģʽ	
	USART_Mode_Rx		����ģʽ
	USART_Mode_Tx		����ģʽ
	USART_Mode_ALL	����/����ģʽ
	
	USART_HardwareFlowControl						Ӳ��������
	USART_HardwareFlowControl_None			��ʹ��Ӳ����
	USART_HardwareFlowControl_RTS				ʹ��RTS
	USART_HardwareFlowControl_CTS				ʹ��CTS
	USART_HardwareFlowControl_RTS_CTS		RTS��CTS��ʹ��
	
*/




/*
inti_it_turn()����˵��


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
	
	
USART_IT	
	�жϴ�����ʽ
	 USART_IT_PE      ��ż�����ж�          
	 USART_IT_TXE     �����ж�      
	 USART_IT_TC      ��������ж�          
	 USART_IT_RXNE    �����ж�       
	 USART_IT_IDLE    ���������ж�          
	 USART_IT_LBD     LIN�ж�̽���ж�      
	 USART_IT_CTS     CTS�ж�                 
	 USART_IT_ORE     ��������ж�                     
	 USART_IT_NE      ���������ж�                  
	 USART_IT_FE      ֡�����ж�        

*/

class UART
{
	public:

	UART(GPIO *RX,GPIO *TX);   //����USART1ʱ�ӣ�������RXΪ�������븡��  TXΪ�����������  TX��RX��uart��tx��rx����  TX1:PA9  RX1:PA10
	void inti_mode(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
	//  ����ͨѶģʽ��ʼ��     ������										�ֳ�										ֹͣλ										У��λ								USARTģʽ									Ӳ��������
	static void sent_byte(uint8_t ch);   //USART1����һ���ֽ�
	static void sent_str(const char *str);  //USART1����һ���ַ��� 
	static uint16_t receive_data();  //USART1�յ�������
	void inti_it_turn( uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd,uint16_t USART_IT);  
	//    �����жϿ���    				������ռ���ȼ�													���������ȼ�											�ж�ͨ��ʹ��									�жϴ�����ʽ
	
	private:
	GPIO *RX;
	GPIO *TX;
		
	
};




#endif
