#ifndef __iic_cs_H
#define __iic_cs_H

//模拟实现iic通信
#include "gpio.h"

/*
IIC_Byte  为写入的数据
slave_adress为从机地址(7位) 比如mpu6050从机地址位0x68（7位）左对齐为8位，如果是写入则是0xd0（最低位为0），读则是0xd1(最低位为1)，此处只要写7位的地址即可
adress为寄存器的地址（8位）


*/


class IIC_CS
{
	public:
	IIC_CS(GPIO *sclPin,GPIO *sdaPin);
    void     Init_Gpio();
	FunctionalState write(unsigned char IIC_Byte,unsigned char slave_adress,unsigned char adress); 
	FunctionalState read(uint8_t* data,uint16_t length,unsigned char slave_adress,unsigned char adress);
	
	protected:
	GPIO *sclPin;
	GPIO *sdaPin;		
	
	private:	
	FunctionalState Start();
	void Stop();
    void Ack();
    void NoAck();
	FunctionalState WaitAck();
    void Write_Byte(unsigned char IIC_Byte); 
	uint8_t Read_Byte();
	
};


#endif
