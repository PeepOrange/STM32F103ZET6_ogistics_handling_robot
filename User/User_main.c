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
OS_TCB  Key2_Scan_TCB;      //按键2接受到信息任务
OS_TCB  Run_TCB;      //车子运行时的任务块
OS_TCB  LED_Twinkle_TCB;   //LED闪烁时的任务快
OS_TCB Position_TCB;        //判断位置及其方向的任务块
OS_TCB  TaskTurn_TCB;       //任务顺序执行任务快


static int8_t Pos_x ,Pos_y;     //方位坐标

//当前车子行驶方向
Diretion  Car_Dir;




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
    
    OSTaskCreate(&TaskTurn_TCB,"顺序执行任务",TaskTurn,0,TaskTurn_PRIO,&TaskTurn_STK[0],TaskTurn_STK_SIZE/10,TaskTurn_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    

                 
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
        Car_Dir=UP;
        OSTaskCreate(&Run_TCB,"车子运行",Run,0,Run_PRIO,&Run_STK[0],Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    
        OSTaskCreate(&Position_TCB,"方向判断",Position,0,Position_PRIO,&Position_STK[0],Position_STK_SIZE/10,Position_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err); 
        Move_Up();        
       
        OSTaskDel(&LED_Twinkle_TCB,&err);       //删除闪烁LED任务块
        LED_ALL_OFF();
               
        OSTaskSemPend ((OS_TICK   )0,                     //无期限等待
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果信号量不可用就等待
                     (CPU_TS   *)0,                     //获取信号量被发布的时间戳
                     (OS_ERR   *)&err);                 //返回错误类型 
        
       OSTaskDel(&Run_TCB,&err);                //删除奔跑任务
       OSTaskDel(&Position_TCB,&err);           //删除判断方位任务
       Move_Stop();
       Car_Dir=Stop;

    }
}


static void Run(void* p_arg)
{
  	OS_ERR     err;
    (void) p_arg;
    while(1)
    {
    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,NULL,&err);
        switch(Car_Dir)
        {
            case UP : Move_Up(); break;
            case Back : Move_Back(); break;
            case Left:  Move_Left(); break;
            case Right : Move_Right(); break;
            case Stop :  Move_Stop(); break;
        }    
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

static void Position(void* p_arg)
{
  	OS_ERR     err;
     static uint8_t Flag_x,Flag_y;       //在棋盘的x、y点坐标    
	char * pMsg;    
    (void) p_arg;
    
    while(1)
    {
		pMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
                          (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
                          (OS_MSG_SIZE   *)NULL,            //返回消息长度
                          (CPU_TS        *)0,                    //返回消息被发布的时间戳
                          (OS_ERR        *)&err);                //返回错误类型     
        switch (*pMsg)
        {
            case 1:         //前传感器 
            {
                Flag_x++;
                break;
            }
            case 2:         //后传感器
            {
                Flag_x++;               
                break; 
            }                
            case 3:         //左传感器
            {
                Flag_y++;                              
                break;
            }
            case 4:         //右传感器
            {
                Flag_y++;                              
                break;
            }
            default :
                break;
        }
        
        if(Flag_y==2)
        {
        switch(Car_Dir)
        {
            case UP : Pos_y++; break;
            case Back : Pos_y--; break;
            case Left:  
            case Right :
            case Stop: break;
        }    
            
            Flag_y=0;
            Flag_x=0;
        }
        
        
        if(Flag_x==2)
        {
        switch(Car_Dir)
        {

            case Left:   Pos_x--; break;
            case Right : Pos_x++; break;
            case UP :  
            case Back :  
            case Stop: break;
        }    
            
            Flag_y=0;
            Flag_x=0;
        }
        
             
		OSMemPut (  (OS_MEM  *)&mem,                                 //指向内存管理对象
                    (void    *)pMsg,                                 //内存块的首地址
                    (OS_ERR  *)&err);		                          //返回错误类型	        
             
    }
    
}







static void TaskTurn(void* p_arg)
{
    
   	OS_ERR     err; 
    static  uint8_t Task_3_7_Time ;
    static uint8_t doTask_Turn;     //任务顺序
    (void) p_arg;   
    
    while(1)
    {
      OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,NULL,&err);  //等待任务信号
        
        switch (doTask_Turn)
        {
            case 0 :    //任务1
            {
                if(Pos_y==1)
                {
                    Car_Dir=Right;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);      
                    
                }
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;
            }
            case 1 : //任务2
            {

                if(Pos_x==6)
                    {
                    //摄像头采取二维码任务
                    Car_Dir=Left;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_x>6)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                
            }
          
            case 2 : //任务3
            {
                if(Pos_x>3)
                    {
                    //摄像头识别色块任务
                    //..............
                    Car_Dir=Stop;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    //机械臂抓取任务
                    //..............
                     Car_Dir=Left;     
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);         
                    doTask_Turn++;                        
                    }    
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                
            }
            
            case 3 : //任务4
            {
                if(Pos_x==3)
                    {
                    Car_Dir=UP;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_x<3||Pos_x>6)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                
            } 
            
            case 4 : //任务5
            {
                if(Pos_y==2)
                    {
                    Car_Dir=Right;      //右平移
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_y>2)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                
            }

            case 5: //任务6
            {
//                  if(goal_color_position==now_color_position)
//                  {
//                    Car_Dir=Stop;      //停止
//                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);  
//                    //放置任务
//                    //......
//                    Car_Dir=Right;      //右行驶
//                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);  
//                    doTask_Turn++;                    
//                  } 
//                   else if(Pos_x==5)
//                       OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
//                       
//                   break;
            }
            
            case 6:  //任务7
            {
                
                if(Pos_x==5)
                    {
                    Car_Dir=Back;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_x>5)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                      
            }
           
            case 7:  //任务8
            {
                
                if(Pos_y==1)
                    {
                        if(Task_3_7_Time==0)
                        {
                        Task_3_7_Time++;
                        doTask_Turn-=5;
                        }
                        else
                        {
                         doTask_Turn++; 
                        }
                     Car_Dir=Left;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);                          
                    }    
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                      
            }
     
            case 8:  //任务9
            {
                
                if(Pos_y==1)
                    {
                        if(Task_3_7_Time==1)
                        {
                        Task_3_7_Time++;
                        doTask_Turn-=5;
                        }
                        else
                        {
                         doTask_Turn++; 
                        }
                     Car_Dir=Left;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);                          
                    }    
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                      
            }
            
            case 9:  //任务10
            {
                if(Pos_y==1)
                    {
                    Car_Dir=Left;
                    doTask_Turn++; 
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);                          
                    }    
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                      
            }  
            
            case 10:  //任务11
            {
                if(Pos_x==1)
                    {
                    Car_Dir=Left;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err); 
                    OSTimeDly(Correct_Move_Time,OS_OPT_TIME_DLY,&err);
                    Car_Dir=Back;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);  
                    OSTimeDly(Correct_Move_Time,OS_OPT_TIME_DLY,&err);
                    Car_Dir=Stop;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);  
                    doTask_Turn++; 
                    }    
                else if(Pos_x>5)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量
                break;                      
            }       

            case 11:  //任务11
            {

                doTask_Turn=0;
                Task_3_7_Time=0;
                Pos_x=0;
                Pos_y=0;
                OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //如果和预期不符，给Key2发送任务信号量    
            }      
          
            
        }
    }   
}



















