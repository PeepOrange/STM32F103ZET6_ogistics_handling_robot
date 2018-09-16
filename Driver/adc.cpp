#include "adc.h"


void  ADC::inti_adc
(
	uint32_t RCC_APB2Periph,										//ADC通道时钟		RCC_APB2Periph_ADC1		RCC_APB2Periph_ADC2		RCC_APB2Periph_ADC3
	ADC_TypeDef* ADCx,													//此参数可 选 ADC1	ADC2	ADC3
	GPIO *InputPin,															//ADC采集的引脚  初始化为模拟输入（已配置）
	uint32_t ADC_Mode,													//ADC工作模式选择
	FunctionalState ADC_ScanConvMode,						//ADC扫描（多通道）、ADC单次（单通道）
	FunctionalState ADC_ContinuousConvMode,			//ADC单次转换或连续转换
	uint32_t ADC_ExternalTrigConv,							//ADC转换触发信号
	uint32_t ADC_DataAlign,											//数据寄存器对齐格式
	uint8_t ADC_NbrOfChannel,										//ADC采集通道数目
	uint32_t RCC_PCLK2													//ADC时钟分频
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
	ADC_ResetCalibration(ADCx); // 初始化 ADC 校准寄存器
	while (ADC_GetResetCalibrationStatus(ADCx));// 等待校准寄存器初始化完成
	ADC_StartCalibration(ADCx);// ADC 开始校准
	while (ADC_GetCalibrationStatus(ADCx));// 等待校准完成

}



void ADC::cmd_adc(ADC_TypeDef* ADCx, FunctionalState NewState)
{
	
	ADC_Cmd( ADCx,NewState);
	
}

void ADC::inti_it																//ADC软件中断设置
	(
	 ADC_TypeDef* ADCx, 												//此参数可 选 ADC1	ADC2	ADC3
	 uint16_t ADC_IT,														//中断标志
   FunctionalState NewState	,									//开启/关闭		ENABLE	DISABLE
	 uint8_t NVIC_IRQChannel ,									//中断源
	 uint8_t NVIC_IRQChannelPreemptionPriority,	//抢占优先级
	 uint8_t NVIC_IRQChannelSubPriority					//子优先级
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
	ADC_TypeDef* ADCx,													//此参数可 选 ADC1	ADC2	ADC3
	uint8_t ADC_Channel, 												//采集通道
	uint8_t Rank, 															//转换顺序
	uint8_t ADC_SampleTime											//采样时间
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



