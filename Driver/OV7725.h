#ifndef _OV7725_H__
#define _OV7725_H__

#include "stm32f10x.h"                  // Device header
#include "gpio.h"
#include "iic_cs.h"



//通过SCCB时序操作OV7725摄像头


/*参数说明

StyleMode       图像风格
0               正常
1               黑白
2               偏蓝
3               复古
4               偏红
5               偏绿
6               反相

Lightmode       光照模式
0               自动
1               晴天
2               多云
3               办公室
4               家里
5               夜晚

Sat             饱和度
[-4,+4]

Bri             亮度
[-4,+4]

Con             对比度
[-4,+4]


void  Set_Window(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t QVGA_Or_VGA)
* @brief  设置图像输出窗口（分辨率）QVGA
* @param  x:窗口x起始位置
* @param  y:窗口y起始位置
* @param  width:窗口宽度
* @param  height:窗口高度
* @param  QVGA_Or_VGA:   0,QVGA模式，1，VGA模式

* @note   QVGA模式 参数要求，x + width <= 320 ,sy+height <= 240
* 		  VGA模式  参数要求，x + width <= 640 ,sy+height <= 480
*		  但由于 液晶屏分辨率 和 FIFO空间 的限制，本工程不适用于超过320*240的配置
*         使用VGA模式主要是因为OV7725无法直接交换XY方向，QVGA不方便使用竖平显示，
*		  设置成VGA模式，可以使用竖屏显示，
*		  相对QVGA模式，同样分辨率下 VGA模式 图像采样帧率稍慢

*/


/*
FIFO_VSYNC  下降沿触发外部中断配置

ov7725 场中断 服务程序 
void OV7725_VSYNC_EXTI_INT_FUNCTION ( void )
{
    if ( EXTI_GetITStatus(OV7725_VSYNC_EXTI_LINE) != RESET ) 	//检查EXTI_Line0线路上的中断请求是否发送到了NVIC 
    {
            if( Ov7725_vsync == 0 )
        {
            this->P_OV7725_Gpio->FIFO_WRST.reset(); 	                //拉低使FIFO写(数据from摄像头)指针复位
            this->P_OV7725_Gpio->FIFO_WE.set();	                        //拉高使FIFO写允许
            this->P_OV7725_Gpio->FIFO_WRST.set(); 	
            Ov7725_vsync = 1;	
        }
        else if( Ov7725_vsync == 1 )
        {
            this->P_OV7725_Gpio->FIFO_WE.reset();	                      //拉低使FIFO写暂停
            Ov7725_vsync = 2;
        }         
        EXTI_ClearITPendingBit(OV7725_VSYNC_EXTI_LINE);		    //清除EXTI_Line0线路挂起标志位        
    }    
}



while(1)
	{
		//接收到新图像进行显示
		if( Ov7725_vsync == 2 )
		{
            OV7725::Prepare();  			//FIFO准备					
            Display_ILI9341_LCD(x,y,width,height);
			Ov7725_vsync = 0;			
		}
    }

*/


struct OV7725_GPIO
{
	GPIO *SCL;
	GPIO *SDA;
    
    GPIO *FIFO_OE;       //FIFO输出使能
    GPIO *FIFO_WRST;     //FIFO写复位
    GPIO *FIFO_RRST;     //FIFO读复位
    GPIO *FIFO_RCLK;     //FIFO读时钟
    GPIO *FIFO_WE;       //FIFO写使能
    GPIO *FIFO_VSYNC;    //FIFO采集完成标志位（需配置外部中断及其中断顺序，此处仅配置引脚模式）

    GPIO_TypeDef *FIFO_DATA_Port;   //FIFO输出数据_8Pin端口,低8位
   
};


class OV7725: protected IIC_CS
{
	OV7725(OV7725_GPIO *gpio);


    public:
    FunctionalState    Init();                            //初始化，返回值为是否成功          
    void               Set_StyleMode(uint8_t StyleMode); //图像风格设置
    void               Set_LightMode(uint8_t Lightmode); //光照模式设置
    void               Set_ColorSaturation(int8_t Sat) ; //设置饱和度
    void               Set_Brightness(int8_t Bri);       //设置亮度
    void               Set_Contrast(int8_t Con);         //设置对比度
    void               Set_Window(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t QVGA_Or_VGA);      //设置窗口大小及其图像模式
    void               Prepare();                        //FIFO准备
    uint16_t            Read_FIFO_Pixel();                //读一个像素点RGB565数据
    
    //显示图像在屏幕上
    //之前要初始化 ILI9341_LCD 
    //ILI9341_LCD扫描模式 
    //1.3.5.7 横向QVGA   
    //  0.2.4.6 竖向VGA
    void               Display_ILI9341_LCD(uint16_t x,uint16_t y,uint16_t width,uint16_t height);                        
    
    private:
    OV7725_GPIO * P_OV7725_Gpio;
    void    Init_Gpio();                                  //引脚初始化

};
















#endif


