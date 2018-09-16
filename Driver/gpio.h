#ifndef __gpio_H
#define __gpio_H

#include "stm32f10x_gpio.h"    
#include "stdint.h"



/*
		GPIOMode_TypeDef
    引脚模式
    GPIO_Mode_AIN         				//模拟量输入
    GPIO_Mode_IN_FLOATING     		//数字输入浮空
    GPIO_Mode_IPD   						  //数字输入下拉 
    GPIO_Mode_IPU    							//数字输入上拉
    GPIO_Mode_Out_OD   						//开漏极输出
    GPIO_Mode_Out_PP   						//数字推挽输出
    GPIO_Mode_AF_OD       				//复用功能OD
    GPIO_Mode_AF_PP       				//复用推挽输出



	GPIOSpeed_TypeDef
	引脚频率
  GPIO_Speed_10MHz 	 //10M
  GPIO_Speed_2MHz, 	 //2M
  GPIO_Speed_50MHz   //50M
 

*/

class GPIO
{
	public:
	  GPIO(GPIO_TypeDef *port,uint16_t pin);
		void mode(GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed); //初始化端口
		void set();                      //置位
		void reset();                    //复位
	  void toggle();                   //取反
	  uint8_t out_read();              //单个引脚读状态(输出状态)
	  uint8_t in_read();               //单个引脚读状态(输入状态)
	  
	  void Write(uint16_t val);        //写2个字节(对输入的port写入val)
	private:	
	  uint16_t pin;
	  GPIO_TypeDef *port;  
};



#endif
