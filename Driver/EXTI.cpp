
#include "EXTI.h"


void EXTI_GPIO::inti_pin(GPIO *exti_gpio)
{
	exti_gpio->mode(GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);//GPIO_Mode_IPU
}


void EXTI_GPIO::inti_souce(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
	GPIO_EXTILineConfig(GPIO_PortSource,GPIO_PinSource);
}


void EXTI_GPIO::inti_mode(uint32_t EXTI_Line , EXTIMode_TypeDef EXTI_Mode , EXTITrigger_TypeDef EXTI_Trigger , FunctionalState EXTI_LineCmd)
{
	EXTI_InitTypeDef EXTI_gpio;
	EXTI_gpio.EXTI_Line=EXTI_Line;
	EXTI_gpio.EXTI_Mode=EXTI_Mode;
	EXTI_gpio.EXTI_Trigger=EXTI_Trigger;
	EXTI_gpio.EXTI_LineCmd=EXTI_LineCmd;
	EXTI_Init(&EXTI_gpio);
}


void EXTI_GPIO::inti_turn(uint8_t NVIC_IRQChannel , uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd)
{
	NVIC_InitTypeDef NVIC_mode;
	NVIC_mode.NVIC_IRQChannel=NVIC_IRQChannel;
	NVIC_mode.NVIC_IRQChannelCmd=NVIC_IRQChannelCmd;
	NVIC_mode.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	NVIC_mode.NVIC_IRQChannelSubPriority=NVIC_IRQChannelSubPriority;
	NVIC_Init(&NVIC_mode);
}
