#ifndef __iic_cs_H
#define __iic_cs_H

//ģ��ʵ��iicͨ��
#include "gpio.h"

/*
IIC_Byte  Ϊд�������
slave_adressΪ�ӻ���ַ(7λ) ����mpu6050�ӻ���ַλ0x68��7λ�������Ϊ8λ�������д������0xd0�����λΪ0����������0xd1(���λΪ1)���˴�ֻҪд7λ�ĵ�ַ����
adressΪ�Ĵ����ĵ�ַ��8λ��


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
