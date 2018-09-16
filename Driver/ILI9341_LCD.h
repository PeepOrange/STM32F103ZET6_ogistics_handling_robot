#ifndef __ILI9341_LCD_H
#define __ILI9341_LCD_H
#include "stm32f10x.h"                  // Device header
#include "gpio.h"


//利用FSMC模拟8080时序控制ILI9341_LCD(RGB565)   100脚以上STM32有FSMC外设

#define 			ILI9341_LESS_PIXEL	  						240								//液晶屏较短方向的像素宽度
#define 			ILI9341_MORE_PIXEL	 						320								//液晶屏较长方向的像素宽度



struct GPIO_ILI9341_Lcd
{
  GPIO*    ILI9341_CS;       //片选
  GPIO*    ILI9341_DC;       //DC引脚，使用FSMC的地址信号控制，本引脚决定了访问LCD时使用的地址
  GPIO*    ILI9341_WR;       //写使能
  GPIO*    ILI9341_RD;       //读使能
  GPIO*    ILI9341_RST;      //复位
  GPIO*    ILI9341_BK;       //背光
    
  //以下为数据位
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
TextColor、BackColor 说明

RGB565,所以是5+6+5=16 位 ，2个字节

0xFFFF	   //白色
0x0000	   //黑色 
0xF7DE	   //灰色 
0x001F	   //蓝色 
0x051F	   //浅蓝色 
0xF800	   //红色 
0xF81F	   //红紫色，洋红色 
0x07E0	   //绿色 
0x7FFF	   //蓝绿色，青色 
0xFFE0	   //黄色 



*/
class ILI9341_Lcd
{
	ILI9341_Lcd(GPIO_ILI9341_Lcd * GPIO_Config);		//该结构体为全局变量
    
public:
	void BackLed(FunctionalState enumState);        //背光灯控制 , 注意我的板子是低电平为亮，即ENABLE
	void GramScan_Mode(uint8_t mode);		//GramScan扫描模式
	void Init();									//初始化,初始化完毕后要调用ILI9341_GramScan_Mode()选择扫描模式
	void Reset();									 //复位
    void DispChar ( uint16_t X, uint16_t Y, const char cChar ,uint16_t  TextColor,uint16_t BackColor);  //显示一个Ascll字符,字体颜色和背景颜色
	void DispStringLine ( uint16_t line,  char * pStr,uint16_t  TextColor,uint16_t BackColor );             //显示Ascll字符串
    void Fill(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height,uint16_t Color);                   //填充区域某种颜色(可用作清屏)
   static void FillPixel_Mode(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height);                      //进入填充像素模式
   static void WritePixel(uint16_t RGB565);                                                                  //根据扫描模式配合上面方式填充，一共填充 Width*Height像素

private:
    GPIO_ILI9341_Lcd  * ILI9341_Lcd_GPIO;		
	uint16_t LCD_X_LENGTH ;
	uint16_t LCD_Y_LENGTH ;
    
   static  inline  void ILI9341_Write_Cmd(uint16_t Cmd);				//写命令
   static	inline  void ILI9341_Write_Data(uint16_t Data);		    //写数据
	inline  uint16_t ILI9341_Read();							//读数据
	void   REG_Config();									    //寄存器初始化
	static void   OpenWindow(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height);  //在LCD上开辟一个窗口
    void   Init_Gpio();                                         //初始化引脚
    void   Init_FSMC();                                         //配置FSMC
};







#endif


