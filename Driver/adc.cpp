#include "adc.h"


void  ADC::inti_adc
(
	uint32_t RCC_APB2Periph,										//ADCͨ��ʱ��		RCC_APB2Periph_ADC1		RCC_APB2Periph_ADC2		RCC_APB2Periph_ADC3
	ADC_TypeDef* ADCx,													//�˲����� ѡ ADC1	ADC2	ADC3
	GPIO *InputPin,															//ADC�ɼ�������  ��ʼ��Ϊģ�����루�����ã�
	uint32_t ADC_Mode,													//ADC����ģʽѡ��
	FunctionalState ADC_ScanConvMode,						//ADCɨ�裨��ͨ������ADC���Σ���ͨ����
	FunctionalState ADC_ContinuousConvMode,			//ADC����ת��������ת��
	uint32_t ADC_ExternalTrigConv,							//ADCת�������ź�
	uint32_t ADC_DataAlign,											//���ݼĴ��������ʽ
	uint8_t ADC_NbrOfChannel,										//ADC�ɼ�ͨ����Ŀ
	uint32_t RCC_PCLK2													//ADCʱ�ӷ�Ƶ
)
{
	InputPin->mode(GPIO_Mode_AIN,GPIO_Speed_50MHz);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph,ENABLE);
	ADC_InitTypeDef ADC_inti_mode;
	ADC_inti_mode.ADC_Mode=ADC_Mode;
	ADC_inti_mode.ADC_ScanConvMode=ADC_ScanConvMode;
	ADC_inti_mode.ADC_ContinuousConvMode=ADC_ContinuousConvMode;
	ADC_inti_mode.ADC_ExternalTrigConv=ADC_ExternalTrigConv;
	ADC_inti_mode.ADC_DataAlign=ADC_DataAlign;
	ADC_inti_mode.ADC_NbrOfChannel=ADC_NbrOfChannel;
	ADC_Init(ADCx,&ADC_inti_mode);
	
	RCC_ADCCLKConfig(RCC_PCLK2);
	
	ADC_Cmd(ADCx, ENABLE);
	ADC_ResetCalibration(ADCx); // ��ʼ�� ADC У׼�Ĵ���
	while (ADC_GetResetCalibrationStatus(ADCx));// �ȴ�У׼�Ĵ�����ʼ�����
	ADC_StartCalibration(ADCx);// ADC ��ʼУ׼
	while (ADC_GetCalibrationStatus(ADCx));// �ȴ�У׼���

}



void ADC::cmd_adc(ADC_TypeDef* ADCx, FunctionalState NewState)
{
	
	ADC_Cmd( ADCx,NewState);
	
}

void ADC::inti_it																//ADC����ж�����
	(
	 ADC_TypeDef* ADCx, 												//�˲����� ѡ ADC1	ADC2	ADC3
	 uint16_t ADC_IT,														//�жϱ�־
   FunctionalState NewState	,									//����/�ر�		ENABLE	DISABLE
	 uint8_t NVIC_IRQChannel ,									//�ж�Դ
	 uint8_t NVIC_IRQChannelPreemptionPriority,	//��ռ���ȼ�
	 uint8_t NVIC_IRQChannelSubPriority					//�����ȼ�
	)
{
	ADC_ITConfig(ADCx, ADC_IT_EOC, NewState);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd=NewState;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=NVIC_IRQChannelSubPriority;
	NVIC_Init(&NVIC_InitStructure);
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
	
}


void ADC::inti_channel
	(
	ADC_TypeDef* ADCx,													//�˲����� ѡ ADC1	ADC2	ADC3
	uint8_t ADC_Channel, 												//�ɼ�ͨ��
	uint8_t Rank, 															//ת��˳��
	uint8_t ADC_SampleTime											//����ʱ��
	)
{
	ADC_RegularChannelConfig(ADCx,ADC_Channel,Rank,ADC_SampleTime);
	
}

void ADC::cmd_dma(ADC_TypeDef* ADCx, FunctionalState NewState)
{
	ADC_DMACmd(ADCx,NewState);
}

uint16_t get(ADC_TypeDef* ADCx)
{
	 return (uint16_t) ADCx->DR;
}



