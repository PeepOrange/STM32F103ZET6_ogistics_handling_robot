#ifndef __tim_H
#define __tim_H
#include "stm32f10x.h"                  // Device header

#define	TIM_channel1	1
#define	TIM_channel2	2
#define	TIM_channel3	3
#define	TIM_channel4	4


/*	��ʱ�����Դ������¼���	��˵ͨ�ö�ʱ�����Լ��ݻ�����ʱ������
�߼���ʱ��		TIM1	TIM8							16λ		����/����		4���Ƚ�/����ͨ��		�л������
ͨ�ö�ʱ��		TIM2	TIM3	TIM4	TIM5	16λ		����/����		4���Ƚ�/����ͨ��		�޻������
������ʱ��		TIM6	TIM7							16λ		����				 �ޱȽ�/����ͨ��		�޻������
*/

/*
			�߼���ʱ�� 							ͨ�ö�ʱ��
			TIM1 						TIM8 		TIM2 				TIM5 		TIM3 					TIM4
CH1 	PA8/PE9 				PC6 		PA0/PA15 		PA0 		PA6/PC6/PB4 	PB6/PD12
CH1N 	PB13/PA7/PE8 		PA7
CH2 	PA9/PE11 				PC7 		PA1/PB3 		PA1 		PA7/PC7/PB5 	PB7/PD13
CH2N 	PB14/PB0/PE10 	PB0
CH3 	PA10/PE13 			PC8 		PA2/PB10 		PA2 		PB0/PC8 			PB8/PD14
CH3N 	PB15/PB1/PE12 	PB1
CH4 	PA11/PE14 			PC9 		PA3/PB11 		PA3 		PB1/PC9 			PB9/PD15
ETR 	PA12/PE7 				PA0 		PA0/PA15 						PD2 					PE0
BKIN 	PB12/PA6/PE15 	PA6

*/

/*����ʱ����������	�߼���ʱ�����ݵͼ���ʱ���Ĺ���
������ʱ��		��������һ��ʱ�䣬�ȵ����ʱ��ͷ����ж�
ͨ�ö�ʱ��		��������PWM�����ڡ�Ƶ�ʿɿأ������Բ���PWM������	
�߼���ʱ��		�������ɻ�����������Բ������������Բ�����·��ɲ����
*/

/*
base()����˵��

	TIMx		
	������ʱ��֧�����ö�ʱ��
	
	RCC_APB1Periph
	��ʱ��ʱ������		RCC_APB1Periph_TIMx		�磺	RCC_APB1Periph_TIM6	/	RCC_APB1Periph_TIM7
	
	NewState
	ʹ��							DISABLE		/		ENABLE

	TIM_Prescaler
	Ԥ��Ƶ��		[1-65536]	
	ʱ��Դ����Ԥ��Ƶ�����Ƕ�ʱ��ʱ��		����ʱ��ԴΪ72Mhz����������Ϊ72��Ϊ1us����һ��

	TIM_Period
	ʱ��				[1-65536]
	�趨��ʱ����ʱ��������ʱ���ǵ�������ֲ����ж�
	
	NVIC_IRQChannel	
	 TIM1_BRK_IRQn 			TIM1�Ͽ��ж�
	 TIM1_UP_IRQn    		TIM1�����ж�
	 TIM1_TRG_COM_IRQn  TIM1������ͨ���ж� 
	 TIM1_CC_IRQn 			TIM1����Ƚ��ж�
	 TIM2_IRQn   				TIM2ȫ���ж�
	 TIM3_IRQn   				TIM3ȫ���ж�
	 TIM4_IRQn   				TIM4ȫ���ж�
	
	NVIC_IRQChannelPreemptionPriority
	��ռ���ȼ�						��Ϊsystem_inti()����Ϊ2;2 ����	��ֵΪ[0,3]
	
	NVIC_IRQChannelSubPriority
	�����ȼ�							ͬ�ϣ���������Ϊ[0,3],���ȼ����忴EXTI.h�ļ���ע��
	
*/



class TIM_Base
{
	public:
			void base																						//������ʱ������ ������stm32f10x_it.cpp�б�д�жϺ����������ʱ�����ж�
			(
			TIM_TypeDef* TIMx,																	//��ʱ����� 
			uint32_t RCC_APB1Periph,														//��ʱ��ʱ������
			FunctionalState NewState,														//ʹ�ܶ�ʱ��
			uint16_t TIM_Prescaler,															//��ʱ��Ԥ��Ƶ
			uint16_t TIM_Period	,																//��ʱ��������
			uint8_t NVIC_IRQChannel,														//�ж�Դ
			uint8_t NVIC_IRQChannelPreemptionPriority,					//��ռ���ȼ�
			uint8_t NVIC_IRQChannelSubPriority									//�����ȼ�
			);		
			//	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE)	��ʼ����ɺ���ô˺����򿪶�ʱ�����ö�ʱ������֮��Ҳ�ɹرգ�ʹ��ʱ����ͣ
			//	STM32F10X_MD	��������TIM6��TIM7���ж�
			//	STM32F10X_MD	ֻ��TIM1��TIM2��TIM3��TIM4
	
};


/*
	base()����˵��
	TIMx												��ʱ�����
	ͨ�ü�ʱ������TIM6��TIM7������ʱ�������඼֧��
	
	RCC_APB1Periph							��ʱ��ʱ��
	RCC_APB1Periph_TIMx		�磺	RCC_APB1Periph_TIM6	/	RCC_APB1Periph_TIM7
	
	TIM_Prescaler								��ʱ��Ԥ��Ƶ			
	Ԥ��Ƶ��		[1-65536]	
	ʱ��Դ����Ԥ��Ƶ�����Ƕ�ʱ��ʱ��		���	TIM_ClockDivisionΪTIM_CKD_DIV1	����ʱ��ԴΪ72Mhz����������Ϊ72��Ϊ1us����һ��	

	TIM_CounterMode							��ʱ��������ʽ
	TIM_CounterMode_Up									���ϼ���
	TIM_CounterMode_Down								���¼���
	TIM_CounterMode_CenterAligned1			�������ģʽ1����������������Ϻ����¼���������Ƚ��жϱ�־λ��ֻ�ڼ��������¼���ʱ������
	TIM_CounterMode_CenterAligned2			�������ģʽ2: ��������������Ϻ����¼���������Ƚ��жϱ�־λ��ֻ�ڼ��������ϼ���ʱ������
	TIM_CounterMode_CenterAligned3			�������ģʽ3: ��������������Ϻ����¼���������Ƚ��жϱ�־λ��ֻ�ڼ��������º����ϼ���ʱ��������
	
	TIM_Period									��ʱ������
	ʱ��				[1-65536]
	�趨��ʱ��������	

	TIM_ClockDivision						ʱ�ӷ�Ƶ
	TIM_CKD_DIV1								1��Ƶ
	TIM_CKD_DIV2								2��Ƶ
	TIM_CKD_DIV4								4��Ƶ
	���ö�ʱ��ʱ�� CK_INT Ƶ���������������Լ������˲�������ʱ��Ƶ�ʷ�Ƶ�ȡ�����ѡ�� 1��2��4��Ƶ��
	������ͨ��PWM����Ϊ	TIM_CKD_DIV1	����
*/


/*
out_pwm_inti()
		TIM_TypeDef* TIMx																			//��ʱ�����
		ͨ�ü�ʱ������TIM6��TIM7������ʱ�������඼֧��		
		
		uint8_t  TIM_Channel																	//��ʱ�����ͨ��
		TIM_channel1			ͨ��1
		TIM_channel2			ͨ��2
		TIM_channel3			ͨ��3
		TIM_channel4			ͨ��4
		
		uint16_t TIM_OCMode								//�Ƚ����ģʽ
		TIM_OCMode_Timing									����Ƚ�ʱ��ģʽ (������Ŷ�����Ч)
		TIM_OCMode_Active									����Ƚ�����ģʽ   (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�ߵ�ƽ)
		TIM_OCMode_Inactive								����ȽϷ�����ģʽ (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�͵�ƽ)
		TIM_OCMode_Toggle									����Ƚϴ���ģʽ    (��ת��������ֵ��Ƚ�/����Ĵ���ֵ��ͬʱ����ת������ŵĵ�ƽ)
		TIM_OCMode_PWM1										���ϼ���ʱ����TIMx_CNT���Ѿ��ǵ����� < TIMx_CCR��TIM_Pulse���õ�ֵ��ʱ�������ƽ��Ч������Ϊ��Ч���¼���ʱ����TIMx_CNT > TIMx_CCRʱ�������ƽ��Ч������Ϊ��Ч
		TIM_OCMode_PWM2										�������෴
		
		uint16_t TIM_OutputState															//�Ƚ����ʹ��
		TIM_OutputState_Disable			ȡ�����������
		TIM_OutputState_Enable			ʹ����������ţ�������
		
		uint32_t TIM_Pulse																		//������
		PWM�������ȣ���TIM_Period��ͬ������Եó�Ƶ�ʺ�ռ�ձ�
		[0,65535]     ռ�ձ�ΪTIM_Pulse/TIM_Period , ���TIM_Pulse���ܳ���TIM_Period���������ܳ���100%
		
		uint16_t TIM_OCPolarity																//�������
		�������Ŷ�ʱ��ͨ����Ч��ƽ���Ǹߵ�ƽ��Ч���ǵ͵�ƽ��Ч
		TIM_OCPolarity_High		�ߵ�ƽ��Ч
		TIM_OCPolarity_Low		�͵�ƽ��Ч
		
		uint16_t TIM_OCIdleState															//����״̬�±Ƚ����״̬
		TIM_OCIdleState_Set				����ʱ��λ
		TIM_OCIdleState_Reset			����ʱ��λ
*/


/*
in_pulse_inti()����˵��

	TIMx					��ʱ�����	��TIM1

	TIM_Channel		����ͨ��
	TIM_Channel_1		ͨ��1
	TIM_Channel_2		ͨ��2
	TIM_Channel_3		ͨ��3
	TIM_Channel_4		ͨ��4
		
	TIM_ICPolarity						���񴥷�
	TIM_ICPolarity_Rising			�����ش���
	TIM_ICPolarity_Falling		�½��ش���
	TIM_ICPolarity_BothEdge		˫���ش���
	
	TIM_ICSelection							���벶��ѡ��
	TIM_ICSelection_DirectTI		ֱ��ӳ��ͬͨ��	ICxӳ�䵽TIx
	TIM_ICSelection_IndirectTI	���ӳ�䵽����һ��ͨ��	ICxӳ�䵽TIy		1��2��3��4		
	TIM_ICSelection_TRC					ӳ�䵽ͬͨ����TRC��
	
	TIM_ICPrescaler
	TIM_ICPSC_DIV1				һ��������һ����Ч����
	TIM_ICPSC_DIV2				����������һ����Ч����
	TIM_ICPSC_DIV4				�ĸ�������һ����Ч����
	TIM_ICPSC_DIV8				�˸�������һ����Ч����
	
	TIM_ICFilter
	[0,15]		���벶���˲���,��ֹ�Ӳ�,����Ϊ0
	
	TIM_FLAG						�жϱ�־λ
	TIM_FLAG_Update			�����¼���־
	TIM_FLAG_CC1				IC1�����¼�������־
	TIM_FLAG_CC2				IC2��׽�¼�������־
	TIM_FLAG_CC3				IC3��׽�¼�������־
	TIM_FLAG_CC4				IC4��׽�¼�������־
	TIM_FLAG_COM				COM�¼�(������/�ȽϿ���λ��CCxE��CCxNE��OCxM�ѱ�����)������־
	TIM_FLAG_Trigger		�����¼�(����ģʽ���������ڳ��ſ�ģʽ�������ģʽʱ����TRGI����˼�⵽��Ч���أ����ſ�ģʽ�µ���һ����)������־
	TIM_FLAG_Break			ɲ���¼���־
	TIM_FLAG_CC1OF			����/�Ƚ�1�ظ�����������
	TIM_FLAG_CC2OF			����/�Ƚ�2�ظ�����������
	TIM_FLAG_CC3OF			����/�Ƚ�3�ظ�����������
	TIM_FLAG_CC4OF			����/�Ƚ�4�ظ�����������
	
	 NVIC_IRQChannel		�ж�Դ
	 TIM1_BRK_IRQn 			TIM1�Ͽ��ж�
	 TIM1_UP_IRQn    		TIM1�����ж�
	 TIM1_TRG_COM_IRQn  TIM1������ͨ���ж� 
	 TIM1_CC_IRQn 			TIM1����Ƚ��ж�
	 TIM2_IRQn   				TIM2ȫ���ж�
	 TIM3_IRQn   				TIM3ȫ���ж�
	 TIM4_IRQn   				TIM4ȫ���ж�
	
	 NVIC_IRQChannelPreemptionPriority
	 ��ռ���ȼ�						��Ϊsystem_inti()����Ϊ2;2 ����	��ֵΪ[0,3]
	
	 NVIC_IRQChannelSubPriority
	 �����ȼ�							ͬ�ϣ���������Ϊ[0,3],���ȼ����忴EXTI.h�ļ���ע��	 
*/

/*in_pwm_inti()����˵��

	TIMx					��ʱ�����	��TIM1

	TIM_Channel		����ͨ��
	TIM_Channel_1		ͨ��1
	TIM_Channel_2		ͨ��2
	TIM_Channel_3		ͨ��3
	TIM_Channel_4		ͨ��4
		
	TIM_ICPolarity						���񴥷�
	TIM_ICPolarity_Rising			�����ش���
	TIM_ICPolarity_Falling		�½��ش���
	TIM_ICPolarity_BothEdge		˫���ش���
	
	TIM_ICSelection							���벶��ѡ��
	TIM_ICSelection_DirectTI		ֱ��ӳ��ͬͨ��	ICxӳ�䵽TIx
	TIM_ICSelection_IndirectTI	���ӳ�䵽����һ��ͨ��	ICxӳ�䵽TIy		1��2��3��4		
	TIM_ICSelection_TRC					ӳ�䵽ͬͨ����TRC��
	
	TIM_ICPrescaler
	TIM_ICPSC_DIV1				һ��������һ����Ч����
	TIM_ICPSC_DIV2				����������һ����Ч����
	TIM_ICPSC_DIV4				�ĸ�������һ����Ч����
	TIM_ICPSC_DIV8				�˸�������һ����Ч����
	
	TIM_IT
	TIM_FLAG_Update			�����¼���־�������ã�
	TIM_FLAG_CC1				IC1�����¼�������־
	TIM_FLAG_CC2				IC2��׽�¼�������־
	TIM_FLAG_CC3				IC3��׽�¼�������־
	TIM_FLAG_CC4				IC4��׽�¼�������־	
	
	
	TIM_ICFilter
	[0,15]		���벶���˲���,��ֹ�Ӳ�,����Ϊ0


*/




class		TIM_Currency																														
{
	// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE)	���ô˺����򿪶�ʱ�����ö�ʱ������֮��Ҳ�ɹرգ�ʹ��ʱ����ͣ,Ĭ�Ͽ���
	public:
		void base																							//ͨ�ü�ʱ������	
		(
			TIM_TypeDef* TIMx,																	//��ʱ����� 
			uint32_t RCC_APB1Periph,														//��ʱ��ʱ������
			uint16_t TIM_Prescaler,															//��ʱ��Ԥ��Ƶ
			uint16_t TIM_CounterMode,														//��ʱ��������ʽ
			uint16_t TIM_Period,																//��ʱ������
			uint16_t TIM_ClockDivision													//ʱ�ӷ�Ƶ			
		);
		
		void out_pwm_inti																			//���PWM��ʼ������Ҫ�����Ӧ����������ΪGPIO_Mode_AF_PP	������������������ȳ�ʼ��base()����
		(
		TIM_TypeDef* TIMx,																		//��ʱ�����
		uint8_t  TIM_Channel,																	//��ʱ�����ͨ��
		uint16_t TIM_OCMode,																	//�Ƚ����ģʽ
		uint16_t TIM_OutputState,															//�Ƚ����ʹ��
		uint32_t TIM_Pulse,																		//������
		uint16_t TIM_OCPolarity,															//�������
		uint16_t TIM_OCIdleState															//����״̬�±Ƚ����״̬
		);
		
		void out_pwm																					//�仯PWM�����ں�ռ�ձ�,�ȳ�ʼ��out_pwm_inti()����ʹ��
		(			
			TIM_TypeDef* TIMx,							//TIM���		TIMx							
			uint8_t  TIM_Channel,						//TIMͨ��		TIM_channelx
			uint16_t TIM_Period,						//TIM����		[0,65535]
			uint32_t TIM_Pulse							//TIMռ�ձ�	[0,65535]
		);
		
		void	out_pwm_per																			//ֻ�ı�PWM��ռ�ձȣ��ȳ�ʼ��out_pwm_inti()����ʹ��
		(
			TIM_TypeDef* TIMx,
			uint8_t  TIM_Channel,
			uint32_t TIM_Pulse
		);
		
		void	in_pulse_inti																		//��׽�������壬��Ҫ�ȳ�ʼ��base()��������Ӧ���ų�ʼ��Ϊ	GPIO_Mode_IN_FLOATING	��������
		(
			TIM_TypeDef* TIMx,																	//��ʱ����� 
			uint16_t TIM_Channel,																//����ͨ��
			uint16_t TIM_ICPolarity,														//���벶�񴥷�
			uint16_t TIM_ICSelection,														//���벶��ѡ��
			uint16_t TIM_ICPrescaler,														//���벶��Ԥ��Ƶ��
			uint16_t TIM_ICFilter,															//���벶���˲���
			uint16_t TIM_FLAG,																	//�жϱ�־λ	����Ϊ	TIM_FLAG_CCx	xΪ���ͨ����
			uint8_t NVIC_IRQChannel,														//�ж�Դ
			uint8_t NVIC_IRQChannelPreemptionPriority,					//��ռ���ȼ�
			uint8_t NVIC_IRQChannelSubPriority									//�����ȼ�
		);
		
		
		
		void	in_pwm_inti																			//��׽����PWM����Ҫ�ȳ�ʼ��base()��������Ӧ���ų�ʼ�� GPIO_Mode_AF_PP �����������
		(
		TIM_TypeDef* TIMx,																		//��ʱ�����
		uint16_t TIM_Channel,																	//����ͨ��
		uint16_t TIM_ICPolarity,															//���벶�񴥷�
		uint16_t TIM_ICSelection,															//���벶��ѡ��
		uint16_t TIM_ICPrescaler,															//���벶��Ԥ��Ƶ��
		uint16_t TIM_IT,																			//������ͨ�������������Ǹ�ͨ���������ж�
		uint16_t TIM_ICFilter																	//���벶���˲���
		);
		//������PWM�������ֻ��Ϊ	72M/TIM_Period	��TIM_Period��base()������λ72�����������ֻ��Ϊ1ms
		
};



/*�߼���ʱ�������ص�ɲ���������ͻ��������ʹ�ò��࣬��ʱ��˵
����Ϊ�ο�
	// ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
	// �������õ�����ʱ��Ϊ 152ns
	TIM_BDTRInitStructure.TIM_DeadTime = 11;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// �� BKIN ���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	�������ʵ����ͬͨ�ü�ʱ����PWM�����ֻ������	TIM_OC1Init()Ҫ��Ľṹ�岹����ȫ����


class	TIM_Senior
{
	public:
		void base																							//�߼���ʱ������	
		(
			TIM_TypeDef* TIMx,																	//��ʱ����� 
			uint32_t RCC_APB1Periph,														//��ʱ��ʱ������
			uint16_t TIM_Prescaler,															//��ʱ��Ԥ��Ƶ
			uint16_t TIM_CounterMode,														//��ʱ��������ʽ
			uint16_t TIM_Period,																//��ʱ������
			uint16_t TIM_ClockDivision													//ʱ�ӷ�Ƶ			
		);
};
*/



#endif
