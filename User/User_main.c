#include "User_main.h"


//��λö��
typedef enum 
{
    UP =0,
    Back=1,
    Left=2,
    Right=3,    
    Stop =4,
}
Diretion;



//������ƿ�
OS_TCB  Key1_Scan_TCB;      //����1��������ƿ�
OS_TCB  USART1_Get_TCB;     //����1���ܵ���Ϣ����
OS_TCB  Key2_Scan_TCB;      //����2���ܵ���Ϣ����
OS_TCB  Run_TCB;      //��������ʱ�������
OS_TCB  LED_Twinkle_TCB;   //LED��˸ʱ�������
OS_TCB Position_TCB;        //�ж�λ�ü��䷽��������
OS_TCB  TaskTurn_TCB;       //����˳��ִ�������


static int8_t Pos_x ,Pos_y;     //��λ����

//��ǰ������ʻ����
Diretion  Car_Dir;




//�ڴ��
OS_MEM   mem;
uint8_t ucArray [ 70 ] [ 4 ];   //�����ڴ������С






void User_main()
{
    OS_ERR      err;

    OSSchedRoundRobinCfg((CPU_BOOLEAN   )DEF_ENABLED,          //ʹ��ʱ��Ƭ��ת����
                        (OS_TICK       )0,                    //�� OSCfg_TickRate_Hz / 10 ��ΪĬ��ʱ��Ƭֵ
                        (OS_ERR       *)&err );               //���ش�������


    OSMemCreate ((OS_MEM      *)&mem,             //ָ���ڴ�������
                 (CPU_CHAR    *)"Mem",   //�����ڴ�������
                 (void        *)ucArray,          //�ڴ�������׵�ַ
                 (OS_MEM_QTY   )70,               //�ڴ�������ڴ����Ŀ
                 (OS_MEM_SIZE  )4,                //�ڴ����ֽ���Ŀ
                 (OS_ERR      *)&err);            //���ش�������    
    
    
    
    OSTaskCreate((OS_TCB     *)&Key1_Scan_TCB,                /* ������ƿ��ַ                               */
                 (CPU_CHAR   *)"����1���",                //��������
                 (OS_TASK_PTR ) Key1_Scan,                   //������
                 (void       *) 0,                              //���ݸ���������ʵ�Σ��β�p_arg��
                 (OS_PRIO     ) Key1_Scan_PRIO,            //��������ȼ�
                 (CPU_STK    *)&Key1_Scan_STK[0],             //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) Key1_Scan_STK_SIZE / 10,   //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) Key1_Scan_STK_SIZE,        //����Ķ�ջ�ռ䣨��λ��size(CPU_STK)��
                 (OS_MSG_QTY  ) 2u,                             //����ɽ��ܵ������Ϣ��
                 (OS_TICK     ) 0u,                             //�����ʱ��Ƭ��������Ĭ��ֵ0��ʾ OS_CFG_TICK_RATE_HZ/10  ��10ms ��
                 (void       *) 0,                              //������չ��0��ʾ����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //����ѡ��
                 (OS_ERR     *)&err);                           //���ش�������

                 
    OSTaskCreate((OS_TCB     *)&USART1_Get_TCB,                /* ������ƿ��ַ                               */
                 (CPU_CHAR   *)"���ڽ���",                //��������
                 (OS_TASK_PTR ) USART1_Get,                   //������
                 (void       *) 0,                              //���ݸ���������ʵ�Σ��β�p_arg��
                 (OS_PRIO     ) USART1_Get_PRIO,            //��������ȼ�
                 (CPU_STK    *)&USART1_Get_STK[0],             //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) USART1_Get_STK_SIZE / 10,   //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) USART1_Get_STK_SIZE,        //����Ķ�ջ�ռ䣨��λ��size(CPU_STK)��
                 (OS_MSG_QTY  ) 2u,                             //����ɽ��ܵ������Ϣ��
                 (OS_TICK     ) 0u,                             //�����ʱ��Ƭ��������Ĭ��ֵ0��ʾ OS_CFG_TICK_RATE_HZ/10  ��10ms ��
                 (void       *) 0,                              //������չ��0��ʾ����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //����ѡ��
                 (OS_ERR     *)&err);                           //���ش������� 
 
                 
    OSTaskCreate((OS_TCB     *)&Key2_Scan_TCB,                /* ������ƿ��ַ                               */
                 (CPU_CHAR   *)"����2���",                //��������
                 (OS_TASK_PTR ) Key2_Scan,                   //������
                 (void       *) 0,                              //���ݸ���������ʵ�Σ��β�p_arg��
                 (OS_PRIO     ) Key2_Scan_PRIO,            //��������ȼ�
                 (CPU_STK    *)&Key2_Scan_STK[0],             //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) Key2_Scan_STK_SIZE / 10,   //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) Key2_Scan_STK_SIZE,        //����Ķ�ջ�ռ䣨��λ��size(CPU_STK)��
                 (OS_MSG_QTY  ) 2u,                             //����ɽ��ܵ������Ϣ��
                 (OS_TICK     ) 0u,                             //�����ʱ��Ƭ��������Ĭ��ֵ0��ʾ OS_CFG_TICK_RATE_HZ/10  ��10ms ��
                 (void       *) 0,                              //������չ��0��ʾ����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //����ѡ��
                 (OS_ERR     *)&err);                           //���ش������� 
    
    OSTaskCreate(&TaskTurn_TCB,"˳��ִ������",TaskTurn,0,TaskTurn_PRIO,&TaskTurn_STK[0],TaskTurn_STK_SIZE/10,TaskTurn_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    

                 
}



static void  Key1_Scan(void *p_arg)
{
	OS_ERR         err;
	CPU_INT16U     version;
	CPU_INT32U     cpu_clk_freq;
	CPU_SR_ALLOC();

	
	(void)p_arg;

	version = OSVersion(&err);                          //��ȡuC/OS�汾��
	
  cpu_clk_freq = BSP_CPU_ClkFreq();                   //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���

	
	while (DEF_TRUE) 
        {                                 
		/* ��������ֱ��KEY1������ */
		OSTaskSemPend ((OS_TICK   )0,                     //�����޵ȴ�
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //����ź��������þ͵ȴ�
                     (CPU_TS   *)0,                     //��ȡ�ź�����������ʱ���
                     (OS_ERR   *)&err);                 //���ش�������
		

		OS_CRITICAL_ENTER();                              //�����ٽ�Σ����⴮�ڴ�ӡ�����

        printf ( "\r\nuC/OS�汾�ţ�V%d.%02d.%02d\r\n",
             version / 10000, version % 10000 / 100, version % 100 );

        printf ( "CPU��Ƶ��%d MHz\r\n", cpu_clk_freq / 1000000 );  


        printf ( "CPUʹ���ʣ�%d.%d%%\r\n",
             OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100 );  

        printf ( "CPU���ʹ���ʣ�%d.%d%%\r\n", 
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

					 
	while (DEF_TRUE) {                                           //������
		/* �������񣬵ȴ�������Ϣ */
		pMsg = OSTaskQPend ((OS_TICK        )0,                    //�����޵ȴ�
                          (OS_OPT         )OS_OPT_PEND_BLOCKING, //û����Ϣ����������
                          (OS_MSG_SIZE   *)&msg_size,            //������Ϣ����
                          (CPU_TS        *)0,                    //������Ϣ��������ʱ���
                          (OS_ERR        *)&err);                //���ش�������

		OS_CRITICAL_ENTER();                                       //�����ٽ�Σ����⴮�ڴ�ӡ�����

		printf ( "%c", * pMsg );                                   //��ӡ��Ϣ����

		OS_CRITICAL_EXIT();                                        //�˳��ٽ��
		
		/* �˻��ڴ�� */
		OSMemPut (  (OS_MEM  *)&mem,                                 //ָ���ڴ�������
                    (void    *)pMsg,                                 //�ڴ����׵�ַ
                    (OS_ERR  *)&err);		                          //���ش�������		
	}       
}


static void  Key2_Scan(void *p_arg)
{
    
 	OS_ERR         err;
    (void) p_arg;
    while(1)
    {
		OSTaskCreate(&LED_Twinkle_TCB,"LED��˸",LED_Twinkle,0,LED_Twinkle_PRIO,&LED_Twinkle_STK[0],LED_Twinkle_STK_SIZE/10,LED_Twinkle_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    
        OSTaskSemPend ((OS_TICK   )0,                     //�����޵ȴ�
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //����ź��������þ͵ȴ�
                     (CPU_TS   *)0,                     //��ȡ�ź�����������ʱ���
                     (OS_ERR   *)&err);                 //���ش������� 
        Car_Dir=UP;
        OSTaskCreate(&Run_TCB,"��������",Run,0,Run_PRIO,&Run_STK[0],Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);    
        OSTaskCreate(&Position_TCB,"�����ж�",Position,0,Position_PRIO,&Position_STK[0],Position_STK_SIZE/10,Position_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err); 
        Move_Up();        
       
        OSTaskDel(&LED_Twinkle_TCB,&err);       //ɾ����˸LED�����
        LED_ALL_OFF();
               
        OSTaskSemPend ((OS_TICK   )0,                     //�����޵ȴ�
                     (OS_OPT    )OS_OPT_PEND_BLOCKING,  //����ź��������þ͵ȴ�
                     (CPU_TS   *)0,                     //��ȡ�ź�����������ʱ���
                     (OS_ERR   *)&err);                 //���ش������� 
        
       OSTaskDel(&Run_TCB,&err);                //ɾ����������
       OSTaskDel(&Position_TCB,&err);           //ɾ���жϷ�λ����
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
     static uint8_t Flag_x,Flag_y;       //�����̵�x��y������    
	char * pMsg;    
    (void) p_arg;
    
    while(1)
    {
		pMsg = OSTaskQPend ((OS_TICK        )0,                    //�����޵ȴ�
                          (OS_OPT         )OS_OPT_PEND_BLOCKING, //û����Ϣ����������
                          (OS_MSG_SIZE   *)NULL,            //������Ϣ����
                          (CPU_TS        *)0,                    //������Ϣ��������ʱ���
                          (OS_ERR        *)&err);                //���ش�������     
        switch (*pMsg)
        {
            case 1:         //ǰ������ 
            {
                Flag_x++;
                break;
            }
            case 2:         //�󴫸���
            {
                Flag_x++;               
                break; 
            }                
            case 3:         //�󴫸���
            {
                Flag_y++;                              
                break;
            }
            case 4:         //�Ҵ�����
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
        
             
		OSMemPut (  (OS_MEM  *)&mem,                                 //ָ���ڴ�������
                    (void    *)pMsg,                                 //�ڴ����׵�ַ
                    (OS_ERR  *)&err);		                          //���ش�������	        
             
    }
    
}







static void TaskTurn(void* p_arg)
{
    
   	OS_ERR     err; 
    static  uint8_t Task_3_7_Time ;
    static uint8_t doTask_Turn;     //����˳��
    (void) p_arg;   
    
    while(1)
    {
      OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,NULL,&err);  //�ȴ������ź�
        
        switch (doTask_Turn)
        {
            case 0 :    //����1
            {
                if(Pos_y==1)
                {
                    Car_Dir=Right;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);      
                    
                }
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;
            }
            case 1 : //����2
            {

                if(Pos_x==6)
                    {
                    //����ͷ��ȡ��ά������
                    Car_Dir=Left;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_x>6)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                
            }
          
            case 2 : //����3
            {
                if(Pos_x>3)
                    {
                    //����ͷʶ��ɫ������
                    //..............
                    Car_Dir=Stop;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    //��е��ץȡ����
                    //..............
                     Car_Dir=Left;     
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);         
                    doTask_Turn++;                        
                    }    
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                
            }
            
            case 3 : //����4
            {
                if(Pos_x==3)
                    {
                    Car_Dir=UP;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_x<3||Pos_x>6)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                
            } 
            
            case 4 : //����5
            {
                if(Pos_y==2)
                    {
                    Car_Dir=Right;      //��ƽ��
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_y>2)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                
            }

            case 5: //����6
            {
//                  if(goal_color_position==now_color_position)
//                  {
//                    Car_Dir=Stop;      //ֹͣ
//                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);  
//                    //��������
//                    //......
//                    Car_Dir=Right;      //����ʻ
//                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);  
//                    doTask_Turn++;                    
//                  } 
//                   else if(Pos_x==5)
//                       OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
//                       
//                   break;
            }
            
            case 6:  //����7
            {
                
                if(Pos_x==5)
                    {
                    Car_Dir=Back;
                    doTask_Turn++;
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);   
                    }    
                else if(Pos_x>5)
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                      
            }
           
            case 7:  //����8
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
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                      
            }
     
            case 8:  //����9
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
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                      
            }
            
            case 9:  //����10
            {
                if(Pos_y==1)
                    {
                    Car_Dir=Left;
                    doTask_Turn++; 
                    OSTaskSemPost(&Run_TCB,OS_OPT_POST_NONE,&err);                          
                    }    
                else 
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                      
            }  
            
            case 10:  //����11
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
                   OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���
                break;                      
            }       

            case 11:  //����11
            {

                doTask_Turn=0;
                Task_3_7_Time=0;
                Pos_x=0;
                Pos_y=0;
                OSTaskSemPost(&Key2_Scan_TCB,OS_OPT_POST_NONE,&err);     //�����Ԥ�ڲ�������Key2���������ź���    
            }      
          
            
        }
    }   
}



















