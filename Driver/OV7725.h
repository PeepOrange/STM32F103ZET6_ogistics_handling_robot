#ifndef _OV7725_H__
#define _OV7725_H__

#include "stm32f10x.h"                  // Device header
#include "gpio.h"
#include "iic_cs.h"



//ͨ��SCCBʱ�����OV7725����ͷ


/*����˵��

StyleMode       ͼ����
0               ����
1               �ڰ�
2               ƫ��
3               ����
4               ƫ��
5               ƫ��
6               ����

Lightmode       ����ģʽ
0               �Զ�
1               ����
2               ����
3               �칫��
4               ����
5               ҹ��

Sat             ���Ͷ�
[-4,+4]

Bri             ����
[-4,+4]

Con             �Աȶ�
[-4,+4]


void  Set_Window(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t QVGA_Or_VGA)
* @brief  ����ͼ��������ڣ��ֱ��ʣ�QVGA
* @param  x:����x��ʼλ��
* @param  y:����y��ʼλ��
* @param  width:���ڿ��
* @param  height:���ڸ߶�
* @param  QVGA_Or_VGA:   0,QVGAģʽ��1��VGAģʽ

* @note   QVGAģʽ ����Ҫ��x + width <= 320 ,sy+height <= 240
* 		  VGAģʽ  ����Ҫ��x + width <= 640 ,sy+height <= 480
*		  ������ Һ�����ֱ��� �� FIFO�ռ� �����ƣ������̲������ڳ���320*240������
*         ʹ��VGAģʽ��Ҫ����ΪOV7725�޷�ֱ�ӽ���XY����QVGA������ʹ����ƽ��ʾ��
*		  ���ó�VGAģʽ������ʹ��������ʾ��
*		  ���QVGAģʽ��ͬ���ֱ����� VGAģʽ ͼ�����֡������

*/


/*
FIFO_VSYNC  �½��ش����ⲿ�ж�����

ov7725 ���ж� ������� 
void OV7725_VSYNC_EXTI_INT_FUNCTION ( void )
{
    if ( EXTI_GetITStatus(OV7725_VSYNC_EXTI_LINE) != RESET ) 	//���EXTI_Line0��·�ϵ��ж������Ƿ��͵���NVIC 
    {
            if( Ov7725_vsync == 0 )
        {
            this->P_OV7725_Gpio->FIFO_WRST.reset(); 	                //����ʹFIFOд(����from����ͷ)ָ�븴λ
            this->P_OV7725_Gpio->FIFO_WE.set();	                        //����ʹFIFOд����
            this->P_OV7725_Gpio->FIFO_WRST.set(); 	
            Ov7725_vsync = 1;	
        }
        else if( Ov7725_vsync == 1 )
        {
            this->P_OV7725_Gpio->FIFO_WE.reset();	                      //����ʹFIFOд��ͣ
            Ov7725_vsync = 2;
        }         
        EXTI_ClearITPendingBit(OV7725_VSYNC_EXTI_LINE);		    //���EXTI_Line0��·�����־λ        
    }    
}



while(1)
	{
		//���յ���ͼ�������ʾ
		if( Ov7725_vsync == 2 )
		{
            OV7725::Prepare();  			//FIFO׼��					
            Display_ILI9341_LCD(x,y,width,height);
			Ov7725_vsync = 0;			
		}
    }

*/


struct OV7725_GPIO
{
	GPIO *SCL;
	GPIO *SDA;
    
    GPIO *FIFO_OE;       //FIFO���ʹ��
    GPIO *FIFO_WRST;     //FIFOд��λ
    GPIO *FIFO_RRST;     //FIFO����λ
    GPIO *FIFO_RCLK;     //FIFO��ʱ��
    GPIO *FIFO_WE;       //FIFOдʹ��
    GPIO *FIFO_VSYNC;    //FIFO�ɼ���ɱ�־λ���������ⲿ�жϼ����ж�˳�򣬴˴�����������ģʽ��

    GPIO_TypeDef *FIFO_DATA_Port;   //FIFO�������_8Pin�˿�,��8λ
   
};


class OV7725: protected IIC_CS
{
	OV7725(OV7725_GPIO *gpio);


    public:
    FunctionalState    Init();                            //��ʼ��������ֵΪ�Ƿ�ɹ�          
    void               Set_StyleMode(uint8_t StyleMode); //ͼ��������
    void               Set_LightMode(uint8_t Lightmode); //����ģʽ����
    void               Set_ColorSaturation(int8_t Sat) ; //���ñ��Ͷ�
    void               Set_Brightness(int8_t Bri);       //��������
    void               Set_Contrast(int8_t Con);         //���öԱȶ�
    void               Set_Window(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t QVGA_Or_VGA);      //���ô��ڴ�С����ͼ��ģʽ
    void               Prepare();                        //FIFO׼��
    uint16_t            Read_FIFO_Pixel();                //��һ�����ص�RGB565����
    
    //��ʾͼ������Ļ��
    //֮ǰҪ��ʼ�� ILI9341_LCD 
    //ILI9341_LCDɨ��ģʽ 
    //1.3.5.7 ����QVGA   
    //  0.2.4.6 ����VGA
    void               Display_ILI9341_LCD(uint16_t x,uint16_t y,uint16_t width,uint16_t height);                        
    
    private:
    OV7725_GPIO * P_OV7725_Gpio;
    void    Init_Gpio();                                  //���ų�ʼ��

};
















#endif


