#include	"spi.h"

void SPI::inti
	(
	SPI_TypeDef* SPIx,
	uint16_t SPI_Direction,
	uint16_t SPI_Mode,
	uint16_t SPI_DataSize,
	uint16_t SPI_CPOL,
	uint16_t SPI_CPHA,
	uint16_t SPI_NSS,
	uint16_t SPI_BaudRatePrescaler,
	uint16_t SPI_FirstBit,
	uint16_t SPI_CRCPolynomial
	)
{
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Direction=SPI_Direction;
	SPI_InitStruct.SPI_Mode=SPI_Mode;
	SPI_InitStruct.SPI_DataSize=SPI_DataSize;
	SPI_InitStruct.SPI_CPOL=SPI_CPOL;
	SPI_InitStruct.SPI_CPHA=SPI_CPHA;
	SPI_InitStruct.SPI_NSS=SPI_NSS;
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler;
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit;
	SPI_InitStruct.SPI_CRCPolynomial=SPI_CRCPolynomial;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_Init(SPIx,&SPI_InitStruct);
	SPI_Cmd(SPIx, ENABLE);
}

FunctionalState SPI::send_16b(SPI_TypeDef* SPIx,uint16_t data,uint16_t* reply)
{
    uint8_t SPITimeout =SPI_timeout;
    if(!SPI::send_16b( SPIx,data))  return DISABLE;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET)
	 {
    if((SPITimeout--) == 0) return DISABLE;
   }
  /* 读取数据寄存器，获取接收缓冲区数据 */
   *reply=SPI_I2S_ReceiveData(SPIx);    
    return ENABLE;    
}


FunctionalState	SPI::send_8b(SPI_TypeDef* SPIx,uint8_t data,uint8_t* reply)
{
    uint8_t SPITimeout =SPI_timeout;
    if(!SPI::send_8b( SPIx,data))  return DISABLE;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return DISABLE;
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  *reply=SPI_I2S_ReceiveData(SPIx );   
   
   return ENABLE;
}


FunctionalState	SPI::send_8b(SPI_TypeDef* SPIx,uint8_t data)
{
     uint8_t SPITimeout =SPI_timeout;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return DISABLE;
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPIx , data);   
    return ENABLE;    
}


FunctionalState SPI::send_16b(SPI_TypeDef* SPIx,uint16_t data)
{
    uint8_t SPITimeout =SPI_timeout;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) return DISABLE;
   }
	
  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPIx , data);       
    return ENABLE;   
}








