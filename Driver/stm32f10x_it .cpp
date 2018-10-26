#ifdef __cplusplus
 extern "C"
{

    
#include "stm32f10x_it.h"   
#include "stm32f10x.h"                  
#include <includes.h>
#include "User_main.h"



    
int16_t LeftUp_PWM ;                              //Ĭ�ϵ���ǰ�ֵ�PWM,[-1000,1000]
int16_t LeftBack_PWM ;                            //Ĭ�ϵ�����ֵ�PWM,[-1000,1000]
int16_t RightUp_PWM ;                             //Ĭ�ϵ���ǰ�ֵ�PWM,[-1000,1000]
int16_t RightBack_PWM ;                           //Ĭ�ϵ��Һ��ֵ�PWM,[-1000,1000]
    
void TIM6_IRQHandler()
{
    OS_ERR err;
    OSIntEnter();       //�����ж�
    if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
    {
       OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);        //��PID+ѭ��������������Ϣ��
       OSTaskSemPost(&Postion_TCB,OS_OPT_POST_NONE,&err);        //��λ������������Ϣ��        
    }
    TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
    OSIntExit();       //�˳��ж�  
}    
    

void TIM7_IRQHandler()
{
    static uint8_t flag;    
    OSIntEnter();       //�����ж�
    if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
    {
     if(flag>=200)
         flag=0;
     else
     {
         if(flag<Steering_engine_angle.up_Steering_angle)
               GPIOC->BSRR = GPIO_Pin_0;
         else
               GPIOC->BRR = GPIO_Pin_0;

         if(flag<Steering_engine_angle.mid_Steering_angle)
               GPIOC->BSRR = GPIO_Pin_1;
         else
               GPIOC->BRR = GPIO_Pin_1;
         
         if(flag<Steering_engine_angle.down_Steering_angle)
               GPIOC->BSRR = GPIO_Pin_2;
         else
               GPIOC->BRR = GPIO_Pin_2;

         if(flag<Steering_engine_angle.hand_Steering_angle)
               GPIOC->BSRR = GPIO_Pin_3;
         else
               GPIOC->BRR = GPIO_Pin_3;                  
     }
     flag++;        
    }
    TIM_ClearITPendingBit(TIM7,TIM_FLAG_Update);
    OSIntExit();       //�˳��ж�  
}    
    
    
void EXTI4_IRQHandler()
{
    OS_ERR err;
    OSIntEnter();       //�����ж�
    
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        OSTaskSemPost(&Key1_Scan_TCB,OS_OPT_POST_NONE,&err);        //��Key1_Scan������������Ϣ��
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    
    OSIntExit();       //�˳��ж�       
}



void USART1_IRQHandler() 
{
    OS_ERR err;
    char * p_mem_blk;
    OSIntEnter();       //�����ж�
    p_mem_blk =(char*) OSMemGet(&mem,&err);
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
    {
		* p_mem_blk = USART_ReceiveData ( USART1 );     //��ȡ���յ�������
		
		OSTaskQPost ((OS_TCB      *)&USART1_Get_TCB,      //Ŀ������Ŀ��ƿ�
                     (void        *)p_mem_blk,             //��Ϣ���ݵ��׵�ַ
                     (OS_MSG_SIZE  )1,                     //��Ϣ����
                     (OS_OPT       )OS_OPT_POST_FIFO,      //������������Ϣ���е���ڶ�
                     (OS_ERR      *)&err);                 //���ش�������        
    }
    OSIntExit();       //�˳��ж�   
   
}























}
#endif



