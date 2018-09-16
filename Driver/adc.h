#ifndef __adc_H
#define __adc_H

#include "stm32f10x.h"                  // Device header
#include "gpio.h"
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC


/*��ѹ���뷶Χ		����Ϊ0-3.3V����
ADC ���뷶ΧΪ��V REF- �� V IN �� V REF+ ���� V REF- ��V REF+ ��V DDA ��V SSA �����ĸ��ⲿ
���ž�����
���������ԭ��ͼ��ʱ��һ���V SSA ��V REF- �ӵأ���V REF+ ��V DDA ��3V3���õ�ADC
�������ѹ��ΧΪ��0~3.3V��
���������������ĵ�ѹ��Χ���ȥ�����Բ��Ը���ѹ���߸��ߵ�����ѹ�����ǿ�
�����ⲿ��һ����ѹ�����·������Ҫת���ĵ�ѹ̧�����߽�ѹ�� 0~3.3V������ ADC ��
���Բ�����
*/

/*����ͨ��

ͨ��		ADC1								ADC2						ADC3
0				A0									A0							A0
1				A1									A1							A1
2				A2									A2							A2
3				A3									A3							A3
4				A4									A4							��
5				A5									A5							��
6				A6									A6							��
7				A7									A7							��
8				B0									B0							��
9				B1									B1							�ڲ�VSS
10			C0									C0							C0
11			C1									C1							C1
12			C2									C2							C2
13			C3									C3							C3
14			C4									C4							�ڲ�VSS
15			C5									C5							�ڲ�VSS
16			�ڲ��¶ȴ�����			�ڲ�VSS					�ڲ�VSS
17			�ڲ�Vrefint					�ڲ�VSS					�ڲ�VSS

*/

/*
��������ΪADCͨ��ת����˳���Ⱥ�֮�֣�[0,17]ͨ���Կ�������Ϊ��������
ע������Ϊ��ӣ������жϣ�����ת����[1,4]ͨ���ſ�������Ϊע������
*/

/*
ģ���ѹ12λ
�� 3.3V��Ӧ2^12
��ѹ= (3.3 * ��������� ) / 2^12		����Ϊ
*/


/*
inti_adc()����˵��
	
		ADC_Mode
		
		ADC_Mode_Independent							����ģʽ	������ҪADCͬ�����߾���һ��ADCѡ�������		
		ADC_Mode_RegInjecSimult						���ͬ������+ע��ͬ��					
		ADC_Mode_RegSimult_AlterTrig			���ͬ������+���津��
		ADC_Mode_InjecSimult_FastInterl		���ͬ��ע��+���ٽ���
		ADC_Mode_InjecSimult_SlowInterl		���ͬ������+���ٽ���
		ADC_Mode_InjecSimult							ע��ͬ��
		ADC_Mode_RegSimult								����ͬ��
		ADC_Mode_FastInterl								���ٽ���
		ADC_Mode_SlowInterl								���ٽ���
		ADC_Mode_AlterTrig								��������
		
		
		ͬ��ע��ģʽ ADC1 �� ADC2 ͬʱת��һ��ע��ͨ���飬���� ADC1 Ϊ����ADC2Ϊ�ӡ� ת�������ݴ洢��ÿ�� ADC�ӿڵ� ADC_JDRx�Ĵ����С�
		ͬ������ģʽ ADC1 �� ADC2 ͬʱת��һ������ͨ���飬���� ADC1 Ϊ����ADC2Ϊ�ӡ�ADC1ת���Ľ������ADC1_DR�ĵ� 16λ��ADC2ת���Ľ������ ADC1_DR �ĸ�ʮ��λ��
		���ٽ���ģʽ ADC1 �� ADC2 ����ɼ�һ������ͨ���飨ͨ��Ϊһ��ͨ��������ADC2 ����֮��ADC1��Ҫ�ȴ� 7�� ADCCLK֮����ܴ�����
		���ٽ���ģʽ ADC1 �� ADC2 ����ɼ�һ������ͨ���飨ֻ��Ϊһ��ͨ��������ADC2 ����֮��ADC1��Ҫ�ȴ� 14 �� ADCCLK֮����ܴ�����
		���津��ģʽ ADC1�� ADC2�����ɼ�ע��ͨ���飬�� ADC1����ͨ���ɼ����֮���ٲɼ� ADC2 ��ͨ�������ѭ����������ɼ���һ����
		��ϵĹ���/ע��ͬ��ģʽ	������ͬ��ת�����жϣ�������ע�����ͬ��ת�����ֿ�����ģʽ�����Ϳ����ˣ��������ע��������жϹ������ת����
		��ϵ�ͬ������+���津��ģʽ	������ͬ��ת�����жϣ�������ע���齻�津��ת�����ֿ�����ģʽ�����Ϳ����ˣ��������ע��������жϹ������ת����
		���ͬ��ע��+����ģʽ	����ת�����Ա�ͬ��ע��ģʽ�жϡ� ��������£�����ת�����жϣ�ע��ת����������
	
	ADC_ScanConvMode	ͨ��ɨ��
		
		ENABLE			��ͨ������
		DISABLE			��ͨ���ر�
	
	ADC_ContinuousConvMode	
	
	DISABLE			����ת���ر�
	ENABLE			����ת��������������
	
	ADC_ExternalTrigConv	����ѡ��			
	
	ADC_ExternalTrigConv_T1_CC1									ADC1��ADC2							��ʱ��1_CC1�¼�					
	ADC_ExternalTrigConv_T1_CC2									ADC1��ADC2							��ʱ��1_CC2�¼�	
	ADC_ExternalTrigConv_T2_CC2									ADC1��ADC2							��ʱ��2_CC2�¼�
	ADC_ExternalTrigConv_T3_TRGO								ADC1��ADC2							��ʱ��3_TRGO�¼�
	ADC_ExternalTrigConv_T4_CC4									ADC1��ADC2							��ʱ��4_CC4�¼�
	ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO			ADC1��ADC2��ADC3				�ⲿ�ж�:���ڹ���ͨ����ѡ��EXTI��11��TIM8_TRGO��Ϊ�ⲿ�����¼�����ע��ͨ������ѡ��EXTI��15��TIM8_CC4��Ϊ�ⲿ�����¼���
	ADC_ExternalTrigConv_T1_CC3									ADC1��ADC2��ADC3				��ʱ��1_CC3�¼�
	ADC_ExternalTrigConv_None										ADC1��ADC2��ADC3  			���ⲿ����Դ�������������
	ADC_ExternalTrigConv_T3_CC1									ADC3										��ʱ��3_CC1�¼�
	ADC_ExternalTrigConv_T2_CC3									ADC3										��ʱ��2_CC3�¼�
	ADC_ExternalTrigConv_T8_CC1									ADC3										��ʱ��8_CC1�¼�
	ADC_ExternalTrigConv_T8_TRGO								ADC3										��ʱ��8_TRGO�¼�
	ADC_ExternalTrigConv_T5_CC1									ADC3										��ʱ��5_CC1�¼�
	ADC_ExternalTrigConv_T5_CC3									ADC3										��ʱ��5_CC3�¼�
	
	
	ADC_DataAlign					���ݼĴ��������ʽ
	
	ADC_DataAlign_Right		�Ҷ���
	ADC_DataAlign_Left		�����
	
	ADC_NbrOfChannel			ADC�ɼ�ͨ����Ŀ
	ADC_Channel_[0,17]		�磺ADC_Channel_0	ADC_Channel_12
	
	RCC_PCLK2				ADCʱ�ӷ�Ƶ(ADCCLK)��ADCΪPCLK2�ķ�Ƶ������PCLK2λ72Mhz

	RCC_PCLK2_Div6		12Mhz
	RCC_PCLK2_Div8		9Mhz
	
*/	


/*	inti_channel()����˵��	
	
	ADC_Channel				ADC�ɼ���ͨ��	ADC_Channel_[0,17]		�磺ADC_Channel_0	ADC_Channel_12
	
	Rank							ת����˳��[1,16]
	
	ADC_SampleTime		�ɼ�ʱ��   �ɼ�ʱ��Խ������Խ�ߡ���֮��Խ�̾���ҲԽ��
	
	ADC_SampleTime_1Cycles5				1.5��ʱ������
	ADC_SampleTime_7Cycles5				7.5��ʱ������
	ADC_SampleTime_13Cycles5			13.5��ʱ������
	ADC_SampleTime_28Cycles5			28.5��ʱ������
	ADC_SampleTime_41Cycles5			41.5��ʱ������
	ADC_SampleTime_55Cycles5			55.5��ʱ������
	ADC_SampleTime_71Cycles5			71.5��ʱ������
	ADC_SampleTime_239Cycles5			239.5��ʱ������
	
*/


/*
inti_it()����˵��
ADC_IT				�жϱ�־

ADC_IT_EOC		������ת������		
ADC_IT_AWD		ģ�⿴�Ź�״̬λ
ADC_IT_JEOC		ע����ת������

*/


class ADC																			
{
	void inti_adc																//������Ĭ�Ͽ�����ͨ��ADC		���δ���ж������ADC_SoftwareStartConvCmd(ADCx, ENABLE)��ʼadcת��
	(
	uint32_t RCC_APB2Periph,										//ADCͨ��ʱ��		RCC_APB2Periph_ADC1		RCC_APB2Periph_ADC2		RCC_APB2Periph_ADC3
	ADC_TypeDef* ADCx,													//�˲����� ѡ ADC1	ADC2	ADC3
	GPIO *InputPin,															//ADC�ɼ�������  ��ʼ��Ϊģ�����루�����ã�
	uint32_t ADC_Mode,													//ADC����ģʽѡ��
	FunctionalState ADC_ScanConvMode,						//ADCɨ�裨��ͨ������ADC���Σ���ͨ����
	FunctionalState ADC_ContinuousConvMode,			//ADC����ת��������ת��
	uint32_t ADC_ExternalTrigConv,							//ADCת�������ź�
	uint32_t ADC_DataAlign,											//���ݼĴ��������ʽ
	uint8_t  ADC_NbrOfChannel,									//ADC�ɼ�ͨ����Ŀ
	uint32_t RCC_PCLK2													//ADCʱ�ӷ�Ƶ
	);
	
	void inti_channel														//��ʼ������ͨ��
	(
	ADC_TypeDef* ADCx,													//�˲����� ѡ ADC1	ADC2	ADC3
	uint8_t ADC_Channel, 												//�ɼ�ͨ��
	uint8_t Rank, 															//ת��˳��
	uint8_t ADC_SampleTime											//����ʱ��
	);
	
	void inti_it																//ADC����ж����� 		�Ѿ������������ת����ʼ��ת����ɽ����ж�
	(
	 ADC_TypeDef* ADCx, 												//�˲�����ѡ ADC1	ADC2	ADC3
	 uint16_t ADC_IT,														//�жϱ�־
   FunctionalState NewState	,									//����/�ر�		ENABLE / DISABLE
	 uint8_t NVIC_IRQChannel ,									//�ж�Դ��ѡ		ADC1_2_IRQn	  ADC3_IRQn�������Ļ���ļ���δ�������жϣ��������Ӳ����ԣ�
	 uint8_t NVIC_IRQChannelPreemptionPriority,	//��ռ���ȼ�		��ϸ�ɿ�EXTI.h�жϵ�����
	 uint8_t NVIC_IRQChannelSubPriority					//�����ȼ�
	);
	
	void cmd_adc(ADC_TypeDef* ADCx, FunctionalState NewState);  //ADCx(ADC1	ADC2	ADC3)ͨ������(ENABLE)��ر�(DISABLE)
	
	void cmd_dma(ADC_TypeDef* ADCx, FunctionalState NewState);	//����ADCx��DMA����,���������ö�Ӧ��DMA���ã���dma.h��
	
	
	uint16_t get(ADC_TypeDef* ADCx);   				  //���ADCx��������ͨ��ת�����ֵ
	
};




#endif
