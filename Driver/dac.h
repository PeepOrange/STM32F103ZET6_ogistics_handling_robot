#ifndef	__dac_h_
#define	__dac_h_
#include "stm32f10x.h"                  // Device header

/*
��ѹ˵��
�� ADC �������ƣ�DAC Ҳʹ�� V REF+ ������Ϊ�ο���ѹ�������ԭ��ͼ��ʱ��һ���
V SSA �ӵأ��� V REF+ �� V DDA �� 3.3V���ɵõ� DAC �������ѹ��ΧΪ��0~3.3V��
�����������ĵ�ѹ��Χ����������ⲿ��һ����ѹ�����·���� 0~3.3V �� DAC
���̧�����ض��ķ�Χ���ɡ�

PA4-ͨ�� 1��PA5-ͨ�� 2��Ϊ������ţ�ʹ�� DAC����ʱ��DACͨ��������Ҫ�����ó�ģ�����빦�ܣ�AIN��

*/


/*
inti()����˵��
		DAC_Channel
		DAC_Channel_1		DACͨ��1
		DAC_Channel_2		DACͨ��2

		DAC_Trigger						�����¼�
		DAC_Trigger_None			��
		DAC_Trigger_T6_TRGO		TIM6����
		DAC_Trigger_T8_TRGO		TIM8����	
		DAC_Trigger_T3_TRGO		TIM3����
		DAC_Trigger_T7_TRGO		TIM7����
		DAC_Trigger_T5_TRGO		TIM5����
		DAC_Trigger_T15_TRGO	TIM15����
		DAC_Trigger_T2_TRGO		TIM2����
		DAC_Trigger_T4_TRGO		TIM4����
		DAC_Trigger_Ext_IT9		�ⲿ�ж�	EXTI_Line9�ⲿ�ж���
		DAC_Trigger_Software	�������
		

		DAC_WaveGeneration		�Ƿ��Զ�������������ǲ�
		DAC_WaveGeneration_None				�����������������ǲ�
		DAC_WaveGeneration_Triangle		�������ǲ�
		DAC_WaveGeneration_Noise			��������
		

		DAC_LFSRUnmask_TriangleAmplitude			ѡ�������������ĵ�ͨ�˲������ǲ��ķ�ֵ����� ����һ��ΪDAC_WaveGeneration_None���������Ч
		 DAC_LFSRUnmask_Bit0              		α��������λ��Խ������Խ��
		 DAC_LFSRUnmask_Bits1_0            
		 DAC_LFSRUnmask_Bits2_0            
		 DAC_LFSRUnmask_Bits3_0            
		 DAC_LFSRUnmask_Bits4_0            
		 DAC_LFSRUnmask_Bits5_0           
		 DAC_LFSRUnmask_Bits6_0           
		 DAC_LFSRUnmask_Bits7_0             
		 DAC_LFSRUnmask_Bits8_0            
		 DAC_LFSRUnmask_Bits9_0           
		 DAC_LFSRUnmask_Bits10_0           
		 DAC_LFSRUnmask_Bits11_0            
		 DAC_TriangleAmplitude_1           		���ǲ�����λ��Խ�����ǵ�б��Խ��
		 DAC_TriangleAmplitude_3           
		 DAC_TriangleAmplitude_7            
		 DAC_TriangleAmplitude_15          
		 DAC_TriangleAmplitude_31           
		 DAC_TriangleAmplitude_63          
		 DAC_TriangleAmplitude_127        
		 DAC_TriangleAmplitude_255          
		 DAC_TriangleAmplitude_511         
		 DAC_TriangleAmplitude_1023         
		 DAC_TriangleAmplitude_2047         
		 DAC_TriangleAmplitude_4095		
		
		DAC_OutputBuffer				ѡ���Ƿ�ʹ�����������	
		DAC_OutputBuffer_Enable		ʹ��
		DAC_OutputBuffer_Disable	�ر�
		ʹ���� DAC ������������Լ�С����迹���ʺ�ֱ������һЩ�ⲿ���ء�
		

*/

/*
uint32_t DAC_Channel
DAC_Channel_1		DACͨ��1
DAC_Channel_2		DACͨ��2

uint32_t DAC_Align 
DAC_Align_12b_R		12λDAC���ݼĴ����Ҷ���
DAC_Align_12b_L		12λDAC���ݼĴ��������
DAC_Align_8b_R		8λDAC���ݼĴ����Ҷ���

uint16_t Data
[0,65535]				DAC������

*/

/*
������ض�����������㷨
��������Dac���ж�ԴΪ��ʱ������������ָ���Ķ�ʱ�������TIM_SelectOutputTrigger(TIMx, TIM_TRGOSource_Update)һ��ѡ�񴥷�Դ������ʱ������������Ӧ�ĵ�ѹ��
һ��ʼ��������DMA,Դ���ݵ�ַΪ&const xx xx[xx],Ŀ���ַΪ	
		DAC->DHR12R1	DAC1ͨ��12λ�Ҷ���		
		DAC->DHR12L1	DAC1ͨ��12λ�����
		DAC->DHR8R1		DAC1ͨ��8λ�Ҷ���
		
		DAC->DHR12R2	DAC2ͨ��12λ�Ҷ���
		DAC->DHR12L2	DAC2ͨ��12λ�����
		DAC->DHR8R2		DAC2ͨ��8λ�Ҷ���
		
		DAC->DHR12RD	DAC˫ͨ��12λ�Ҷ���
		DAC->DHR12LD	DAC˫ͨ��12λ�����
		DAC->DHR8RD		DAC˫ͨ��8λ�Ҷ���

*/




class Dac
{
	public:
	void inti																					//��ʼ������ɺ�gpio����ΪGPIO_Mode_AIN(δ����),����ʹ��dma(�Ѿ�ʹ��)
	(
	uint32_t DAC_Channel,															//DACͨ��
	uint32_t DAC_Trigger,															//������ʽ
	uint32_t DAC_WaveGeneration,											//�Ƿ��Զ�������������ǲ�
	uint32_t DAC_LFSRUnmask_TriangleAmplitude,				//ѡ�������������ĵ�ͨ�˲������ǲ��ķ�ֵ 
	uint32_t DAC_OutputBuffer													//ѡ���Ƿ�ʹ�����������
	);
	
	void set_data(uint32_t DAC_Channel,uint32_t DAC_Align, uint16_t Data);		//�������ֵ
	

};





#endif

