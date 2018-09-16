#include "gpio.h"
 

GPIO::GPIO(GPIO_TypeDef *port,uint16_t pin)
{	
	this->pin=pin;
	this->port=port;	
}


void GPIO::mode(GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=mode;
	gpio.GPIO_Speed=speed;
	gpio.GPIO_Pin=pin;
	GPIO_Init(port,&gpio);
}

void GPIO::set()
{
	GPIO_SetBits(port,pin);
}

void GPIO::reset()
{
	GPIO_ResetBits(port,pin);
}

void GPIO::toggle()
{
	port->ODR ^= pin;	
}

void GPIO::Write(uint16_t val)
{
	GPIO_Write(port,val);	
}

uint8_t GPIO::out_read()
{
	return
	GPIO_ReadOutputDataBit( port,pin );
}

uint8_t GPIO::in_read()
{
	return
	GPIO_ReadInputDataBit( port,pin );
}



