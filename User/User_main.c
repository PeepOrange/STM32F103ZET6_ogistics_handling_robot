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





//��ǰ������ʻ����
Diretion  Car_Dir;




//�ڴ��
OS_MEM   mem;
uint8_t ucArray [ 4 ] [ 4 ];   //�����ڴ������С






void User_main()
{
    OS_ERR      err;

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
  
    OSTaskCreate(&Run_TCB,"Ѱ��",Run,0,Run_PRIO,Run_STK,Run_STK_SIZE/10,Run_STK_SIZE,2,0,0,(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),&err);  
                 
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
 	CPU_SR_ALLOC();   
    while(1)
    {
      OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
      OS_CRITICAL_ENTER();                             
        
      printf ( "CPUʹ���ʣ�%d.%d%%\r\n",
             OSStatTaskCPUUsage / 100, OSStatTaskCPUUsage % 100 );  

      printf ( "CPU���ʹ���ʣ�%d.%d%%\r\n", 
                 OSStatTaskCPUUsageMax / 100, OSStatTaskCPUUsageMax % 100 );        
      
     printf ("\n\r��ǰ�ֵ�Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",LeftUp_PID_Mortor.Kp,LeftUp_PID_Mortor.Ki,LeftUp_PID_Mortor.Kd); 
     printf ("��ǰ�ֵ�Ŀ��ֵ:%.2f   ��ȡֵ:%.2f \r\n",LeftUp_PID_Mortor.goal_point,LeftUp_PID_Mortor.read_point);    
     printf ("��ǰ�ֵ��ϴ����:%.2f \r\n",LeftUp_PID_Mortor.last_Error);    

     printf ("\n\r����ֵ�Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",LeftBack_PID_Mortor.Kp,LeftBack_PID_Mortor.Ki,LeftBack_PID_Mortor.Kd); 
     printf ("����ֵ�Ŀ��ֵ:%.2f   ��ȡֵ:%.2f \r\n",LeftBack_PID_Mortor.goal_point,LeftBack_PID_Mortor.read_point); 
     printf ("����ֵ��ϴ����:%.2f \r\n",LeftBack_PID_Mortor.last_Error);   
        
     printf ("\n\r��ǰ�ֵ�Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",RightUp_PID_Mortor.Kp,RightUp_PID_Mortor.Ki,RightUp_PID_Mortor.Kd); 
     printf ("��ǰ�ֵ�Ŀ��ֵ:%.2f   ��ȡֵ:%.2f \r\n",RightUp_PID_Mortor.goal_point,RightUp_PID_Mortor.read_point);       
     printf ("��ǰ�ֵ��ϴ����:%.2f \r\n",RightUp_PID_Mortor.last_Error);  
        
     printf ("\n\r�Һ��ֵ�Kp:%.2f  Ki:%.2f  Kd:%.2f\r\n",RightBack_PID_Mortor.Kp,RightBack_PID_Mortor.Ki,RightBack_PID_Mortor.Kd); 
     printf ("�Һ��ֵ�Ŀ��ֵ:%.2f   ��ȡֵ:%.2f \r\n",RightBack_PID_Mortor.goal_point,RightBack_PID_Mortor.read_point); 
     printf ("�Һ��ֵ��ϴ����:%.2f \r\n",RightBack_PID_Mortor.last_Error);  


      OS_CRITICAL_EXIT();   

    }
    
}




static void    Run(void *p_arg)
{
    OS_ERR      err;
    while(1)
    {
     OSTaskSemPend (0,OS_OPT_PEND_BLOCKING,NULL,&err);
   
     if(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)||!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3))     //ǰ��  ��ǰ      �͵�ƽ������⵽���ߣ�
     {
         RightUp_PWM=Correct_Up_PWM;
         RightBack_PWM=Correct_Up_PWM;
         LeftUp_PWM=Correct_Back_PWM;
         LeftBack_PWM=Correct_Back_PWM;
     }

     else if(!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)||!GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5))    //ǰ��  �Һ�
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























