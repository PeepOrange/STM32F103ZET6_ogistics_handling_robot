#include "User_main.h"


//任务控制块
OS_TCB  Key1_Scan_TCB;      //按键1的任务控制块
OS_TCB  USART1_Get_TCB;     //串口1接受到信息任务
OS_TCB  Key2_Scan_TCB;      //按键2接受到信息任务
OS_TCB  Run_TCB;      //车子运行时的任务块
OS_TCB  LED_Twinkle_TCB;   //LED闪烁时的任务快




//内存池
OS_MEM   mem;
uint8_t ucArray [ 70 ] [ 4 ];   //声明内存分区大小



void User_main()
{
    OS_ERR      err;

    OSSchedRoundRobinCfg((CPU_BOOLEAN   )DEF_ENABLED,          //使能时间片轮转调度
                        (OS_TICK       )0,                    //把 OSCfg_TickRate_Hz / 10 设为默认时间片值
                        (OS_ERR       *)&err );               //返回错误类型


    OSMemCreate ((OS_MEM      *)&mem,             //指向内存管理对象
                 (CPU_CHAR    *)"Mem",   //命名内存管理对象
                 (void        *)ucArray,          //内存分区的首地址
                 (OS_MEM_QTY   )70,               //内存分区中内存块数目
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
 
                 
    OSTaskCreate((OS_TCB     *)&Key2_Scan_TCB,                /* 任务控制块地址                               */
                 (CPU_CHAR   *)"按键2检测",                //任务名字
                 (OS_TASK_PTR ) Key2_Scan,                   //任务函数
                 (void       *) 0,                              //传递给任务函数的实参（形参p_arg）
                 (OS_PRIO     ) Key2_Scan_PRIO,            //任务的优先级
                 (CPU_STK    *)&Key2_Scan_STK[0],             //任务堆栈的基地址
                 (CPU_STK_SIZE) Key2_Scan_STK_SIZE / 10,   //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) Key2_Scan_STK_SIZE,        //任务的堆栈空间（单位：size(CPU_STK)）
                 (OS_MSG_QTY  ) 2u,                             //任务可接受的最大消息数
                 (OS_TICK     ) 0u,                             //任务的时间片节拍数（默认值0表示 OS_CFG_TICK_RATE_HZ/10  即10ms ）
                 (void       *) 0,                              //任务拓展（0表示不拓展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //任务选项
                 (OS_ERR     *)&err);                           //返回错误类型 


                 
}



static void  Key1_Scan(void *p_arg)
{
	OS_ERR         err;
	CPU_INT16U     version;
	CPU_INT32U     cpu_clk_freq;
	CPU_SR_ALLOC();

	
	(void)p_arg;

	version = OSVersion(&err);                          //获取uC/OS版本号
	
  cpu_clk_freq = BSP_CPU_ClkFreq();                   //获取CPU时钟，时间戳是以该时钟计数

	
	while (DEF_TRUE) 
        {                                 
		/* 阻塞任务，直到KEY1被单击 */
		OSTaskSemPend ((OS_TICK   )0,                     //无期限等待
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果信号量不可用就等待
                     (CPU_TS   *)0,                     //获取信号量被发布的时间戳
                     (OS_ERR   *)&err);                 //返回错误类型
		

		OS_CRITICAL_ENTER();                              //进入临界段，避免串口打印被打断

        printf ( "\r\nuC/OS版本号：V%d.%02d.%02d\r\n",
             version / 10000, version % 10000 / 100, version % 100 );

        printf ( "CPU主频：%d MHz\r\n", cpu_clk_freq / 1000000 );  


        printf ( "CPU使用率：%d.%d%%\r\n",
             OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100 );  

        printf ( "CPU最大使用率：%d.%d%%\r\n", 
                 OSStatTaskCPUUsageMax / 100, OSStatTaskCPUUsageMax % 100 );


		
		OS_CRITICAL_EXIT();                              
		
	}
      
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


static void  Key2_Scan(void *p_arg)
{
    
 	OS_ERR         err;
    (void) p_arg;
    while(1)
    {
		OSTaskCreate(&LED_Twinkle_TCB,"LED闪烁",LED_Twinkle,0,LED_Twinkle_PRIO,&LED_Twinkle_STK[0],LED_Twinkle_STK_SIZE/10,LED_Twinkle_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    
        OSTaskSemPend ((OS_TICK   )0,                     //无期限等待
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果信号量不可用就等待
                     (CPU_TS   *)0,                     //获取信号量被发布的时间戳
                     (OS_ERR   *)&err);                 //返回错误类型 

        OSTaskCreate(&Run_TCB,"车子运行",Run,0,Run_PRIO,&Run_STK[0],Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    

        OSTaskDel(&LED_Twinkle_TCB,&err);
        LED_ALL_OFF();

        
       
        OSTaskSemPend ((OS_TICK   )0,                     //无期限等待
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果信号量不可用就等待
                     (CPU_TS   *)0,                     //获取信号量被发布的时间戳
                     (OS_ERR   *)&err);                 //返回错误类型 
        
       OSTaskDel(&Run_TCB,&err);
       Move_Stop();
    }
}


static void Run(void* p_arg)
{
  	OS_ERR         err;
    (void) p_arg;
    while(1)
    {
       Move_Up();
       OSTimeDly(2000,OS_OPT_TIME_DLY,&err);
       Move_Back();
       OSTimeDly(2000,OS_OPT_TIME_DLY,&err); 
    }    
    
    
    
}


static void LED_Twinkle(void* p_arg)
{
  	OS_ERR         err;
    (void) p_arg;

    while(1)
    {
        LED1_Toggle();
        OSTimeDly(250,OS_OPT_TIME_DLY,&err);
        LED1_Toggle();
        LED2_Toggle();
        OSTimeDly(250,OS_OPT_TIME_DLY,&err);        
        LED2_Toggle();       
    }
    
    
}



