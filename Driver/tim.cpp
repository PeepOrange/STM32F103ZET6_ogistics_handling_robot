#include "tim.h"


void TIM_Base::base																				//������ʱ������
(
	TIM_TypeDef* TIMx,																	//��ʱ����� TIM6	/TIM7
	uint32_t RCC_APB1Periph,														//��ʱ��ʱ������
	FunctionalState NewState,														//ʹ�ܶ�ʱ��
	uint16_t TIM_Prescaler,															//��ʱ��Ԥ��Ƶ
	uint16_t TIM_Period	,																//��ʱ��������
	uint8_t NVIC_IRQChannel,														//�ж�Դ
	uint8_t NVIC_IRQChannelPreemptionPriority,					//��ռ���ȼ�
	uint8_t NVIC_IRQChannelSubPriority									//�����ȼ�
)
{
	TIM_TimeBaseInitTypeDef	tim_base;
	NVIC_InitTypeDef				tim_it;
	tim_base.TIM_Prescaler=TIM_Prescaler-1;
	tim_base.TIM_Period=TIM_Period-1;
	tim_it.NVIC_IRQChannel=NVIC_IRQChannel;
	tim_it.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	tim_it.NVIC_IRQChannelSubPriority=NVIC_IRQChannelSubPriority;
	tim_it.NVIC_IRQChannelCmd=NewState;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);
	TIM_ITConfig(TIMx,TIM_IT_Update ,NewState);
	TIM_TimeBaseInit(TIMx,&tim_base );
	NVIC_Init(&tim_it);
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
	TIM_Cmd(TIMx,NewState);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, DISABLE);
}

void	TIM_Currency::base
		(
			TIM_TypeDef* TIMx,																	//��ʱ����� 
			uint32_t RCC_APB1Periph,														//��ʱ��ʱ������
			uint16_t TIM_Prescaler,															//��ʱ��Ԥ��Ƶ
			uint16_t TIM_CounterMode,														//��ʱ��������ʽ
			uint16_t TIM_Period,																//��ʱ������
			uint16_t TIM_ClockDivision													//ʱ�ӷ�Ƶ			
		)

{
	TIM_TimeBaseInitTypeDef	tim_base;
	tim_base.TIM_Prescaler=TIM_Prescaler-1;
	tim_base.TIM_Period=TIM_Period-1;
	tim_base.TIM_CounterMode=TIM_CounterMode;
	tim_base.TIM_ClockDivision=TIM_ClockDivision;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);
	TIM_TimeBaseInit(TIMx,&tim_base );
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);
	TIM_Cmd(TIMx,ENABLE);
	
}



void TIM_Currency::out_pwm_inti																			//PWM��ʼ������Ҫ�����Ӧ����������ΪGPIO_Mode_AF_PP
		(
		TIM_TypeDef* TIMx,
		uint8_t  TIM_Channel,
		uint16_t TIM_OCMode,
		uint16_t TIM_OutputState,
		uint32_t TIM_Pulse,
		uint16_t TIM_OCPolarity,
		uint16_t TIM_OCIdleState
		)
{

	TIM_OCInitTypeDef	tim_oc_mode;
	tim_oc_mode.TIM_OCMode=TIM_OCMode;
	tim_oc_mode.TIM_OutputState=TIM_OutputState;
	tim_oc_mode.TIM_Pulse=TIM_Pulse;
	tim_oc_mode.TIM_OCPolarity=TIM_OCPolarity;
	tim_oc_mode.TIM_OCIdleState=TIM_OCIdleState;
	
	switch(TIM_Channel)
	{
		case	1:TIM_OC1Init(TIMx,&tim_oc_mode),TIM_OC1PreloadConfig(TIMx,TIM_OCPreload_Enable);break;
		case	2:TIM_OC2Init(TIMx,&tim_oc_mode),TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);break;
		case	3:TIM_OC3Init(TIMx,&tim_oc_mode),TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);break;
		case	4:TIM_OC4Init(TIMx,&tim_oc_mode),TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);break;
	}
	
}


void TIM_Currency::out_pwm
		(		
			TIM_TypeDef* TIMx,
			uint8_t  TIM_Channel,
			uint16_t TIM_Period,
			uint32_t TIM_Pulse	
		)	
{
	TIMx->ARR =TIM_Period;
	
	switch(TIM_Channel)
	{
		case	1:TIMx->CCR1 =TIM_Pulse-1;break;
		case	2:TIMx->CCR2 =TIM_Pulse-1;break;
		case	3:TIMx->CCR3 =TIM_Pulse-1;break;
		case	4:TIMx->CCR4 =TIM_Pulse-1;break;
	}	
	
}

void	TIM_Currency::out_pwm_per																			//ֻ�ı�PWM��ռ�ձ�
		(
			TIM_TypeDef* TIMx,
			uint8_t  TIM_Channel,
			uint32_t TIM_Pulse
		)
{
	switch(TIM_Channel)
	{
		case	1:TIMx->CCR1 =TIM_Pulse-1;;break;
		case	2:TIMx->CCR2 =TIM_Pulse-1;;break;
		case	3:TIMx->CCR3 =TIM_Pulse-1;;break;
		case	4:TIMx->CCR4 =TIM_Pulse-1;;break;
	}		
}


void	TIM_Currency::in_pulse_inti																		//��׽�������壬��Ҫ�ȳ�ʼ��base()��������Ӧ���ų�ʼ��Ϊ��������
		(
			TIM_TypeDef* TIMx,																	//��ʱ����� 
			uint16_t TIM_Channel,																//����ͨ��
			uint16_t TIM_ICPolarity,														//���벶�񴥷�
			uint16_t TIM_ICSelection,														//���벶��ѡ��
			uint16_t TIM_ICPrescaler,														//���벶��Ԥ��Ƶ��
			uint16_t TIM_ICFilter,															//���벶���˲���
			uint16_t TIM_FLAG	,																	//�жϱ�־λ
			uint8_t NVIC_IRQChannel,														//�ж�Դ
			uint8_t NVIC_IRQChannelPreemptionPriority,					//��ռ���ȼ�
			uint8_t NVIC_IRQChannelSubPriority									//�����ȼ�
		)
{
	TIM_ICInitTypeDef	TIM_ICInitStruct;
	NVIC_InitTypeDef				tim_it;
	TIM_ICInitStruct.TIM_Channel=TIM_Channel;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity;
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection;
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPrescaler;
	TIM_ICInitStruct.TIM_ICFilter=TIM_ICFilter;
	TIM_ClearFlag(TIMx, TIM_FLAG_Update|TIM_FLAG);
	tim_it.NVIC_IRQChannel=NVIC_IRQChannel;
	tim_it.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	tim_it.NVIC_IRQChannelSubPriority=NVIC_IRQChannelSubPriority;
	tim_it.NVIC_IRQChannelCmd=ENABLE;	
	
	TIM_ICInit(TIMx,& TIM_ICInitStruct);
	TIM_Cmd(TIMx, ENABLE);
	NVIC_Init(&tim_it);	
}


void	TIM_Currency::in_pwm_inti
		(
		TIM_TypeDef* TIMx,
		uint16_t TIM_Channel,
		uint16_t TIM_ICPolarity,
		uint16_t TIM_ICSelection,
		uint16_t TIM_ICPrescaler,
		uint16_t TIM_IT,
		uint16_t TIM_ICFilter
		)
{
	TIM_ICInitTypeDef	TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel=TIM_Channel;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity;
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection;
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPrescaler;
	TIM_ICInitStruct.TIM_ICFilter=TIM_ICFilter;	
	TIM_PWMIConfig(TIMx,&TIM_ICInitStruct);

  TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);

 // ѡ���ģʽ: ��λģʽ
 // PWM ����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������ CNT �ᱻ��λ
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIMx,TIM_MasterSlaveMode_Enable);

	// ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
	TIM_ITConfig(TIMx, TIM_IT, ENABLE);
	// ����жϱ�־λ
	TIM_ClearITPendingBit(TIMx, TIM_IT);

	// ʹ�ܶ�ʱ������������ʼ����
	TIM_Cmd(TIMx, ENABLE);
};



//TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);


