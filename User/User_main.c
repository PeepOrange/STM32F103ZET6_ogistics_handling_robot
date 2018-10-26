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
OS_TCB  Run_TCB;            //ѭ��+PIDģ��
OS_TCB  Postion_TCB;        //��λ����



//��ǰ������ʻ����
Diretion  Car_Dir=Test_Dir;
Steering_engine_angle_Struct  Steering_engine_angle;



//�ڴ��
OS_MEM   mem;
uint8_t ucArray [ 4 ] [ 4 ];   //�����ڴ������С






void User_main()
{
    OS_ERR      err;
    Pos_X=0;
    Pos_Y=0;
    OSSchedRoundRobinCfg((CPU_BOOLEAN   )DEF_ENABLED,          //ʹ��ʱ��Ƭ��ת����
                        (OS_TICK       )0,                    //�� OSCfg_TickRate_Hz / 10 ��ΪĬ��ʱ��Ƭֵ
                        (OS_ERR       *)&err );               //���ش�������


    OSMemCreate ((OS_MEM      *)&mem,             //ָ���ڴ�������
                 (CPU_CHAR    *)"Mem",   //�����ڴ�������
                 (void        *)ucArray,          //�ڴ�������׵�ַ
                 (OS_MEM_QTY   )4,               //�ڴ�������ڴ����Ŀ
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






static void    Key1_Scan(void *p_arg)
{
    OS_ERR      err;

    while(1)
    {
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
    OSTaskCreate(&Run_TCB,"Ѱ��",Run,0,Run_PRIO,Run_STK,Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);  
    OSTaskCreate(&Postion_TCB,"��λ",Postion,0,Postion_PRIO,Postion_STK,Postion_STK_SIZE/10,Postion_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);  
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















