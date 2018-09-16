#ifndef __adc_H
#define __adc_H

#include "stm32f10x.h"                  // Device header
#include "gpio.h"
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC


/*电压输入范围		正常为0-3.3V左右
ADC 输入范围为：V REF- ≤ V IN ≤ V REF+ 。由 V REF- 、V REF+ 、V DDA 、V SSA 、这四个外部
引脚决定。
我们在设计原理图的时候一般把V SSA 和V REF- 接地，把V REF+ 和V DDA 接3V3，得到ADC
的输入电压范围为：0~3.3V。
如果我们想让输入的电压范围变宽，去到可以测试负电压或者更高的正电压，我们可
以在外部加一个电压调理电路，把需要转换的电压抬升或者降压到 0~3.3V，这样 ADC 就
可以测量。
*/

/*输入通道

通道		ADC1								ADC2						ADC3
0				A0									A0							A0
1				A1									A1							A1
2				A2									A2							A2
3				A3									A3							A3
4				A4									A4							无
5				A5									A5							无
6				A6									A6							无
7				A7									A7							无
8				B0									B0							无
9				B1									B1							内部VSS
10			C0									C0							C0
11			C1									C1							C1
12			C2									C2							C2
13			C3									C3							C3
14			C4									C4							内部VSS
15			C5									C5							内部VSS
16			内部温度传感器			内部VSS					内部VSS
17			内部Vrefint					内部VSS					内部VSS

*/

/*
规则序列为ADC通道转换的顺序，先后之分，[0,17]通道皆可以设置为规则序列
注入序列为插队（类似中断）抢先转换，[1,4]通道才可以设置为注入序列
*/

/*
模拟电压12位
即 3.3V对应2^12
电压= (3.3 * 保存的数据 ) / 2^12		精度为
*/


/*
inti_adc()参数说明
	
		ADC_Mode
		
		ADC_Mode_Independent							独立模式	（不需要ADC同步或者就用一个ADC选择这个）		
		ADC_Mode_RegInjecSimult						混合同步规则+注入同步					
		ADC_Mode_RegSimult_AlterTrig			混合同步规则+交替触发
		ADC_Mode_InjecSimult_FastInterl		混合同步注入+快速交替
		ADC_Mode_InjecSimult_SlowInterl		混合同步规则+慢速交替
		ADC_Mode_InjecSimult							注入同步
		ADC_Mode_RegSimult								规则同步
		ADC_Mode_FastInterl								快速交替
		ADC_Mode_SlowInterl								慢速交替
		ADC_Mode_AlterTrig								触发交替
		
		
		同步注入模式 ADC1 和 ADC2 同时转换一个注入通道组，其中 ADC1 为主，ADC2为从。 转换的数据存储在每个 ADC接口的 ADC_JDRx寄存器中。
		同步规则模式 ADC1 和 ADC2 同时转换一个规则通道组，其中 ADC1 为主，ADC2为从。ADC1转换的结果放在ADC1_DR的低 16位，ADC2转换的结果放在 ADC1_DR 的高十六位。
		快速交叉模式 ADC1 和 ADC2 交替采集一个规则通道组（通常为一个通道）。当ADC2 触发之后，ADC1需要等待 7个 ADCCLK之后才能触发。
		慢速交叉模式 ADC1 和 ADC2 交替采集一个规则通道组（只能为一个通道）。当ADC2 触发之后，ADC1需要等待 14 个 ADCCLK之后才能触发。
		交替触发模式 ADC1和 ADC2轮流采集注入通道组，当 ADC1所有通道采集完毕之后再采集 ADC2 的通道，如此循环。跟交叉采集不一样。
		混合的规则/注入同步模式	规则组同步转换被中断，以启动注入组的同步转换。分开两个模式来理解就可以了，区别就是注入组可以中断规则组的转换。
		混合的同步规则+交替触发模式	规则组同步转换被中断，以启动注入组交替触发转换。分开两个模式来理解就可以了，区别就是注入组可以中断规则组的转换。
		混合同步注入+交叉模式	交叉转换可以被同步注入模式中断。 这种情况下，交叉转换被中断，注入转换被启动。
	
	ADC_ScanConvMode	通道扫描
		
		ENABLE			多通道开启
		DISABLE			单通道关闭
	
	ADC_ContinuousConvMode	
	
	DISABLE			连续转换关闭
	ENABLE			连续转换开启（常见）
	
	ADC_ExternalTrigConv	触发选择			
	
	ADC_ExternalTrigConv_T1_CC1									ADC1、ADC2							定时器1_CC1事件					
	ADC_ExternalTrigConv_T1_CC2									ADC1、ADC2							定时器1_CC2事件	
	ADC_ExternalTrigConv_T2_CC2									ADC1、ADC2							定时器2_CC2事件
	ADC_ExternalTrigConv_T3_TRGO								ADC1、ADC2							定时器3_TRGO事件
	ADC_ExternalTrigConv_T4_CC4									ADC1、ADC2							定时器4_CC4事件
	ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO			ADC1、ADC2、ADC3				外部中断:对于规则通道，选择EXTI线11和TIM8_TRGO作为外部触发事件；而注入通道组则选择EXTI线15和TIM8_CC4作为外部触发事件。
	ADC_ExternalTrigConv_T1_CC3									ADC1、ADC2、ADC3				定时器1_CC3事件
	ADC_ExternalTrigConv_None										ADC1、ADC2、ADC3  			无外部触发源，即用软件触发
	ADC_ExternalTrigConv_T3_CC1									ADC3										定时器3_CC1事件
	ADC_ExternalTrigConv_T2_CC3									ADC3										定时器2_CC3事件
	ADC_ExternalTrigConv_T8_CC1									ADC3										定时器8_CC1事件
	ADC_ExternalTrigConv_T8_TRGO								ADC3										定时器8_TRGO事件
	ADC_ExternalTrigConv_T5_CC1									ADC3										定时器5_CC1事件
	ADC_ExternalTrigConv_T5_CC3									ADC3										定时器5_CC3事件
	
	
	ADC_DataAlign					数据寄存器对齐格式
	
	ADC_DataAlign_Right		右对齐
	ADC_DataAlign_Left		左对齐
	
	ADC_NbrOfChannel			ADC采集通道数目
	ADC_Channel_[0,17]		如：ADC_Channel_0	ADC_Channel_12
	
	RCC_PCLK2				ADC时钟分频(ADCCLK)，ADC为PCLK2的分频。正常PCLK2位72Mhz

	RCC_PCLK2_Div6		12Mhz
	RCC_PCLK2_Div8		9Mhz
	
*/	


/*	inti_channel()参数说明	
	
	ADC_Channel				ADC采集的通道	ADC_Channel_[0,17]		如：ADC_Channel_0	ADC_Channel_12
	
	Rank							转换的顺序[1,16]
	
	ADC_SampleTime		采集时间   采集时间越长精度越高、反之，越短精度也越低
	
	ADC_SampleTime_1Cycles5				1.5个时钟周期
	ADC_SampleTime_7Cycles5				7.5个时钟周期
	ADC_SampleTime_13Cycles5			13.5个时钟周期
	ADC_SampleTime_28Cycles5			28.5个时钟周期
	ADC_SampleTime_41Cycles5			41.5个时钟周期
	ADC_SampleTime_55Cycles5			55.5个时钟周期
	ADC_SampleTime_71Cycles5			71.5个时钟周期
	ADC_SampleTime_239Cycles5			239.5个时钟周期
	
*/


/*
inti_it()参数说明
ADC_IT				中断标志

ADC_IT_EOC		规则组转换结束		
ADC_IT_AWD		模拟看门狗状态位
ADC_IT_JEOC		注入组转换结束

*/


class ADC																			
{
	void inti_adc																//初化完默认开启此通道ADC		如果未用中断则调用ADC_SoftwareStartConvCmd(ADCx, ENABLE)开始adc转换
	(
	uint32_t RCC_APB2Periph,										//ADC通道时钟		RCC_APB2Periph_ADC1		RCC_APB2Periph_ADC2		RCC_APB2Periph_ADC3
	ADC_TypeDef* ADCx,													//此参数可 选 ADC1	ADC2	ADC3
	GPIO *InputPin,															//ADC采集的引脚  初始化为模拟输入（已配置）
	uint32_t ADC_Mode,													//ADC工作模式选择
	FunctionalState ADC_ScanConvMode,						//ADC扫描（多通道）、ADC单次（单通道）
	FunctionalState ADC_ContinuousConvMode,			//ADC单次转换或连续转换
	uint32_t ADC_ExternalTrigConv,							//ADC转换触发信号
	uint32_t ADC_DataAlign,											//数据寄存器对齐格式
	uint8_t  ADC_NbrOfChannel,									//ADC采集通道数目
	uint32_t RCC_PCLK2													//ADC时钟分频
	);
	
	void inti_channel														//初始化规则通道
	(
	ADC_TypeDef* ADCx,													//此参数可 选 ADC1	ADC2	ADC3
	uint8_t ADC_Channel, 												//采集通道
	uint8_t Rank, 															//转换顺序
	uint8_t ADC_SampleTime											//采样时间
	);
	
	void inti_it																//ADC软件中断设置 		已经调用软件触发转换开始，转换完成进入中断
	(
	 ADC_TypeDef* ADCx, 												//此参数可选 ADC1	ADC2	ADC3
	 uint16_t ADC_IT,														//中断标志
   FunctionalState NewState	,									//开启/关闭		ENABLE / DISABLE
	 uint8_t NVIC_IRQChannel ,									//中断源可选		ADC1_2_IRQn	  ADC3_IRQn（启动的汇编文件我未见到此中断，我这块板子不可以）
	 uint8_t NVIC_IRQChannelPreemptionPriority,	//抢占优先级		详细可看EXTI.h中断的配置
	 uint8_t NVIC_IRQChannelSubPriority					//子优先级
	);
	
	void cmd_adc(ADC_TypeDef* ADCx, FunctionalState NewState);  //ADCx(ADC1	ADC2	ADC3)通道开启(ENABLE)或关闭(DISABLE)
	
	void cmd_dma(ADC_TypeDef* ADCx, FunctionalState NewState);	//开启ADCx的DMA功能,请自行配置对应的DMA设置，在dma.h中
	
	
	uint16_t get(ADC_TypeDef* ADCx);   				  //获得ADCx规则序列通道转化后的值
	
};




#endif
