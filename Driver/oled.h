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



//ID_Adress�ӻ���ַ(7λ) ����mpu6050�ӻ���ַλ0x68��7λ���Ҷ���Ϊ8λ�������д������0xd0�����λΪ0����������0xd1(���λΪ1)���˴�ֻҪд7λ�ĵ�ַ����

class OLED : public IIC_CS
{
	public:
	OLED(OLED_GPIO *Gpio);								 //OLED�Ĵӻ���ַ
    void Inti();                                     //��ʼ��OLED
	void DisplayOn();                                //����OLED��ʾ    
	void DisplayOff();                               //�ر�OLED��ʾ  
	void Black();                                    //����OLED(�����ڹ�������������)
	void Printf(unsigned char  x, unsigned char  y,const char *fmt, ...);  // x[0,127]  y[0,7]   ��(x,y)���ӡһ��ascii�ַ���(��СΪ6*8)
	void ShowChinese(unsigned char x,unsigned char y,unsigned char font_num);    //ͬ�Ϸ�Χ ��(x,y)��ӡһ������ 16*16   font_numΪ oled_font.h �ļ���Chinese_font[][16]��������        
  void DrawBMP(unsigned char x0, unsigned char y0,unsigned char weight, unsigned char height); 	//��ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7,���ؼ���ͼ�� weight(��)��height(��)����һ��ʹ����Ҫ�����ϵ�(OLED���м���)

	private:
	uint8_t ID_Adress;
	void SetPos(unsigned char x, unsigned char y) ;   
	void Write_Command(unsigned char IIC_Command);
	void Write_Date(unsigned char IIC_Date);
};





#endif
