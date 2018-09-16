#include "iic_ee.h"


IIC_EE::IIC_EE(I2C_TypeDef* I2Cx)
{
    this->I2Cx=I2Cx;
}



void IIC_EE::Init(I2C_InitTypeDef* I2C_InitStruct,GPIO *scl,GPIO *sda)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    scl->mode(GPIO_Mode_AF_OD,GPIO_Speed_50MHz);
    sda->mode(GPIO_Mode_AF_OD,GPIO_Speed_50MHz);
    I2C_Init( I2Cx, I2C_InitStruct);
    I2C_Cmd(I2Cx,  ENABLE);
}
    



FunctionalState IIC_EE::Write(uint8_t* p_Data,uint16_t Length,uint8_t ID_7Bit,uint8_t WriteAddr)
{
  uint16_t time_out=TimeOut;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))   
  {
    if((time_out--) == 0) return DISABLE;
  } 
  I2C_GenerateSTART(I2Cx, ENABLE);
  time_out = TimeOut;

  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
  {
    if((time_out--) == 0) return DISABLE;
  }     

  I2C_Send7bitAddress(I2Cx, ID_7Bit, I2C_Direction_Transmitter);
  
  time_out=TimeOut;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))  
  {
    if((time_out--) == 0) return DISABLE;
  }   
    
  I2C_SendData(I2Cx, WriteAddr);  
  
  time_out=TimeOut;
  while(! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((time_out--) == 0) return DISABLE;
  } 
  
  while(Length--)  
  {
    I2C_SendData(I2Cx, *p_Data); 

    p_Data++; 
  
    time_out = TimeOut;

    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
      if((time_out--) == 0) return DISABLE;
    } 
  }  
  
  I2C_GenerateSTOP(I2Cx, ENABLE);
  
  return ENABLE;
}


FunctionalState IIC_EE::Read(uint8_t* p_Data,uint16_t Length,uint8_t ID_7Bit,uint8_t ReadAddr )
{

  uint16_t time_out=TimeOut;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))   
  {
    if((time_out--) == 0) return DISABLE;
  } 
  I2C_GenerateSTART(I2Cx, ENABLE);    

  time_out=TimeOut;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((time_out--) == 0) return DISABLE;
  }
    
  I2C_Send7bitAddress(I2Cx, ID_7Bit, I2C_Direction_Transmitter);

  time_out=TimeOut;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((time_out--) == 0) return DISABLE;
  }
  
  I2C_Cmd(I2Cx, ENABLE);       //重新置位     
  
  I2C_SendData(I2Cx, ReadAddr);  
  
  time_out=TimeOut;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((time_out--) == 0) return DISABLE;
  }
  
  I2C_GenerateSTART(I2Cx, ENABLE);      //发送完成ID重新开始
  
  time_out=TimeOut;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((time_out--) == 0) return DISABLE;
  }  
  
  I2C_Send7bitAddress(I2Cx, ReadAddr, I2C_Direction_Receiver);
  
  time_out=TimeOut;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if((time_out--) == 0) return DISABLE;
   }
  
  while(Length)  
  {
    if(Length == 1)
    {
      I2C_AcknowledgeConfig(I2Cx, DISABLE);      
      I2C_GenerateSTOP(I2Cx, ENABLE);
    }

    time_out=TimeOut;
    
	while(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((time_out--) == 0) return DISABLE;
		} 
    {      
      *p_Data = I2C_ReceiveData(I2Cx);
      p_Data++; 
      Length--;        
    }   
  }
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  return ENABLE;  
}












