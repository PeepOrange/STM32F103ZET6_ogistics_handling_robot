#include "dma.h"


void DMA::inti
 (
	DMA_Channel_TypeDef* DMAy_Channelx,
	uint32_t DMA_PeripheralBaseAddr,
	uint32_t DMA_MemoryBaseAddr,
	uint32_t DMA_DIR,
	uint32_t DMA_BufferSize,
	uint32_t DMA_PeripheralInc,
	uint32_t DMA_MemoryInc,
	uint32_t DMA_PeripheralDataSize,
	uint32_t DMA_MemoryDataSize,
	uint32_t DMA_Mode,
	uint32_t DMA_Priority,
	uint32_t DMA_M2M,
	uint32_t RCC_AHBPeriph
	)
							
	{
		DMA_InitTypeDef DMA_mode;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph,ENABLE);
		DMA_DeInit(DMAy_Channelx);
		DMA_mode.DMA_PeripheralBaseAddr=DMA_PeripheralBaseAddr;
		DMA_mode.DMA_MemoryBaseAddr=DMA_MemoryBaseAddr;
		DMA_mode.DMA_DIR=DMA_DIR;
		DMA_mode.DMA_BufferSize=DMA_BufferSize;
		DMA_mode.DMA_PeripheralInc=DMA_PeripheralInc;
		DMA_mode.DMA_MemoryInc=DMA_MemoryInc;
		DMA_mode.DMA_PeripheralDataSize=DMA_PeripheralDataSize;
		DMA_mode.DMA_MemoryDataSize=DMA_MemoryDataSize;
		DMA_mode.DMA_Mode=DMA_Mode;
		DMA_mode.DMA_Priority=DMA_Priority;
		DMA_mode.DMA_M2M=DMA_M2M;
		
		DMA_Init(DMAy_Channelx,&DMA_mode);
		DMA_Cmd(DMAy_Channelx,ENABLE);
	}
	
	void DMA::cmd(DMA_Channel_TypeDef* DMAy_Channelx,FunctionalState NewState)
	{
		DMA_Cmd(DMAy_Channelx,NewState);
	}
	FlagStatus DMA::flag(uint32_t DMAy_FLAG)
	{
		return(DMA_GetFlagStatus( DMAy_FLAG));	
	}
	


