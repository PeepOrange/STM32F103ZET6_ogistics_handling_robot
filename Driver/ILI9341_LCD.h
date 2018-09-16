#ifndef __ILI9341_LCD_H
#define __ILI9341_LCD_H
#include "stm32f10x.h"                  // Device header
#include "gpio.h"


//����FSMCģ��8080ʱ�����ILI9341_LCD(RGB565)   100������STM32��FSMC����

#define 			ILI9341_LESS_PIXEL	  						240								//Һ�����϶̷�������ؿ��
#define 			ILI9341_MORE_PIXEL	 						320								//Һ�����ϳ���������ؿ��



struct GPIO_ILI9341_Lcd
{
  GPIO*    ILI9341_CS;       //Ƭѡ
  GPIO*    ILI9341_DC;       //DC���ţ�ʹ��FSMC�ĵ�ַ�źſ��ƣ������ž����˷���LCDʱʹ�õĵ�ַ
  GPIO*    ILI9341_WR;       //дʹ��
  GPIO*    ILI9341_RD;       //��ʹ��
  GPIO*    ILI9341_RST;      //��λ
  GPIO*    ILI9341_BK;       //����
    
  //����Ϊ����λ
  GPIO*    ILI9341_D0;
  GPIO*    ILI9341_D1;
  GPIO*    ILI9341_D2;
  GPIO*    ILI9341_D3;
  GPIO*    ILI9341_D4;
  GPIO*    ILI9341_D5;
  GPIO*    ILI9341_D6;
  GPIO*    ILI9341_D7;
  GPIO*    ILI9341_D8;
  GPIO*    ILI9341_D9;
  GPIO*    ILI9341_D10;
  GPIO*    ILI9341_D11;
  GPIO*    ILI9341_D12;
  GPIO*    ILI9341_D13;
  GPIO*    ILI9341_D14;
  GPIO*    ILI9341_D15;  
};

/*
TextColor��BackColor ˵��

RGB565,������5+6+5=16 λ ��2���ֽ�

0xFFFF	   //��ɫ
0x0000	   //��ɫ 
0xF7DE	   //��ɫ 
0x001F	   //��ɫ 
0x051F	   //ǳ��ɫ 
0xF800	   //��ɫ 
0xF81F	   //����ɫ�����ɫ 
0x07E0	   //��ɫ 
0x7FFF	   //����ɫ����ɫ 
0xFFE0	   //��ɫ 



*/
class ILI9341_Lcd
{
	ILI9341_Lcd(GPIO_ILI9341_Lcd * GPIO_Config);		//�ýṹ��Ϊȫ�ֱ���
    
public:
	void BackLed(FunctionalState enumState);        //����ƿ��� , ע���ҵİ����ǵ͵�ƽΪ������ENABLE
	void GramScan_Mode(uint8_t mode);		//GramScanɨ��ģʽ
	void Init();									//��ʼ��,��ʼ����Ϻ�Ҫ����ILI9341_GramScan_Mode()ѡ��ɨ��ģʽ
	void Reset();									 //��λ
    void DispChar ( uint16_t X, uint16_t Y, const char cChar ,uint16_t  TextColor,uint16_t BackColor);  //��ʾһ��Ascll�ַ�,������ɫ�ͱ�����ɫ
	void DispStringLine ( uint16_t line,  char * pStr,uint16_t  TextColor,uint16_t BackColor );             //��ʾAscll�ַ���
    void Fill(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height,uint16_t Color);                   //�������ĳ����ɫ(����������)
   static void FillPixel_Mode(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height);                      //�����������ģʽ
   static void WritePixel(uint16_t RGB565);                                                                  //����ɨ��ģʽ������淽ʽ��䣬һ����� Width*Height����

private:
    GPIO_ILI9341_Lcd  * ILI9341_Lcd_GPIO;		
	uint16_t LCD_X_LENGTH ;
	uint16_t LCD_Y_LENGTH ;
    
   static  inline  void ILI9341_Write_Cmd(uint16_t Cmd);				//д����
   static	inline  void ILI9341_Write_Data(uint16_t Data);		    //д����
	inline  uint16_t ILI9341_Read();							//������
	void   REG_Config();									    //�Ĵ�����ʼ��
	static void   OpenWindow(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height);  //��LCD�Ͽ���һ������
    void   Init_Gpio();                                         //��ʼ������
    void   Init_FSMC();                                         //����FSMC
};







#endif


