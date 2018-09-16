#ifndef	__dac_h_
#define	__dac_h_
#include "stm32f10x.h"                  // Device header

/*
电压说明
与 ADC 外设类似，DAC 也使用 V REF+ 引脚作为参考电压，在设计原理图的时候一般把
V SSA 接地，把 V REF+ 和 V DDA 接 3.3V，可得到 DAC 的输出电压范围为：0~3.3V。
如果想让输出的电压范围变宽，可以在外部加一个电压调理电路，把 0~3.3V 的 DAC
输出抬升到特定的范围即可。

PA4-通道 1，PA5-通道 2，为避免干扰，使用 DAC功能时，DAC通道引脚需要被配置成模拟输入功能（AIN）

*/


/*
inti()参数说明
		DAC_Channel
		DAC_Channel_1		DAC通道1
		DAC_Channel_2		DAC通道2

		DAC_Trigger						触发事件
		DAC_Trigger_None			无
		DAC_Trigger_T6_TRGO		TIM6更新
		DAC_Trigger_T8_TRGO		TIM8更新	
		DAC_Trigger_T3_TRGO		TIM3更新
		DAC_Trigger_T7_TRGO		TIM7更新
		DAC_Trigger_T5_TRGO		TIM5更新
		DAC_Trigger_T15_TRGO	TIM15更新
		DAC_Trigger_T2_TRGO		TIM2更新
		DAC_Trigger_T4_TRGO		TIM4更新
		DAC_Trigger_Ext_IT9		外部中断	EXTI_Line9外部中断线
		DAC_Trigger_Software	软件触发
		

		DAC_WaveGeneration		是否自动输出噪声或三角波
		DAC_WaveGeneration_None				不生成噪声或是三角波
		DAC_WaveGeneration_Triangle		生成三角波
		DAC_WaveGeneration_Noise			生成噪声
		

		DAC_LFSRUnmask_TriangleAmplitude			选择噪声生成器的低通滤波或三角波的幅值，如果 上面一项为DAC_WaveGeneration_None，则此项无效
		 DAC_LFSRUnmask_Bit0              		伪噪声生成位，越大噪声越大
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
		 DAC_TriangleAmplitude_1           		三角波生成位，越大三角的斜率越大
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
		
		DAC_OutputBuffer				选择是否使能输出缓冲器	
		DAC_OutputBuffer_Enable		使能
		DAC_OutputBuffer_Disable	关闭
		使能了 DAC 的输出缓冲后可以减小输出阻抗，适合直接驱动一些外部负载。
		

*/

/*
uint32_t DAC_Channel
DAC_Channel_1		DAC通道1
DAC_Channel_2		DAC通道2

uint32_t DAC_Align 
DAC_Align_12b_R		12位DAC数据寄存器右对齐
DAC_Align_12b_L		12位DAC数据寄存器左对齐
DAC_Align_8b_R		8位DAC数据寄存器右对齐

uint16_t Data
[0,65535]				DAC数字量

*/

/*
可输出特定波形利用描点法
首先配置Dac（中断源为定时器），再配置指定的定时器（外加TIM_SelectOutputTrigger(TIMx, TIM_TRGOSource_Update)一句选择触发源），定时器溢出后产生对应的电压。
一开始可以配置DMA,源数据地址为&const xx xx[xx],目标地址为	
		DAC->DHR12R1	DAC1通道12位右对齐		
		DAC->DHR12L1	DAC1通道12位左对齐
		DAC->DHR8R1		DAC1通道8位右对齐
		
		DAC->DHR12R2	DAC2通道12位右对齐
		DAC->DHR12L2	DAC2通道12位左对齐
		DAC->DHR8R2		DAC2通道8位右对齐
		
		DAC->DHR12RD	DAC双通道12位右对齐
		DAC->DHR12LD	DAC双通道12位左对齐
		DAC->DHR8RD		DAC双通道8位右对齐

*/




class Dac
{
	public:
	void inti																					//初始化化完成后gpio配置为GPIO_Mode_AIN(未配置),并且使能dma(已经使能)
	(
	uint32_t DAC_Channel,															//DAC通道
	uint32_t DAC_Trigger,															//触发方式
	uint32_t DAC_WaveGeneration,											//是否自动输出噪声或三角波
	uint32_t DAC_LFSRUnmask_TriangleAmplitude,				//选择噪声生成器的低通滤波或三角波的幅值 
	uint32_t DAC_OutputBuffer													//选择是否使能输出缓冲器
	);
	
	void set_data(uint32_t DAC_Channel,uint32_t DAC_Align, uint16_t Data);		//设置输出值
	

};





#endif

