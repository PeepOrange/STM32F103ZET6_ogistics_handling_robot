#ifndef __dma_H_
#define __dma_H_
#include "stm32f10x.h"                  // Device header


/*inti() ����˵��  ���������洢��ģʽ������һ���洢����Ϊ���裨���������ڴ棩

��ʼ����ɺ��뿪����Ӧ�����DMA���ڶ�Ӧ��������ļ��п�������	
{										
USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
USART_DMACmd �������ڿ��� USART �� DMA ����������͹رա�������������
������һ�������������ô������裬������ USART1/2/3 �� UART4/5 �� 5 ��������ѡ����
�����������ô��ڵľ��� DMA �����д��ڷ������� USART_DMAReq_Tx �ͽ�������
USART_DMAReq_Rx ��ѡ���������������������������� ENABLE ���߹ر�����
DISABLE�����иú�����USART��DMA���ʹ���Ϳ�ʼ�ˣ��������ô洢�������ݻᷢ
�͵����ڡ�
}
										
 DMAy_Channelx   
 DMAͨ��						���裨�洢�����洢��������ѡ��ͨ����
 DMA1_Channel1      ADC1	TIM2_CH3	TIM4_CH1
 DMA1_Channel2      SPI1_RX		USART3_TX		TIM1_CH1	TIM2_UP		TIM3_CH3	 
 DMA1_Channel3      SPI1_TX		USART3_RX		TIM1_CH2	TIM3_CH4	TIM3_UP 
 DMA1_Channel4      SPI/I2S2_RX		USART1_TX		I2C2_TX		TIM1_TX4	TIM1_TRIG		TIMI_COM	TIM4_CH2
 DMA1_Channel5      SPI/I2S2_TX		USART1_RX		I2C2_RX		TIM1_UP		TIM2_CH1		TIM4_CH3
 DMA1_Channel6      USART2_RX		I2C1_TX		TIM1_CH3		TIM3_CH1		TIM3_TRIG		
 DMA1_Channel7      USART2_TX 	I2C1_RX		TIM2_CH2		TIM2_CH4		TIM4_UP
 
 DMA2_Channel1      SPI/I2S3_RX		TIM5_CH4	TIM5_TRIG		TIM8_CH3		TIM8_UP
 DMA2_Channel2      SPI/I2S3_TX 	TIM5_CH3	TIM5_UP			TIM8_CH4		TIM8_TRIG		TIM8_COM
 DMA2_Channel3      UART4_RX			TIM6_UP/DACͨ��1			TIM8_CH1
 DMA2_Channel4      SDIO					TIM5_CH2	TIM7_UP/DACͨ��2		
 DMA2_Channel5      UART4_TX 			ADC3			TIM5_CH1		TIM8_CH2
 
 DMA_PeripheralBaseAddr		һ������Ϊ��������ݼĴ�����ַ������Ǵ洢�����洢��ģʽ������Ϊ����һ���洢����ַ��
 
 DMA_MemoryBaseAddr				һ������Ϊ�����Զ���洢�����׵�ַ��
 
 DMA_DIR
	 DMA_DIR_PeripheralDST	������ΪĿ�ĵ�
	 DMA_DIR_PeripheralSRC	������Ϊ����Դ
 
 DMA_BufferSize
	ָ����DMA�Ļ����С����λΪ DMA_PeripheralDataSize	��	DMA_PeripheralDataSize
 
 DMA_PeripheralInc
	DMA_PeripheralInc_Enable		����Ĵ�����ַ����ʹ��
	DMA_PeripheralInc_Disable		����Ĵ�����ַ�����رգ���������Ĵ�����ַ���䣩
 
 DMA_MemoryInc
	DMA_MemoryInc_Enable		�洢����ַ����ʹ�ܣ�������������ַ������
	DMA_MemoryInc_Disable		�洢����ַ�����ر�
 
 DMA_PeripheralDataSize			��������ݴ�С��λ
	DMA_PeripheralDataSize_Byte			8λ ��һ���ֽڣ�
	DMA_PeripheralDataSize_HalfWord	16λ�������ֽڣ�
	DMA_PeripheralDataSize_Word			32λ���ĸ��ֽڣ�
 
 DMA_MemoryDataSize					�洢����ַ��С��λ������������������ʱӦ��������һ�£�
	 DMA_MemoryDataSize_Byte		    8λ ��һ���ֽڣ�
	 DMA_MemoryDataSize_HalfWord		16λ�������ֽڣ�
	 DMA_MemoryDataSize_Word				32λ���ĸ��ֽڣ�
 
 DMA_Mode				ģʽѡ��
	 DMA_Mode_Circular		ѭ������
	 DMA_Mode_Normal			һ�δ���
 
 DMA_Priority			ͨ�����ȼ�
	 DMA_Priority_VeryHigh		�ǳ���
	 DMA_Priority_High				��
	 DMA_Priority_Medium			�е�
	 DMA_Priority_Low					��
 
 DMA_M2M				�洢�����洢��ģʽ
 DMA_M2M_Enable		�洢�����洢��ģʽʹ��
 DMA_M2M_Disable	�洢�����洢��ģʽ�ر�
 
 RCC_AHBPeriph   AHBʱ���߿���DMAʱ��
 RCC_AHBPeriph_DMA1  DMAͨ��һ
 RCC_AHBPeriph_DMA2	 DMAͨ����
 
*/

/*cmd()		��ʼ����Ĭ�Ͽ�����Ӧͨ��
	DMAy_Channelx Ϊ DMA1_Channel1 ��	DMA2_Channel5

	ENABLE	 			����
	DISABLE			  �ر�
*/

/*flag()  ����RESET��SET
	DMAy_FLAG_GLx   DMAyd��ͨ��x��ȫ���жϱ�־                  
	DMAy_FLAG_TCx   DMAyd��ͨ��x�Ĵ�����ɱ�־                   
	DMAy_FLAG_HTx   DMAyd��ͨ��x�İ봫���־                  
	DMAy_FLAG_TEx   DMAyd��ͨ��x�Ĵ�������־                   
*/

//(u32)(&( ADCx->DR ))  ADCx�����ݼĴ�����ַ



class DMA
{
	public:
	void inti
	 (DMA_Channel_TypeDef* DMAy_Channelx,    //DMAͨ��
		uint32_t DMA_PeripheralBaseAddr,			 //�����ַ
		uint32_t DMA_MemoryBaseAddr,					 //��������ַ
		uint32_t DMA_DIR,											 //���䷽��
		uint32_t DMA_BufferSize,						   //������Ŀ
		uint32_t DMA_PeripheralInc,						 //�����ַ����
		uint32_t DMA_MemoryInc,								 //�洢����ַ����
		uint32_t DMA_PeripheralDataSize,			 //�������ݿ��
		uint32_t DMA_MemoryDataSize,					 //�洢�����ݿ��
		uint32_t DMA_Mode,										 //ģʽѡ��
		uint32_t DMA_Priority,								 //ͨ�����ȼ�
		uint32_t DMA_M2M,											 //�洢����������ģʽ
		uint32_t RCC_AHBPeriph								 //AHBʱ���߿���DMAʱ��
	 );
	void cmd(DMA_Channel_TypeDef* DMAy_Channelx,FunctionalState NewState);  //������ر�DMAͨ��(��ʼ����Ĭ�Ͽ���)
	
	FlagStatus flag(uint32_t DMAy_FLAG);
	
};



#endif

