#ifndef __stm32f10x_it_H
#define __stm32f10x_it_H



 #ifdef __cplusplus
extern "C"
{
void SysTick_Handler();   //系统滴答定时器中断服务函数


void WWDG_IRQHandler() ;  //窗口看门狗中断服务函数


void PVD_IRQHandler();   //联到EXTI的电源电压检测(PVD)中断 


void TAMPER_IRQHandler();   //侵入中断服务函数


void RTC_IRQHandler();   //RTC实时时钟中断服务函数


void FLASH_IRQHandler() ;  //闪存全局中断服务函数


void RCC_IRQHandler() ;  //复位和时钟控制中断服务函数


void EXTI0_IRQHandler()  ; //EXTI线0中断服务函数


void EXTI1_IRQHandler() ;  //EXTI线1中断服务函数


void EXTI2_IRQHandler();   //EXTI线2中断服务函数

void EXTI3_IRQHandler();   //EXTI线3中断服务函数


void EXTI4_IRQHandler() ;  //EXTI线4中断服务函数


void DMA1_Channel1_IRQHandler() ;  //DMA1通道1全局中断服务函数


void DMA1_Channel2_IRQHandler() ;  //DMA1通道2全局中断服务函数


void DMA1_Channel3_IRQHandler() ;  //DMA1通道3全局中断服务函数


void DMA1_Channel4_IRQHandler() ;  //DMA1通道4全局中断服务函数


void DMA1_Channel5_IRQHandler() ; //DMA1通道5全局中断服务函数


void DMA1_Channel6_IRQHandler() ;  //DMA1通道6全局中断服务函数


void DMA1_Channel7_IRQHandler() ;  //DMA1通道7全局中断服务函数


void ADC1_2_IRQHandler()  ; //ADC全局中断


void USB_HP_CAN1_TX_IRQHandler()  ; //USB高优先级或CAN发送中断


void USB_LP_CAN1_RX0_IRQHandler()  ; //USB低优先级或CAN接收0中断 


void CAN1_RX1_IRQHandler()  ; //CAN接收1中断



void CAN1_SCE_IRQHandler()  ;//CAN SCE中断



void EXTI9_5_IRQHandler()  ; //EXTI线[5:9]中断 


void TIM1_BRK_IRQHandler()   ;//TIM1断开中断


void TIM1_UP_IRQHandler()   ;//TIM1更新中断


void TIM1_TRG_COM_IRQHandler()   ;//TIM1触发和通信中断 


void TIM1_CC_IRQHandler() ;  //TIM1捕获比较中断


void TIM2_IRQHandler()  ; //TIM2全局中断


void TIM3_IRQHandler()  ; //TIM3全局中断


void TIM4_IRQHandler()  ; //TIM4全局中断


void I2C1_EV_IRQHandler() ;  //I2C1事件中断



void I2C1_ER_IRQHandler() ;  //I2C1错误中断


void I2C2_EV_IRQHandler() ;  //I2C2事件中断



void I2C2_ER_IRQHandler() ;  //I2C2错误中断



void SPI1_IRQHandler() ;  //SPI1全局中断


void SPI2_IRQHandler()  ; //SPI2全局中断 



void USART1_IRQHandler() ;  //USART1全局中断 


void USART2_IRQHandler()   ;//USART2全局中断 



void USART3_IRQHandler()  ; //USART3全局中断



void EXTI15_10_IRQHandler()  ; //EXTI线[15:10]中断 


void RTCAlarm_IRQHandler()  ; //联到EXTI的RTC闹钟中断


void USBWakeUp_IRQHandler() ;  //联到EXTI的从USB待机唤醒中断 


}
#endif








#endif

