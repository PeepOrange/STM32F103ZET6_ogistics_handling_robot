#ifndef __stm32f10x_it_H
#define __stm32f10x_it_H



 #ifdef __cplusplus
extern "C"
{
void SysTick_Handler();   //ϵͳ�δ�ʱ���жϷ�����


void WWDG_IRQHandler() ;  //���ڿ��Ź��жϷ�����


void PVD_IRQHandler();   //����EXTI�ĵ�Դ��ѹ���(PVD)�ж� 


void TAMPER_IRQHandler();   //�����жϷ�����


void RTC_IRQHandler();   //RTCʵʱʱ���жϷ�����


void FLASH_IRQHandler() ;  //����ȫ���жϷ�����


void RCC_IRQHandler() ;  //��λ��ʱ�ӿ����жϷ�����


void EXTI0_IRQHandler()  ; //EXTI��0�жϷ�����


void EXTI1_IRQHandler() ;  //EXTI��1�жϷ�����


void EXTI2_IRQHandler();   //EXTI��2�жϷ�����

void EXTI3_IRQHandler();   //EXTI��3�жϷ�����


void EXTI4_IRQHandler() ;  //EXTI��4�жϷ�����


void DMA1_Channel1_IRQHandler() ;  //DMA1ͨ��1ȫ���жϷ�����


void DMA1_Channel2_IRQHandler() ;  //DMA1ͨ��2ȫ���жϷ�����


void DMA1_Channel3_IRQHandler() ;  //DMA1ͨ��3ȫ���жϷ�����


void DMA1_Channel4_IRQHandler() ;  //DMA1ͨ��4ȫ���жϷ�����


void DMA1_Channel5_IRQHandler() ; //DMA1ͨ��5ȫ���жϷ�����


void DMA1_Channel6_IRQHandler() ;  //DMA1ͨ��6ȫ���жϷ�����


void DMA1_Channel7_IRQHandler() ;  //DMA1ͨ��7ȫ���жϷ�����


void ADC1_2_IRQHandler()  ; //ADCȫ���ж�


void USB_HP_CAN1_TX_IRQHandler()  ; //USB�����ȼ���CAN�����ж�


void USB_LP_CAN1_RX0_IRQHandler()  ; //USB�����ȼ���CAN����0�ж� 


void CAN1_RX1_IRQHandler()  ; //CAN����1�ж�



void CAN1_SCE_IRQHandler()  ;//CAN SCE�ж�



void EXTI9_5_IRQHandler()  ; //EXTI��[5:9]�ж� 


void TIM1_BRK_IRQHandler()   ;//TIM1�Ͽ��ж�


void TIM1_UP_IRQHandler()   ;//TIM1�����ж�


void TIM1_TRG_COM_IRQHandler()   ;//TIM1������ͨ���ж� 


void TIM1_CC_IRQHandler() ;  //TIM1����Ƚ��ж�


void TIM2_IRQHandler()  ; //TIM2ȫ���ж�


void TIM3_IRQHandler()  ; //TIM3ȫ���ж�


void TIM4_IRQHandler()  ; //TIM4ȫ���ж�


void I2C1_EV_IRQHandler() ;  //I2C1�¼��ж�



void I2C1_ER_IRQHandler() ;  //I2C1�����ж�


void I2C2_EV_IRQHandler() ;  //I2C2�¼��ж�



void I2C2_ER_IRQHandler() ;  //I2C2�����ж�



void SPI1_IRQHandler() ;  //SPI1ȫ���ж�


void SPI2_IRQHandler()  ; //SPI2ȫ���ж� 



void USART1_IRQHandler() ;  //USART1ȫ���ж� 


void USART2_IRQHandler()   ;//USART2ȫ���ж� 



void USART3_IRQHandler()  ; //USART3ȫ���ж�



void EXTI15_10_IRQHandler()  ; //EXTI��[15:10]�ж� 


void RTCAlarm_IRQHandler()  ; //����EXTI��RTC�����ж�


void USBWakeUp_IRQHandler() ;  //����EXTI�Ĵ�USB���������ж� 


}
#endif








#endif

