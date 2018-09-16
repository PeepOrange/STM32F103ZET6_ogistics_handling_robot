#ifndef __oled_H
#define __oled_H

#include "iic_cs.h"
#include "stdint.h"
#include "gpio.h"




struct OLED_GPIO
{
    uint8_t ID_Adress;      //0x3c
    GPIO    *SCL;
    GPIO    *SDA;
};



//ID_Adress从机地址(7位) 比如mpu6050从机地址位0x68（7位）右对齐为8位，如果是写入则是0xd0（最低位为0），读则是0xd1(最低位为1)，此处只要写7位的地址即可

class OLED : public IIC_CS
{
	public:
	OLED(OLED_GPIO *Gpio);								 //OLED的从机地址
    void Inti();                                     //初始化OLED
	void DisplayOn();                                //开启OLED显示    
	void DisplayOff();                               //关闭OLED显示  
	void Black();                                    //黑屏OLED(依旧在工作，用作清屏)
	void Printf(unsigned char  x, unsigned char  y,const char *fmt, ...);  // x[0,127]  y[0,7]   从(x,y)点打印一段ascii字符串(大小为6*8)
	void ShowChinese(unsigned char x,unsigned char y,unsigned char font_num);    //同上范围 在(x,y)打印一个汉字 16*16   font_num为 oled_font.h 文件中Chinese_font[][16]的数组码        
  void DrawBMP(unsigned char x0, unsigned char y0,unsigned char weight, unsigned char height); 	//显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7,像素级别图像 weight(宽)，height(高)，第一次使用需要重新上电(OLED具有记忆)

	private:
	uint8_t ID_Adress;
	void SetPos(unsigned char x, unsigned char y) ;   
	void Write_Command(unsigned char IIC_Command);
	void Write_Date(unsigned char IIC_Date);
};





#endif
