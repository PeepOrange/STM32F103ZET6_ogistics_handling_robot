#ifdef __cplusplus
 extern "C"
{

    
#include "stm32f10x_it.h"   
#include "stm32f10x.h"                  
#include <includes.h>
#include "User_main.h"

    
uint16_t LeftUp_PWM = 200;                              //Ĭ�ϵ���ǰ�ֵ�PWM,[-1000,1000]
uint16_t LeftBack_PWM = 200;                            //Ĭ�ϵ�����ֵ�PWM,[-1000,1000]
uint16_t RightUp_PWM = 200;                             //Ĭ�ϵ���ǰ�ֵ�PWM,[-1000,1000]
uint16_t RightBack_PWM = 200;                           //Ĭ�ϵ��Һ��ֵ�PWM,[-1000,1000]
    
void TIM6_IRQHandler()
{
    OSIntEnter();       //�����ж�
    
    if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET)
    {
        PID_PWM_Adujust(LeftUp_PWM,LeftBack_PWM,RightUp_PWM,RightBack_PWM);
    }
    TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
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



