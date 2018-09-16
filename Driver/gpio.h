#ifndef __gpio_H
#define __gpio_H

#include "stm32f10x_gpio.h"    
#include "stdint.h"



/*
		GPIOMode_TypeDef
    ����ģʽ
    GPIO_Mode_AIN         				//ģ��������
    GPIO_Mode_IN_FLOATING     		//�������븡��
    GPIO_Mode_IPD   						  //������������ 
    GPIO_Mode_IPU    							//������������
    GPIO_Mode_Out_OD   						//��©�����
    GPIO_Mode_Out_PP   						//�����������
    GPIO_Mode_AF_OD       				//���ù���OD
    GPIO_Mode_AF_PP       				//�����������



	GPIOSpeed_TypeDef
	����Ƶ��
  GPIO_Speed_10MHz 	 //10M
  GPIO_Speed_2MHz, 	 //2M
  GPIO_Speed_50MHz   //50M
 

*/

class GPIO
{
	public:
	  GPIO(GPIO_TypeDef *port,uint16_t pin);
		void mode(GPIOMode_TypeDef mode,GPIOSpeed_TypeDef speed); //��ʼ���˿�
		void set();                      //��λ
		void reset();                    //��λ
	  void toggle();                   //ȡ��
	  uint8_t out_read();              //�������Ŷ�״̬(���״̬)
	  uint8_t in_read();               //�������Ŷ�״̬(����״̬)
	  
	  void Write(uint16_t val);        //д2���ֽ�(�������portд��val)
	private:	
	  uint16_t pin;
	  GPIO_TypeDef *port;  
};



#endif
