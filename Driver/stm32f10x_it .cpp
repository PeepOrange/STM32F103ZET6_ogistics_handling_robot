#ifdef __cplusplus
 extern "C"
{

    
#include "stm32f10x_it.h"   
#include "stm32f10x.h"                  
#include <includes.h>
#include "User_main.h"

    
    
    
    
    


    
    
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



void EXTI1_IRQHandler()     //前传感器
{
    OS_ERR err;
    char * p_mem_blk;
    OSIntEnter();       //进入中断
    p_mem_blk =(char*) OSMemGet(&mem,&err);
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
//		* p_mem_blk = 1;     //获取接收到的数据        
//		OSTaskQPost ((OS_TCB      *)&Position_TCB,      //目标任务的控制块
//                     (void        *)p_mem_blk,             //消息内容的首地址
//                     (OS_MSG_SIZE  )1,                     //消息长度
//                     (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
//                     (OS_ERR      *)&err);                 //返回错误类型  
//        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    
    OSIntExit();       //退出中断   
    
}


void EXTI2_IRQHandler()     //后传感器
{
    OS_ERR err;
    char * p_mem_blk;
    OSIntEnter();       //进入中断
    p_mem_blk =(char*) OSMemGet(&mem,&err);
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
//		* p_mem_blk = 2;     //获取接收到的数据        
//		OSTaskQPost ((OS_TCB      *)&Position_TCB,      //目标任务的控制块
//                     (void        *)p_mem_blk,             //消息内容的首地址
//                     (OS_MSG_SIZE  )1,                     //消息长度
//                     (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
//                     (OS_ERR      *)&err);                 //返回错误类型  
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    
    OSIntExit();       //退出中断   
    
}


void EXTI3_IRQHandler()         //左传感器
{
    OS_ERR err;
    char * p_mem_blk;
    OSIntEnter();       //进入中断
    p_mem_blk =(char*) OSMemGet(&mem,&err);
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
//		* p_mem_blk = 3;     //获取接收到的数据        
//		OSTaskQPost ((OS_TCB      *)&Position_TCB,      //目标任务的控制块
//                     (void        *)p_mem_blk,             //消息内容的首地址
//                     (OS_MSG_SIZE  )1,                     //消息长度
//                     (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
//                     (OS_ERR      *)&err);                 //返回错误类型  
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    
    OSIntExit();       //退出中断   
    
}

void EXTI9_5_IRQHandler()       //右传感器
{
    OS_ERR err;
    char * p_mem_blk;
    OSIntEnter();       //进入中断
    p_mem_blk =(char*) OSMemGet(&mem,&err);
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
//		* p_mem_blk = 4;     //获取接收到的数据        
//		OSTaskQPost ((OS_TCB      *)&Position_TCB,      //目标任务的控制块
//                     (void        *)p_mem_blk,             //消息内容的首地址
//                     (OS_MSG_SIZE  )1,                     //消息长度
//                     (OS_OPT       )OS_OPT_POST_FIFO,      //发布到任务消息队列的入口端
//                     (OS_ERR      *)&err);                 //返回错误类型  
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    
    OSIntExit();       //退出中断   
    
}

















}
#endif



