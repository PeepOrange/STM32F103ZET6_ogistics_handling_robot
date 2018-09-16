#ifdef __cplusplus
 extern "C"
{

    
#include "stm32f10x_it.h"   
#include "stm32f10x.h"                  
#include <includes.h>
#include "User_main.h"

    
    
    
    
    

//void SysTick_Handler(){};  //ϵͳ�δ��ж��Է�ֹ���������ļ���ѭ��

    
    
void EXTI0_IRQHandler()
{
    OS_ERR err;
    OSIntEnter();       //�����ж�
    
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        OSTaskSemPost(&Key1_Scan_TCB,OS_OPT_POST_NONE,&err);        //��Key1_Scan������������Ϣ��
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    
    OSIntExit();       //�˳��ж�   
    
    
}

	

void EXTI4_IRQHandler()
{
    OS_ERR err;
    
    OSIntEnter();       //�����ж�
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);        //��Key2_Scan������������Ϣ��
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



