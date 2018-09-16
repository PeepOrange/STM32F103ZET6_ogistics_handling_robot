#include "oled.h"
#include "oled_font.h"
#include "stdarg.h"
#include "stdio.h"

OLED::OLED(OLED_GPIO *Gpio):IIC_CS(Gpio->SCL,Gpio->SDA) 
{
		this->ID_Adress=Gpio->ID_Adress;		
}	


void OLED::Write_Command(unsigned char IIC_Command)
{
	
	   OLED::write(IIC_Command,ID_Adress,0x00);  //写命令
}

void OLED::Write_Date(unsigned char IIC_Date)
{

	   OLED::write(IIC_Date,ID_Adress,0x40);			//写数据
}


void OLED::Inti()
{
//  this->sclPin->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
//	this->sdaPin->mode(GPIO_Mode_Out_OD,GPIO_Speed_50MHz);
		OLED::Write_Command(0xAE);//--display off
		OLED::Write_Command(0x00);//---set low column address
		OLED::Write_Command(0x10);//---set high column address
		OLED::Write_Command(0x40);//--set start line address  
		OLED::Write_Command(0xB0);//--set page address
		OLED::Write_Command(0x81); // contract control
		OLED::Write_Command(0xFF);//--128   
		OLED::Write_Command(0xA1);//set segment remap 
		OLED::Write_Command(0xA6);//--normal / reverse
		OLED::Write_Command(0xA8);//--set multiplex ratio(1 to 64)
		OLED::Write_Command(0x3F);//--1/32 duty
		OLED::Write_Command(0xC8);//Com scan direction
		OLED::Write_Command(0xD3);//-set display offset
		OLED::Write_Command(0x00);//
		OLED::Write_Command(0xD5);//set osc division
		OLED::Write_Command(0x80);//
		OLED::Write_Command(0xD8);//set area color mode off
		OLED::Write_Command(0x05);//
		OLED::Write_Command(0xD9);//Set Pre-Charge Period
		OLED::Write_Command(0xF1);//
		OLED::Write_Command(0xDA);//set com pin configuartion
		OLED::Write_Command(0x12);//
		OLED::Write_Command(0xDB);//set Vcomh
		OLED::Write_Command(0x30);//
		OLED::Write_Command(0x8D);//set charge pump enable
		OLED::Write_Command(0x14);//
		OLED::Write_Command(0xAF);//--turn on oled panel	
		OLED::Black();		
}
//设置打印起点
void OLED::SetPos(unsigned char x, unsigned char y) 
{ OLED::Write_Command(0xb0+y);
	OLED::Write_Command(((x&0xf0)>>4)|0x10);
	OLED::Write_Command((x&0x0f)); 
}   	  
//开启OLED显示    
void OLED::DisplayOn(void)
{
	OLED::Write_Command(0X8D);  //SET DCDC命令
	OLED::Write_Command(0X14);  //DCDC ON
	OLED::Write_Command(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED::DisplayOff(void)
{
	OLED::Write_Command(0X8D);  //SET DCDC命令
	OLED::Write_Command(0X10);  //DCDC OFF
	OLED::Write_Command(0XAE);  //DISPLAY OFF
}	


void OLED::Black(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED::Write_Command (0xb0+i);    //设置页地址（0~7）
		OLED::Write_Command (0x00);      //设置显示位置—列低地址
		OLED::Write_Command (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		OLED::Write_Date(0); 
	} 
}

void OLED::Printf(unsigned char  x, unsigned char  y,const char *fmt, ...)
{
    va_list args;
    uint8_t length;
    uint8_t i,j,c;
    static char log_buf[128];
    va_start(args, fmt);
    length = vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, args);
    if (length > 128) length = 128;
    for(i = 0; i < length; i ++)
    {
        c = log_buf[i] - 32;
        if( x > 120 )
        {
            x = 0;
            y++;
        }
        SetPos(x, y);
        for(j = 0; j < 6; j++)
        {
					OLED::Write_Date(ascii[c][j]);
        }
        x += 6;
        j++;
    }
    va_end(args);
}


void OLED::ShowChinese(u8 x,u8 y,u8 font_num)
{      			    
	u8 t,adder=0;
	OLED::SetPos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED::Write_Date(Chinese_font[2*font_num][t]);
				adder+=1;
     }	
		OLED::SetPos(x,y+1);	
    for(t=0;t<16;t++)
		{	
				OLED::Write_Date(Chinese_font[2*font_num+1][t]);
				adder+=1;
    }					
}


void OLED::DrawBMP(unsigned char x0, unsigned char y0,unsigned char weight, unsigned char height)
{ 	
	u8 t,adder;
	OLED::SetPos(x0,y0);	
  for(adder=0;adder<height;adder++)  
	{ 
		for(t=0;t<weight;t++)		
	  OLED::Write_Date(BMP[adder][t]);		
    SetPos(x0,y0+adder);			
	}  		
} 


