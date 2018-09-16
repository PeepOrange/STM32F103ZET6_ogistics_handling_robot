#ifndef __dma_H_
#define __dma_H_
#include "stm32f10x.h"                  // Device header


/*inti() 参数说明  储存器到存储器模式则将任意一个存储器视为外设（储存器即内存）

初始化完成后请开启对应外设的DMA（在对应的外设库文件中开启）如	
{										
USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
USART_DMACmd 函数用于控制 USART 的 DMA 请求的启动和关闭。它接收三个参
数，第一个参数用于设置串口外设，可以是 USART1/2/3 和 UART4/5 这 5 个参数可选，第
二个参数设置串口的具体 DMA 请求，有串口发送请求 USART_DMAReq_Tx 和接收请求
USART_DMAReq_Rx 可选，第三个参数用于设置启动请求 ENABLE 或者关闭请求
DISABLE。运行该函数后USART的DMA发送传输就开始了，根据配置存储器的数据会发
送到串口。
}
										
 DMAy_Channelx   
 DMA通道						外设（存储器到存储器可任意选择通道）
 DMA1_Channel1      ADC1	TIM2_CH3	TIM4_CH1
 DMA1_Channel2      SPI1_RX		USART3_TX		TIM1_CH1	TIM2_UP		TIM3_CH3	 
 DMA1_Channel3      SPI1_TX		USART3_RX		TIM1_CH2	TIM3_CH4	TIM3_UP 
 DMA1_Channel4      SPI/I2S2_RX		USART1_TX		I2C2_TX		TIM1_TX4	TIM1_TRIG		TIMI_COM	TIM4_CH2
 DMA1_Channel5      SPI/I2S2_TX		USART1_RX		I2C2_RX		TIM1_UP		TIM2_CH1		TIM4_CH3
 DMA1_Channel6      USART2_RX		I2C1_TX		TIM1_CH3		TIM3_CH1		TIM3_TRIG		
 DMA1_Channel7      USART2_TX 	I2C1_RX		TIM2_CH2		TIM2_CH4		TIM4_UP
 
 DMA2_Channel1      SPI/I2S3_RX		TIM5_CH4	TIM5_TRIG		TIM8_CH3		TIM8_UP
 DMA2_Channel2      SPI/I2S3_TX 	TIM5_CH3	TIM5_UP			TIM8_CH4		TIM8_TRIG		TIM8_COM
 DMA2_Channel3      UART4_RX			TIM6_UP/DAC通道1			TIM8_CH1
 DMA2_Channel4      SDIO					TIM5_CH2	TIM7_UP/DAC通道2		
 DMA2_Channel5      UART4_TX 			ADC3			TIM5_CH1		TIM8_CH2
 
 DMA_PeripheralBaseAddr		一般设置为外设的数据寄存器地址，如果是存储器到存储器模式则设置为其中一个存储器地址。
 
 DMA_MemoryBaseAddr				一般设置为我们自定义存储区的首地址。
 
 DMA_DIR
	 DMA_DIR_PeripheralDST	外设作为目的地
	 DMA_DIR_PeripheralSRC	外设作为数据源
 
 DMA_BufferSize
	指定的DMA的缓存大小，单位为 DMA_PeripheralDataSize	或	DMA_PeripheralDataSize
 
 DMA_PeripheralInc
	DMA_PeripheralInc_Enable		外设寄存器地址递增使能
	DMA_PeripheralInc_Disable		外设寄存器地址递增关闭（正常外设寄存器地址不变）
 
 DMA_MemoryInc
	DMA_MemoryInc_Enable		存储器地址递增使能（正常储存器地址递增）
	DMA_MemoryInc_Disable		存储器地址递增关闭
 
 DMA_PeripheralDataSize			外设的数据大小单位
	DMA_PeripheralDataSize_Byte			8位 （一个字节）
	DMA_PeripheralDataSize_HalfWord	16位（二个字节）
	DMA_PeripheralDataSize_Word			32位（四个字节）
 
 DMA_MemoryDataSize					存储器地址大小单位（当与外设数据运输时应该与上面一致）
	 DMA_MemoryDataSize_Byte		    8位 （一个字节）
	 DMA_MemoryDataSize_HalfWord		16位（二个字节）
	 DMA_MemoryDataSize_Word				32位（四个字节）
 
 DMA_Mode				模式选择
	 DMA_Mode_Circular		循环传输
	 DMA_Mode_Normal			一次传输
 
 DMA_Priority			通道优先级
	 DMA_Priority_VeryHigh		非常高
	 DMA_Priority_High				高
	 DMA_Priority_Medium			中等
	 DMA_Priority_Low					低
 
 DMA_M2M				存储器到存储器模式
 DMA_M2M_Enable		存储器到存储器模式使能
 DMA_M2M_Disable	存储器到存储器模式关闭
 
 RCC_AHBPeriph   AHB时钟线开启DMA时钟
 RCC_AHBPeriph_DMA1  DMA通道一
 RCC_AHBPeriph_DMA2	 DMA通道二
 
*/

/*cmd()		初始化完默认开启对应通道
	DMAy_Channelx 为 DMA1_Channel1 到	DMA2_Channel5

	ENABLE	 			开启
	DISABLE			  关闭
*/

/*flag()  返回RESET或SET
	DMAy_FLAG_GLx   DMAyd的通道x的全局中断标志                  
	DMAy_FLAG_TCx   DMAyd的通道x的传输完成标志                   
	DMAy_FLAG_HTx   DMAyd的通道x的半传输标志                  
	DMAy_FLAG_TEx   DMAyd的通道x的传输错误标志                   
*/

//(u32)(&( ADCx->DR ))  ADCx的数据寄存器地址



class DMA
{
	public:
	void inti
	 (DMA_Channel_TypeDef* DMAy_Channelx,    //DMA通道
		uint32_t DMA_PeripheralBaseAddr,			 //外设地址
		uint32_t DMA_MemoryBaseAddr,					 //储存器地址
		uint32_t DMA_DIR,											 //传输方向
		uint32_t DMA_BufferSize,						   //传输数目
		uint32_t DMA_PeripheralInc,						 //外设地址增量
		uint32_t DMA_MemoryInc,								 //存储器地址增量
		uint32_t DMA_PeripheralDataSize,			 //外设数据宽度
		uint32_t DMA_MemoryDataSize,					 //存储器数据宽度
		uint32_t DMA_Mode,										 //模式选择
		uint32_t DMA_Priority,								 //通道优先级
		uint32_t DMA_M2M,											 //存储器到储存器模式
		uint32_t RCC_AHBPeriph								 //AHB时钟线开启DMA时钟
	 );
	void cmd(DMA_Channel_TypeDef* DMAy_Channelx,FunctionalState NewState);  //开启或关闭DMA通道(初始化后默认开启)
	
	FlagStatus flag(uint32_t DMAy_FLAG);
	
};



#endif

