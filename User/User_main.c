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
OS_TCB  Postion_TCB;        //定位任务



//当前车子行驶方向
Diretion  Car_Dir=Test_Dir;
Steering_engine_angle_Struct  Steering_engine_angle;



//内存池
OS_MEM   mem;
uint8_t ucArray [ 4 ] [ 4 ];   //声明内存分区大小






void User_main()
{
    OS_ERR      err;
    Pos_X=0;
    Pos_Y=0;
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
      
    Steering_engine_angle.down_Steering_angle=Test_down_Steering_angle;
    Steering_engine_angle.hand_Steering_angle=Test_hand_Steering_angle;
    Steering_engine_angle.mid_Steering_angle=Test_mid_Steering_angle;
    Steering_engine_angle.up_Steering_angle=Test_up_Steering_angle;             
                 
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

    while(1)
    {
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
    OSTaskCreate(&Run_TCB,"寻线",Run,0,Run_PRIO,Run_STK,Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);  
    OSTaskCreate(&Postion_TCB,"定位",Postion,0,Postion_PRIO,Postion_STK,Postion_STK_SIZE/10,Postion_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);  
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
     OSTaskDel(&Run_TCB,&err);   
     OSTaskDel(&Postion_TCB,&err);           
     Pos_X=0;
     Pos_Y=0;
     Run_Stop();  
    
    }
    
}






static void    Run(void *p_arg)
{
    OS_ERR      err;
    while(1)
    {
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
     
     switch(Car_Dir)
        {
         case UP:
         {
             Run_Up();
             break;
         }
         case Back:
         {
             Run_Back();
             break;
         }
         case Left:
         {             
             Run_Left();
             break;
         }
         case Right:
         {
             Run_Right();
             break;
         }
         case Stop:
         {
             Run_Stop();
             break;
         }         
        }
     if(Pos_Y==3)
     Car_Dir=Left;
     if(Car_Dir!=Stop)
     PID_PWM_Adujust(LeftUp_PWM,LeftBack_PWM,RightUp_PWM,RightBack_PWM);
    }        
    
}







static void    Postion(void *p_arg)
{
    OS_ERR      err;
    while(1)
    {
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
     
     switch(Car_Dir)
        {
         case UP:
         {
             Up_Position();
             break;
         }
         case Back:
         {
             Back_Position();   
             break;
         }
         case Left:
         {             
             Left_Position();
             break;
         }
         case Right:
         {
             Right_Position();
             break;
         }
         case Stop:
         {
             break;
         }         
        }
    }        
    
}















