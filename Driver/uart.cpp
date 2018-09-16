#include "uart.h"
#include "stdio.h"

 UART::UART(GPIO *RX,GPIO *TX)
{
	this->RX=RX;
	this->TX=TX;
}

void UART::inti_mode(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	TX->mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
	RX->mode(GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
	USART_InitTypeDef usart1_mode;
	usart1_mode.USART_BaudRate=USART_BaudRate;
	usart1_mode.USART_WordLength=USART_WordLength;
	usart1_mode.USART_StopBits=USART_StopBits;
	usart1_mode.USART_Parity=USART_Parity;
	usart1_mode.USART_Mode=USART_Mode;
	usart1_mode.USART_HardwareFlowControl=USART_HardwareFlowControl;
	USART_Init(USART1,&usart1_mode);
	USART_Cmd(USART1, ENABLE);
	
}

void UART::sent_byte(uint8_t ch)
{
	
	USART_SendData(USART1,ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void UART::sent_str(const char *str)
{
	unsigned int num=0;
	do
	{
	UART::sent_byte(*(str+num));
	num++;
	}
	while((*(str+num))!='\0');
	while (USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}


void UART::inti_it_turn( uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd,uint16_t USART_IT)
{
	NVIC_InitTypeDef NVIC_mode;
	NVIC_mode.NVIC_IRQChannel=USART1_IRQn;
	NVIC_mode.NVIC_IRQChannelCmd=NVIC_IRQChannelCmd;
	NVIC_mode.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	NVIC_mode.NVIC_IRQChannelSubPriority=NVIC_IRQChannelSubPriority;
	NVIC_Init(&NVIC_mode);
	USART_ITConfig(USART1,USART_IT, NVIC_IRQChannelCmd);
	
}


uint16_t UART::receive_data()
{
	
	return(USART_ReceiveData(USART1));
	
}


 #ifdef __cplusplus
extern "C"
{
	
	
	int std::fputc(int ch, FILE *f)
{
USART_SendData(USART1, (uint8_t) ch);
while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
return (ch);
}


int std::fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	return (int)USART_ReceiveData(USART1);
	
}


};
#endif
