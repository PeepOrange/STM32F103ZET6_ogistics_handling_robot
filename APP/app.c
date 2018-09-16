/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "config.h"
#include "User_main.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;




/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];





/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


static  void  AppTaskStart  (void *p_arg);




/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


                                               /* Disable all interrupts.                              */

    OSInit(&err);                                               /* ��ʼ��uC/OS-III                                     */
    //������ʼ����
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* ������ƿ��ַ                               */
                 (CPU_CHAR   *)"App Task Start",                //��������
                 (OS_TASK_PTR ) AppTaskStart,                   //������
                 (void       *) 0,                              //���ݸ���������ʵ�Σ��β�p_arg��
                 (OS_PRIO     ) APP_TASK_START_PRIO,            //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],             //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,   //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,        //����Ķ�ջ�ռ䣨��λ��size(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                             //����ɽ��ܵ������Ϣ��
                 (OS_TICK     ) 0u,                             //�����ʱ��Ƭ��������Ĭ��ֵ0��ʾ OS_CFG_TICK_RATE_HZ/10  ��10ms ��
                 (void       *) 0,                              //������չ��0��ʾ����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //����ѡ��
                 (OS_ERR     *)&err);                           //���ش�������

    OSStart(&err);                                              /*�����������������uC/OS-III���� */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 /* ��ʼ��Ӳ��                             */
    CPU_Init();                                                 //��ʼ��CPU�����ʱ��������ж�ʱ���������������

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* ��ȡCPU�ں�ʱ��Ƶ��(Systick����ʱ��)                  */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* �����û������ʱ��Ƶ�ʽ��ļ���Systick��ʱ���Ļ���ֵ                    */
    OS_CPU_SysTickInit(cnts);                                   /* ����Systick��ʱ�����������ֵ              */

    Mem_Init();                                                 /* ��ʼ���ڴ������������ڴ�غ��ڴ�ر�                 */

#if OS_CFG_STAT_TASK_EN > 0u                                    /* ���ʹ��ͳ�������ܣ�Ĭ��ʹ�ܣ�        */
    OSStatTaskCPUUsageInit(&err);  //����û��Ӧ������ֻ�п�����������ʱcpu���������OS_Stat_IdleCtrMax��,Ϊ�������cpuʹ������׼��                           
#endif

    CPU_IntDisMeasMaxCurReset();                                //��λ��ǰ�����ж�ʱ��
    
    User_main();
          
    OSTaskDel(&AppTaskStartTCB,&err);              

}

















