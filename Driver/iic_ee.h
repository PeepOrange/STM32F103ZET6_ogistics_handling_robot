#ifndef __iic_ee_H
#define __iic_ee_H
#include "stm32f10x.h"                  // Device header
#include "gpio.h"


//Ӳ��ʵ��iicͨ�ţ�����stm32iic����
#define TimeOut 0xa000      //ͨ�ų�ʱʱ��


/*
I2C_InitTypeDef
  // I2C ���� 
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
  //�ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� 
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7;      //�����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ 

  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
  // I2C��Ѱַģʽ 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
  // ͨ������ 
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; //I2C_Speed=400000  ����ģʽ

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
