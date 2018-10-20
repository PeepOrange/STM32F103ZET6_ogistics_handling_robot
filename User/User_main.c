#include "User_main.h"


//方位枚举
typedef enum 
{
    UP =0,
    Back=1,
    Left=2,
    Right=3,    
    Stop =4,
}
Diretion;



//任务控制块
OS_TCB  Key1_Scan_TCB;      //按键1的任务控制块
OS_TCB  USART1_Get_TCB;     //串口1接受到信息任务
OS_TCB  Run_TCB;            //循迹+PID模块





//当前车子行驶方向
Diretion  Car_Dir;




//内存池
OS_MEM   mem;
uint8_t ucArray [ 4 ] [ 4 ];   //声明内存分区大小






void User_main()
{
    OS_ERR      err;

    OSSchedRoundRobinCfg((CPU_BOOLEAN   )DEF_ENABLED,          //使能时间片轮转调度
                        (OS_TICK       )0,                    //把 OSCfg_TickRate_Hz / 10 设为默认时间片值
                        (OS_ERR       *)&err );               //返回错误类型


    OSMemCreate ((OS_MEM      *)&mem,             //指向内存管理对象
                 (CPU_CHAR    *)"Mem",   //命名内存管理对象
                 (void        *)ucArray,          //内存分区的首地址
                 (OS_MEM_QTY   )4,               //内存分区中内存块数目
                 (OS_MEM_SIZE  )4,                //内存块的字节数目
                 (OS_ERR      *)&err);            //返回错误类型    
    
    
    
    OSTaskCreate((OS_TCB     *)&Key1_Scan_TCB,                /* 任务控制块地址                               */
                 (CPU_CHAR   *)"按键1检测",                //任务名字
                 (OS_TASK_PTR ) Key1_Scan,                   //任务函数
                 (void       *) 0,                              //传递给任务函数的实参（形参p_arg）
                 (OS_PRIO     ) Key1_Scan_PRIO,            //任务的优先级
                 (CPU_STK    *)&Key1_Scan_STK[0],             //任务堆栈的基地址
                 (CPU_STK_SIZE) Key1_Scan_STK_SIZE / 10,   //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) Key1_Scan_STK_SIZE,        //任务的堆栈空间（单位：size(CPU_STK)）
                 (OS_MSG_QTY  ) 2u,                             //任务可接受的最大消息数
                 (OS_TICK     ) 0u,                             //任务的时间片节拍数（默认值0表示 OS_CFG_TICK_RATE_HZ/10  即10ms ）
                 (void       *) 0,                              //任务拓展（0表示不拓展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //任务选项
                 (OS_ERR     *)&err);                           //返回错误类型

                 
    OSTaskCreate((OS_TCB     *)&USART1_Get_TCB,                /* 任务控制块地址                               */
                 (CPU_CHAR   *)"串口接受",                //任务名字
                 (OS_TASK_PTR ) USART1_Get,                   //任务函数
                 (void       *) 0,                              //传递给任务函数的实参（形参p_arg）
                 (OS_PRIO     ) USART1_Get_PRIO,            //任务的优先级
                 (CPU_STK    *)&USART1_Get_STK[0],             //任务堆栈的基地址
                 (CPU_STK_SIZE) USART1_Get_STK_SIZE / 10,   //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) USART1_Get_STK_SIZE,        //任务的堆栈空间（单位：size(CPU_STK)）
                 (OS_MSG_QTY  ) 2u,                             //任务可接受的最大消息数
                 (OS_TICK     ) 0u,                             //任务的时间片节拍数（默认值0表示 OS_CFG_TICK_RATE_HZ/10  即10ms ）
                 (void       *) 0,                              //任务拓展（0表示不拓展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //任务选项
                 (OS_ERR     *)&err);                           //返回错误类型 
  
    OSTaskCreate(&Run_TCB,"寻线",Run,0,Run_PRIO,Run_STK,Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);  
                 
}






static void    USART1_Get(void *p_arg)
{
	OS_ERR         err;
	OS_MSG_SIZE    msg_size;
	CPU_SR_ALLOC();
	
	char * pMsg;

	
	(void)p_arg;

					 
	while (DEF_TRUE) {                                           //任务体
		/* 阻塞任务，等待任务消息 */
		pMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
                          (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
                          (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
                          (CPU_TS        *)0,                    //返回消息被发布的时间戳
                          (OS_ERR        *)&err);                //返回错误类型

		OS_CRITICAL_ENTER();                                       //进入临界段，避免串口打印被打断

		printf ( "%c", * pMsg );                                   //打印消息内容

		OS_CRITICAL_EXIT();                                        //退出临界段
		
		/* 退还内存块 */
		OSMemPut (  (OS_MEM  *)&mem,                                 //指向内存管理对象
                    (void    *)pMsg,                                 //内存块的首地址
                    (OS_ERR  *)&err);		                          //返回错误类型		
	}       
}






static void    Key1_Scan(void *p_arg)
{
    OS_ERR      err;
 	CPU_SR_ALLOC();   
    while(1)
    {
      OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
      OS_CRITICAL_ENTER();                             
        
      printf ( "CPU使用率：%d.%d%%\r\n",
             OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100 );  

      printf ( "CPU最大使用率：%d.%d%%\r\n", 
                 OSStatTaskCPUUsageMax / 100, OSStatTaskCPUUsageMax % 100 );        
      
     printf ("\n\r左前轮的Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",LeftUp_PID_Mortor.Kp,LeftUp_PID_Mortor.Ki,LeftUp_PID_Mortor.Kd); 
     printf ("左前轮的目标值:%.2f   读取值:%.2f \r\n",LeftUp_PID_Mortor.goal_point,LeftUp_PID_Mortor.read_point);    
     printf ("左前轮的上次误差:%.2f \r\n",LeftUp_PID_Mortor.last_Error);    

     printf ("\n\r左后轮的Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",LeftBack_PID_Mortor.Kp,LeftBack_PID_Mortor.Ki,LeftBack_PID_Mortor.Kd); 
     printf ("左后轮的目标值:%.2f   读取值:%.2f \r\n",LeftBack_PID_Mortor.goal_point,LeftBack_PID_Mortor.read_point); 
     printf ("左后轮的上次误差:%.2f \r\n",LeftBack_PID_Mortor.last_Error);   
        
     printf ("\n\r右前轮的Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",RightUp_PID_Mortor.Kp,RightUp_PID_Mortor.Ki,RightUp_PID_Mortor.Kd); 
     printf ("右前轮的目标值:%.2f   读取值:%.2f \r\n",RightUp_PID_Mortor.goal_point,RightUp_PID_Mortor.read_point);       
     printf ("右前轮的上次误差:%.2f \r\n",RightUp_PID_Mortor.last_Error);  
        
     printf ("\n\r右后轮的Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",RightBack_PID_Mortor.Kp,RightBack_PID_Mortor.Ki,RightBack_PID_Mortor.Kd); 
     printf ("右后轮的目标值:%.2f   读取值:%.2f \r\n",RightBack_PID_Mortor.goal_point,RightBack_PID_Mortor.read_point); 
     printf ("右后轮的上次误差:%.2f \r\n",RightBack_PID_Mortor.last_Error);  


      OS_CRITICAL_EXIT();   

    }
    
}




static void    Run(void *p_arg)
{
    OS_ERR      err;
    while(1)
    {
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
   
     if(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)||!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3))     //前左  右前      低电平（即检测到黑线）
     {
         RightUp_PWM=Correct_Up_PWM;
         RightBack_PWM=Correct_Up_PWM;
         LeftUp_PWM=Correct_Back_PWM;
         LeftBack_PWM=Correct_Back_PWM;
     }

     else if(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)||!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5))    //前右  右后
     {
         RightUp_PWM=Correct_Back_PWM;
         RightBack_PWM=Correct_Back_PWM;
         LeftUp_PWM=Correct_Up_PWM;
         LeftBack_PWM=Correct_Up_PWM;
     }
     
     else
     {
         RightUp_PWM=Goal_RightUp_PWM;
         RightBack_PWM=Goal_RightBack_PWM;
         LeftUp_PWM=Goal_LeftUp_PWM;
         LeftBack_PWM=Goal_LeftBack_PWM;                  
     }    
            
     PID_PWM_Adujust(LeftUp_PWM,LeftBack_PWM,RightUp_PWM,RightBack_PWM);
    }        
    
}























