#ifndef __iic_ee_H
#define __iic_ee_H
#include "stm32f10x.h"                  // Device header
#include "gpio.h"


//硬件实现iic通信，利用stm32iic外设
#define TimeOut 0xa000      //通信超时时间


/*
I2C_InitTypeDef
  // I2C 配置 
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
  //高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7;      //这个地址只要与STM32外挂的I2C器件地址不一样即可 

  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
  // I2C的寻址模式 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
  // 通信速率 
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; //I2C_Speed=400000  快速模式

*/



class IIC_EE
{
    public:
    IIC_EE(I2C_TypeDef* I2Cx);
    void Init(I2C_InitTypeDef* I2C_InitStruct,GPIO* scl, GPIO* sda);
    FunctionalState Write(uint8_t* p_Data,uint16_t Length,uint8_t ID_7Bit,uint8_t WriteAddr );
    FunctionalState Read (uint8_t* p_Data,uint16_t Length,uint8_t ID_7Bit,uint8_t ReadAddr );
    
    
    private:
    I2C_TypeDef *  I2Cx;
    
};








#endif
