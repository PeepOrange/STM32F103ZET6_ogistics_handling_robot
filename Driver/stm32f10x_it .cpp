#ifdef __cplusplus
 extern "C"
{

    
#include "stm32f10x_it.h"   
#include "stm32f10x.h"                  
#include <includes.h>
#include "User_main.h"

    
    
    
    
    

//void SysTick_Handler(){};  //系统滴答中断以防止陷入启动文件死循环

    
    
void EXTI0_IRQHandler()
{
    OS_ERR err;
    OSIntEnter();       //进入中断
    
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        OSTaskSemPost(&Key1_Scan_TCB,OS_OPT_POST_NONE,&err);        //向Key1_Scan任务发送任务信息量
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    
    OSIntExit();       //退出中断   
    
    
}

	

void EXTI4_IRQHandler()
{
    OS_ERR err;
    
    OSIntEnter();       //进入中断
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);        //向Key2_Scan任务发送任务信息量
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    
    OSIntExit();       //退出中断   
    
}

void USART1_IRQHandler() 
{
    OS_ERR err;
    char * p_mem_blk;
    OSIntEnter();       //进入中断
    p_mem_blk =(char*) OSMemGet(&mem,&err);
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
    {
		* p_mem_blk = USART_ReceiveData ( USART1 );     //获取接收到的数据
		
		OSTaskQPost ((OS_TCB      *)&USART1_Get_TCB,      //目标任务的控制块
                     (void        *)p_mem_blk,             //消息内容的首地址
                     (OS_MSG_SIZE  )1,                     //消息长度
                     (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
                     (OS_ERR      *)&err);                 //返回错误类型        
    }
    OSIntExit();       //退出中断   
   
}
























}
#endif



