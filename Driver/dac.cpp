#include	"dac.h"




void Dac::inti
	(
	uint32_t DAC_Channel,
	uint32_t DAC_Trigger,
	uint32_t DAC_WaveGeneration,
	uint32_t DAC_LFSRUnmask_TriangleAmplitude,
	uint32_t DAC_OutputBuffer
	)
{
	DAC_InitTypeDef DAC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	
	DAC_InitStructure.DAC_Trigger=DAC_Trigger;
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_TriangleAmplitude;
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer;
	
	DAC_Init(DAC_Channel, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel, ENABLE);
	DAC_DMACmd(DAC_Channel, ENABLE);
}


void Dac::set_data(uint32_t DAC_Channel,uint32_t DAC_Align, uint16_t Data)
{
	if(DAC_Channel)
		DAC_SetChannel2Data(DAC_Align,Data);
	else
		DAC_SetChannel1Data(DAC_Align,Data);
	DAC_SoftwareTriggerCmd(DAC_Channel, ENABLE);
}

