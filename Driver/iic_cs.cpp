#include "iic_cs.h"

IIC_CS::IIC_CS(GPIO *sclPin,GPIO *sdaPin)
{
    this->sclPin=sclPin;
    this->sdaPin=sdaPin;	  
}



void    IIC_CS::Init_Gpio()
{
    this->sclPin->mode(GPIO_Mode_Out_OD,GPIO_Speed_50MHz);
    this->sdaPin->mode(GPIO_Mode_Out_OD,GPIO_Speed_50MHz);   
}

FunctionalState IIC_CS::Start()
{
	this->sclPin->set();
	this->sdaPin->set();
    if(!this->sdaPin->out_read())
        return DISABLE;
    this->sdaPin->reset();
    if(this->sdaPin->out_read())
        return DISABLE;    
	this->sdaPin->reset();	
    return ENABLE;
}

void IIC_CS::Stop()
{
	this->sclPin->set();
	this->sdaPin->reset();
	this->sdaPin->set();
	
}


void IIC_CS::Ack()
{    
	this->sclPin->reset();
	this->sdaPin->reset();    
    this->sclPin->set();
    this->sclPin->reset();
}


void IIC_CS::NoAck()
{    
	this->sclPin->reset();
	this->sdaPin->set();    
    this->sclPin->set();
    this->sclPin->reset();
}



FunctionalState IIC_CS::WaitAck()
{
	this->sclPin->reset();
    this->sdaPin->set();
    this->sclPin->set();
    if(this->sdaPin->out_read())
    {
        this->sclPin->reset();
        return DISABLE;       
    }
    else
    {
        this->sclPin->reset();
        return ENABLE;  
    }
}

void IIC_CS::Write_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	this->sclPin->reset();
	for(i=0;i<8;i++)		
	{
	if((0x80>>i)&(IIC_Byte))
		this->sdaPin->set();
	else 
		this->sdaPin->reset();		
	this->sclPin->set();
	this->sclPin->reset();
	}
}



uint8_t IIC_CS::Read_Byte()
{
	unsigned char i;
	unsigned char IIC_Byte=0;
	this->sclPin->reset();
	for(i=0;i<8;i++)
	{
		
		if(sdaPin->out_read())
		//IIC_Byte|=(1<<i);
		IIC_Byte|=(0x80>>i);
		this->sclPin->set();
		this->sclPin->reset();
	}	
	
	return IIC_Byte;
}


FunctionalState IIC_CS::write(unsigned char IIC_Byte,unsigned char slave_adress,unsigned char adress)
{
	if(!IIC_CS::Start())
        return DISABLE;
	IIC_CS::Write_Byte((slave_adress)<<1);
    if(!IIC_CS::WaitAck())
    {
        IIC_CS::Stop();
        return DISABLE;
    }
	IIC_CS::Write_Byte(adress);
    if(!IIC_CS::WaitAck())
    {
        IIC_CS::Stop();
        return DISABLE;
    }
	IIC_CS::Write_Byte(IIC_Byte);
    if(!IIC_CS::WaitAck())
    {
        IIC_CS::Stop();
        return DISABLE;
    }
	IIC_CS::Stop();
    return ENABLE;
}

FunctionalState IIC_CS::read(uint8_t* data,uint16_t length,unsigned char slave_adress,unsigned char adress)
{	
	if(!IIC_CS::Start())
        return DISABLE;
	IIC_CS::Write_Byte((slave_adress)<<1);
    if(!IIC_CS::WaitAck())
    {
        IIC_CS::Stop();
        return DISABLE;
    }    
    IIC_CS::Write_Byte(adress);
    if(!IIC_CS::WaitAck())
    {
        IIC_CS::Stop();
        return DISABLE;
    }  
    IIC_CS::Stop();
    
	if(!IIC_CS::Start())
        return DISABLE;
	IIC_CS::Write_Byte((slave_adress)<<1|1);
    if(!IIC_CS::WaitAck())
    {
        IIC_CS::Stop();
        return DISABLE;
    }     
    while(length)
    {
    * data =IIC_CS::Read_Byte();
    if(length!=1)       IIC_CS::Ack();
    else                IIC_CS::NoAck();   
    data++;
    length--;
    }
    IIC_CS::Stop();
    return ENABLE;
}



