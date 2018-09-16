/*
***********************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TICK MANAGEMENT
*
* File    : OS_TICK.C
* By      : JJL
* Version : V3.03.01
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or 
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your 
*           application/product.   We provide ALL the source code for your convenience and to help you 
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use 
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
************************************************************************************************************************
*/

#define  MICRIUM_SOURCE
#include <os.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *os_tick__c = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                  LOCAL PROTOTYPES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                      TICK TASK
*
* Description: This task is internal to uC/OS-III and is triggered by the tick interrupt.
*
* Arguments  : p_arg     is an argument passed to the task when the task is created (unused).
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TickTask (void  *p_arg)
{
    OS_ERR  err;
    CPU_TS  ts;
    (void) p_arg;

    while (DEF_ON) {                                        //ѭ������
        (void)OSTaskSemPend((OS_TICK  )0,                   //�ȴ�����Systick�жϷ��͵��ź��������յ��ź������������
                            (OS_OPT   )OS_OPT_PEND_BLOCKING,
                            (CPU_TS  *)&ts,
                            (OS_ERR  *)&err);              
        if (err == OS_ERR_NONE) {                           //������յ��ź����޴���
            if (OSRunning == OS_STATE_OS_RUNNING) {         //���ϵͳ��������
                OS_TickListUpdate();                        /* ������������ĵȴ�ʱ�䣨����ʱ����ʱ�ȣ�                    */
            }
        }
    }
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                 INITIALIZE TICK TASK
*
* Description: This function is called by OSInit() to create the tick task.
*
* Arguments  : p_err   is a pointer to a variable that will hold the value of an error code:
*
*                          OS_ERR_TICK_STK_INVALID   if the pointer to the tick task stack is a NULL pointer
*                          OS_ERR_TICK_STK_SIZE      indicates that the specified stack size
*                          OS_ERR_PRIO_INVALID       if the priority you specified in the configuration is invalid
*                                                      (There could be only one task at the Idle Task priority)
*                                                      (Maybe the priority you specified is higher than OS_CFG_PRIO_MAX-1
*                          OS_ERR_??                 other error code returned by OSTaskCreate()
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TickTaskInit (OS_ERR  *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

    OSTickCtr         = (OS_TICK)0u;                        /* Clear the tick counter                                 */

    OSTickTaskTimeMax = (CPU_TS)0u;


    OS_TickListInit();                                      /* Initialize the tick list data structures               */

                                                            /* ---------------- CREATE THE TICK TASK ---------------- */
    if (OSCfg_TickTaskStkBasePtr == (CPU_STK *)0) {
       *p_err = OS_ERR_TICK_STK_INVALID;
        return;
    }

    if (OSCfg_TickTaskStkSize < OSCfg_StkSizeMin) {
       *p_err = OS_ERR_TICK_STK_SIZE_INVALID;
        return;
    }

    if (OSCfg_TickTaskPrio >= (OS_CFG_PRIO_MAX - 1u)) {     /* Only one task at the 'Idle Task' priority              */
       *p_err = OS_ERR_TICK_PRIO_INVALID;
        return;
    }

    OSTaskCreate((OS_TCB     *)&OSTickTaskTCB,
                 (CPU_CHAR   *)((void *)"uC/OS-III Tick Task"),
                 (OS_TASK_PTR )OS_TickTask,
                 (void       *)0,
                 (OS_PRIO     )OSCfg_TickTaskPrio,
                 (CPU_STK    *)OSCfg_TickTaskStkBasePtr,
                 (CPU_STK_SIZE)OSCfg_TickTaskStkLimit,
                 (CPU_STK_SIZE)OSCfg_TickTaskStkSize,
                 (OS_MSG_QTY  )0u,
                 (OS_TICK     )0u,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_NO_TLS),
                 (OS_ERR     *)p_err);
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                               INITIALIZE THE TICK LIST
*
* Description: This function initializes the tick handling data structures of uC/OS-III.
*
* Arguments  : none
*
* Returns    : None
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_TickListInit (void)
{
    OS_TICK_SPOKE_IX   i;
    OS_TICK_SPOKE     *p_spoke;



    for (i = 0u; i < OSCfg_TickWheelSize; i++) {
        p_spoke                = (OS_TICK_SPOKE *)&OSCfg_TickWheel[i];
        p_spoke->FirstPtr      = (OS_TCB        *)0;
        p_spoke->NbrEntries    = (OS_OBJ_QTY     )0u;
        p_spoke->NbrEntriesMax = (OS_OBJ_QTY     )0u;
    }
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                ADD TASK TO TICK LIST
*
* Description: This function is called to place a task in a list of task waiting for either time to expire or waiting to
*              timeout on a pend call.
*
* Arguments  : p_tcb          is a pointer to the OS_TCB of the task to add to the tick list
*              -----
*
*              time           represents either the 'match' value of OSTickCtr or a relative time from the current
*                             value of OSTickCtr as specified by the 'opt' argument..
*
*                             relative when 'opt' is set to OS_OPT_TIME_DLY
*                             relative when 'opt' is set to OS_OPT_TIME_TIMEOUT
*                             match    when 'opt' is set to OS_OPT_TIME_MATCH
*                             periodic when 'opt' is set to OS_OPT_TIME_PERIODIC
*
*              opt            is an option specifying how to calculate time.  The valid values are:
*              ---
*                                 OS_OPT_TIME_DLY
*                                 OS_OPT_TIME_TIMEOUT
*                                 OS_OPT_TIME_PERIODIC
*                                 OS_OPT_TIME_MATCH
*
*              p_err          is a pointer to a variable that will contain an error code returned by this function.
*              -----
*                                 OS_ERR_NONE           the call was successful and the time delay was scheduled.
*                                 OS_ERR_TIME_ZERO_DLY  if delay is zero or already occurred.
*
* Returns    : None
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) This function is assumed to be called with interrupts disabled.
************************************************************************************************************************
*/

void  OS_TickListInsert (OS_TCB   *p_tcb,                   //������ƿ�
                         OS_TICK   time,                    //��ʱʱ��
                         OS_OPT    opt,                     //��ʱѡ��
                         OS_ERR   *p_err)                   //��������
{
    OS_TICK            tick_delta;
    OS_TICK            tick_next;
    OS_TICK_SPOKE     *p_spoke;
    OS_TCB            *p_tcb0;
    OS_TCB            *p_tcb1;
    OS_TICK_SPOKE_IX   spoke;



    if (opt == OS_OPT_TIME_MATCH) {                              /* ������ʱ                            */
        tick_delta = time - OSTickCtr - 1u;                     //����ʣ����ʱʱ��
        if (tick_delta > OS_TICK_TH_RDY) {                       /* �����ʱʱ�䳬���������                   */
            p_tcb->TickCtrMatch = (OS_TICK        )0u;          //�������ʱ�ӽ�����0
            p_tcb->TickRemain   = (OS_TICK        )0u;          //���������ʱʣ��ʱ�ӽ�����0
            p_tcb->TickSpokePtr = (OS_TICK_SPOKE *)0;           //���񲻲�������б�
           *p_err               =  OS_ERR_TIME_ZERO_DLY;         /* ��������Ϊ0��ʱ                                */
            return;                                             //���أ���ִ����ʱ����
        }
        p_tcb->TickCtrMatch = time;                             //����ƥ���Ϊ OSTickCtr =time
        p_tcb->TickRemain   = tick_delta + 1u;                  //���������뵽�ڵ�ʣ��ʱ��

    } else if (time > (OS_TICK)0u) {                            //���time>0
        if (opt == OS_OPT_TIME_PERIODIC) {                       /* �����������ʱ                           */
            tick_next  = p_tcb->TickCtrPrev + time;             //�������������Ҫƥ���ʱ�ӽ����ܼ���
            tick_delta = tick_next - OSTickCtr - 1u;            //����������ƥ�仹�ж೤ʱ��
            if (tick_delta < time) {                             /* �ж�TickCtrPrev �� OSTickCtr+1 ��С */
                p_tcb->TickCtrMatch = tick_next;                 /* �� p_tcb->TickCtrPrev + time ��Ϊ����ƥ���    */
            } else {
                p_tcb->TickCtrMatch = OSTickCtr + time;          /*�� OSTickCtr + time ��Ϊ����ƥ���                    */
            }
            p_tcb->TickRemain   = p_tcb->TickCtrMatch - OSTickCtr;  //���������뵽�ڻ��ж೤ʱ��
            p_tcb->TickCtrPrev  = p_tcb->TickCtrMatch;          //���浱ǰƥ��ֵΪ��һ������ʱ��

        } else {                                                 /* ����������ʱ                 */
            p_tcb->TickCtrMatch = OSTickCtr + time;             //����ȴ���ƥ���Ϊ OSTickCtr +time
            p_tcb->TickRemain   = time;                         //���������뵽�ڵ�ʱ�䣨��time��
        }

    } else {                                                     /* ���time==0.                              */
        p_tcb->TickCtrMatch = (OS_TICK        )0u;              //ʱ�ӽ���ƥ�������0
        p_tcb->TickRemain   = (OS_TICK        )0u;              //������ʱʣ���������0
        p_tcb->TickSpokePtr = (OS_TICK_SPOKE *)0;               //�����񲻲�����Ķ���
       *p_err               =  OS_ERR_TIME_ZERO_DLY;             /*�������͡�0��ʱ��                              */
        return;             
    }


    spoke   = (OS_TICK_SPOKE_IX)(p_tcb->TickCtrMatch % OSCfg_TickWheelSize); //����Hash�㷨��ȡ�ࣩ������������� OSCfg_TickWheel �������λ�ã�����½����б����Ӧ����������ҵ�������
    p_spoke = &OSCfg_TickWheel[spoke];                                      //��ȡҪ�����������б�

    if (p_spoke->NbrEntries == (OS_OBJ_QTY)0u) {                 /* �����ǰ�����б�Ϊ��                        */
        p_tcb->TickNextPtr   = (OS_TCB   *)0;                   //������ָ������б����һ������ָ����0
        p_tcb->TickPrevPtr   = (OS_TCB   *)0;                   //������ָ������б����һ������ָ����0
        p_spoke->FirstPtr    =  p_tcb;                          //��ǰ������Ϊ�����б��һ������
        p_spoke->NbrEntries  = (OS_OBJ_QTY)1u;                  //�����б�Ԫ����Ŀ��1
    } else {                                                    //��������б�ǿ�
        p_tcb1     = p_spoke->FirstPtr;                          /* ��ȡ�б�ĵ�һ����          */
        while (p_tcb1 != (OS_TCB *)0) {                         //������������
            p_tcb1->TickRemain = p_tcb1->TickCtrMatch            /* ����������ʣ��ʱ��     */
                               - OSTickCtr;
            if (p_tcb->TickRemain > p_tcb1->TickRemain) {        /* �����ǰ����ʣ��ʱ����ڸ�����   */
                if (p_tcb1->TickNextPtr != (OS_TCB *)0) {        /* ��������������һ������*/
                    p_tcb1               =  p_tcb1->TickNextPtr; /*�õ�ǰ�����������������һ������Ƚ�               */
                } else {                                        //������������б����һ������
                    p_tcb->TickNextPtr   = (OS_TCB *)0;         //���������һ������ָ��Ϊ�գ���������Ϊ�б����һ������
                    p_tcb->TickPrevPtr   =  p_tcb1;             //������Ϊ�ǵ�ǰ�����ǰһλ
                    p_tcb1->TickNextPtr  =  p_tcb;               /* ��ǰ�����Ǹ������һλ*/
                    p_tcb1               = (OS_TCB *)0;          /* ������ɣ��˳�whileѭ��                                       */
                }
            } else {                                             /* �����ǰ����ʣ��ʱ��С�ڻ���ڸ�����                   */
                if (p_tcb1->TickPrevPtr == (OS_TCB *)0) {        /* ������������б�ĵ�һ��Ԫ��         */
                    p_tcb->TickPrevPtr   = (OS_TCB *)0;            //���������Ϊ�б�ĵ�һ��Ԫ��
                    p_tcb->TickNextPtr   =  p_tcb1;             //�������ǵ�ǰ����ĺ�һλ
                    p_tcb1->TickPrevPtr  =  p_tcb;              //�������ǰһλ�ǵ�ǰ����
                    p_spoke->FirstPtr    =  p_tcb;              //��ǰ�������б�ĵ�һλ����
                } else {                                         /* ������������б�ĵ�һԪ��      */
                    p_tcb0               =  p_tcb1->TickPrevPtr;    //p_tcb0��ʱ����������ǰһλ����
                    p_tcb->TickPrevPtr   =  p_tcb0;                 //��ǰ�����ǰһλ�����Ǹ������ǰһλ����
                    p_tcb->TickNextPtr   =  p_tcb1;                 //��ǰ����ĺ�һ�������Ǹ�����
                    p_tcb0->TickNextPtr  =  p_tcb;                  //�������ǰһ������ĺ�һ�������ǵ�ǰ����
                    p_tcb1->TickPrevPtr  =  p_tcb;                  //�������ǰһ������Ҳ��Ϊ��ǰ����
                }
                p_tcb1 = (OS_TCB *)0;                            /* ������ɣ��˳�ѭ��                                      */
            }
        }
        p_spoke->NbrEntries++;                                  //�����б�Ԫ�ؼ�
    }
    if (p_spoke->NbrEntriesMax < p_spoke->NbrEntries) {          /* ���½����б������Ŀ  */
        p_spoke->NbrEntriesMax = p_spoke->NbrEntries;
    }
    p_tcb->TickSpokePtr = p_spoke;                               /* ��¼��ǰ��������ĸ������б�                           */
   *p_err               = OS_ERR_NONE;                              //��¼��������Ϊ�޴���
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                         REMOVE A TASK FROM THE TICK LIST
*
* Description: This function is called to remove a task from the tick list
*
* Arguments  : p_tcb          Is a pointer to the OS_TCB to remove.
*              -----
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
*
*              2) This function is assumed to be called with interrupts disabled.
************************************************************************************************************************
*/

void  OS_TickListRemove (OS_TCB  *p_tcb)                        //������ӽ����б��Ƴ�
{
    OS_TICK_SPOKE  *p_spoke;
    OS_TCB         *p_tcb1;
    OS_TCB         *p_tcb2;



    p_spoke = p_tcb->TickSpokePtr;                                    //��ȡ�������ĸ������б�  
    if (p_spoke != (OS_TICK_SPOKE *)0) {                              /* �������ȷʵ�ڽ����б�            */
        p_tcb->TickRemain = (OS_TICK)0u;                                //��������ӳ�ʣ�������0
        if (p_spoke->FirstPtr == p_tcb) {                             /* �������Ϊ�����б��һ������ */
            p_tcb1            = (OS_TCB *)p_tcb->TickNextPtr;         /*  ��ȡ��������һ������Ϊtcb1                                        */
            p_spoke->FirstPtr = p_tcb1;                                 //�����б�ĵ�һ������Ϊtcb1
            if (p_tcb1 != (OS_TCB *)0) {                               //���tcb1����
                p_tcb1->TickPrevPtr = (OS_TCB *)0;                      //tcb1��ǰһ������Ϊ�գ���tcb1λ�����б�ĵ�һ����
            } 
        } else {                                                     /*�������Ϊ�����б�ĵ�һ������   */  
            p_tcb1              = p_tcb->TickPrevPtr;                   //��ȡ�������ǰһ������Ϊtcb1
            p_tcb2              = p_tcb->TickNextPtr;                   //��ȡ������ĺ�һ������Ϊtcb2
            p_tcb1->TickNextPtr = p_tcb2;                               //tcb1�ĺ�һ������Ϊtcb2
            if (p_tcb2 != (OS_TCB *)0) {                                //���tcb��Ϊ��
                p_tcb2->TickPrevPtr = p_tcb1;                           //tcb2��ǰһ������Ϊtcb1
            }
        }
        p_tcb->TickNextPtr  = (OS_TCB        *)0;                        //�����������һ���������  
        p_tcb->TickPrevPtr  = (OS_TCB        *)0;                       //���������ǰһ���������
        p_tcb->TickSpokePtr = (OS_TICK_SPOKE *)0;                       //���������κ�һ�������б�
        p_tcb->TickCtrMatch = (OS_TICK        )0u;                      //�������ƥ��������
        p_spoke->NbrEntries--;                                          //�����б���Ŀ-1
    }
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                              RESET TICK LIST PEAK DETECTOR
*
* Description: This function is used to reset the peak detector for the number of entries in each spoke.
*
* Arguments  : void
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/


void  OS_TickListResetPeak (void)
{
    OS_TICK_SPOKE_IX   i;
    OS_TICK_SPOKE     *p_spoke;



    for (i = 0u; i < OSCfg_TickWheelSize; i++) {
        p_spoke                = (OS_TICK_SPOKE *)&OSCfg_TickWheel[i];
        p_spoke->NbrEntriesMax = (OS_OBJ_QTY     )0u;
    }
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                UPDATE THE TICK LIST
*
* Description: This function is called when a tick occurs and determines if the timeout waiting for a kernel object has
*              expired or a delay has expired.
*
* Arguments  : non
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_TickListUpdate (void)
{
    CPU_BOOLEAN        done;
    OS_TICK_SPOKE     *p_spoke;
    OS_TCB            *p_tcb;
    OS_TCB            *p_tcb_next;
    OS_TICK_SPOKE_IX   spoke;
    CPU_TS             ts_start;
    CPU_TS             ts_end;
    CPU_SR_ALLOC(); //ʹ�õ��ٽ��ʱ����ʹ�øú궨�壬�ú������Ͷ���һ���ֲ����������ڱ�����ж�ǰ��CPU״̬�Ĵ���SR (�ٽ�ι��ж�ֻ�豣��SR),�뿪�ж�ʱ����ֵ��ԭ


    OS_CRITICAL_ENTER();                                                //�����ٽ��
    ts_start = OS_TS_GET();                                             //��ȡOS_TickTask()�������ʼʱ���
    OSTickCtr++;                                                       /* ʱ�ӽ���+1         */
    spoke    = (OS_TICK_SPOKE_IX)(OSTickCtr % OSCfg_TickWheelSize);     //ʹ�ù�ϣ�㷨(ȡ��)������������� OSCFg_TickWheel�����ĸ�Ԫ�أ������б�������ҵ�������ʱ��Ӧ���������
    p_spoke  = &OSCfg_TickWheel[spoke];                                 //�ĸ�Ԫ�أ�һ�������б����������������ʱ��Ӧ������ֻ�������б�
    p_tcb    = p_spoke->FirstPtr;                                       //��ȡ�����б��׸�������ƿ�ĵ�ַ
    done     = DEF_FALSE;
    while (done == DEF_FALSE) {                                         //���û�ҵ���һֱѰ��
        if (p_tcb != (OS_TCB *)0) {                                     //������񲻴���
            p_tcb_next = p_tcb->TickNextPtr;                           // ��ȡ���б��и�������һ��������ƿ�ĵ�ַ
            switch (p_tcb->TaskState) {
                case OS_TASK_STATE_RDY:
                case OS_TASK_STATE_PEND:
                case OS_TASK_STATE_SUSPENDED:
                case OS_TASK_STATE_PEND_SUSPENDED:
                     break;

                case OS_TASK_STATE_DLY:                                 //�������ʱ״̬
                     p_tcb->TickRemain = p_tcb->TickCtrMatch           /* ������ʱ��ʣ��ʱ��      */
                                       - OSTickCtr;
                     if (OSTickCtr == p_tcb->TickCtrMatch) {           /* �����������             */
                         p_tcb->TaskState = OS_TASK_STATE_RDY;          /* �޸ĸ�����״̬��Ϊ����̬                     */
                         OS_TaskRdy(p_tcb);                              //���������   
                     } else {
                         done             = DEF_TRUE;                  /* ���������δ��������������������У��϶�Ҳδ�������˳�while()ѭ��     */
                     }
                     break;

                case OS_TASK_STATE_PEND_TIMEOUT:                        //����������޵ȴ�״̬
                     p_tcb->TickRemain = p_tcb->TickCtrMatch           /* �������޵�ʣ��ʱ��       */
                                       - OSTickCtr;
                     if (OSTickCtr == p_tcb->TickCtrMatch) {           /* �����������              */
#if (OS_MSG_EN > 0u)                                                    //���ʹ������Ϣ���У���ͨ��Ϣ���л�������Ϣ���У�
                         p_tcb->MsgPtr     = (void      *)0;            //�����񱣴���յ�����Ϣ�� ��ַ��Ա ���
                         p_tcb->MsgSize    = (OS_MSG_SIZE)0u;           //�����񱣴���յ�����Ϣ�� ���ȳ�Ա ���
#endif
                         p_tcb->TS         = OS_TS_GET();               //��¼��������ȴ���ʱ���
                         OS_PendListRemove(p_tcb);                     /* �ӵȴ��б��Ƴ�������                      */
                         OS_TaskRdy(p_tcb);                             //���������
                         p_tcb->TaskState  = OS_TASK_STATE_RDY;         //�޸�����״̬Ϊ����̬
                         p_tcb->PendStatus = OS_STATUS_PEND_TIMEOUT;   /* ��¼�ȴ�״̬Ϊ��ʱ                     */
                         p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;  /* ��¼�ȴ��ں˶������Ϊ��                 */
                     } else {
                         done              = DEF_TRUE;                 /* ���������δ��������������������У��϶�Ҳδ�������˳�while()ѭ��            */
                     }
                     break;

                case OS_TASK_STATE_DLY_SUSPENDED:                       //�������ʱ�б������״̬
                     p_tcb->TickRemain = p_tcb->TickCtrMatch           /* ������ʱʣ��ʱ��      */
                                       - OSTickCtr;
                     if (OSTickCtr == p_tcb->TickCtrMatch) {           /* �����������              */
                         p_tcb->TaskState  = OS_TASK_STATE_SUSPENDED;   //�޸�����״̬��Ϊ������״̬
                         OS_TickListRemove(p_tcb);                     /* �ӽ����б��Ƴ�����             */
                     } else {
                         done              = DEF_TRUE;                 /* ���������δ��������������������У��϶�Ҳδ�������˳�while()ѭ��            */
                     }
                     break;

                case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:              //����������޵ȴ��б������״̬
                     p_tcb->TickRemain = p_tcb->TickCtrMatch           /* �������޵�ʣ��ʱ��      */
                                       - OSTickCtr;
                     if (OSTickCtr == p_tcb->TickCtrMatch) {           /* �����������             */
#if (OS_MSG_EN > 0u)                                                     //���ʹ������Ϣ���У���ͨ��Ϣ���л�������Ϣ���У�
                         p_tcb->MsgPtr     = (void      *)0;            //�����񱣴���յ�����Ϣ�� ��ַ��Ա ���
                         p_tcb->MsgSize    = (OS_MSG_SIZE)0u;            //�����񱣴���յ�����Ϣ�� ���ȳ�Ա ���
#endif
                         p_tcb->TS         = OS_TS_GET();               //��¼��������ȴ���ʱ���
                         OS_PendListRemove(p_tcb);                     /*�ӵȴ��б��Ƴ�������                     */
                         OS_TickListRemove(p_tcb);                     /* �ӽ����б��Ƴ�������            */
                         p_tcb->TaskState  = OS_TASK_STATE_SUSPENDED;   //�޸�����״̬��Ϊ����״̬
                         p_tcb->PendStatus = OS_STATUS_PEND_TIMEOUT;   /* ��¼�ȴ�״̬Ϊ��ʱ                    */
                         p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;  /* ��¼�ȴ��ں˶������Ϊ��                */
                     } else {
                         done              = DEF_TRUE;                 /* ���������δ��������������������У��϶�Ҳδ�������˳�while()ѭ��            */
                     }
                     break;

                default:
                     break;
            }
            p_tcb = p_tcb_next;                                         //����������һ������
        } else {
            done  = DEF_TRUE;                                           //������񲻴��ڣ��˳�ѭ��
        }
    }
    ts_end = OS_TS_GET() - ts_start;                                   /* ��ȡOS_TickTask()����Ľ���ʱ��� ��������ִ��ʱ��      */
    if (OSTickTaskTimeMax < ts_end) {                                   
        OSTickTaskTimeMax = ts_end;                                     //����OS_TickTask()�����������ʱ��
    }
    OS_CRITICAL_EXIT();                                                 //�˳��ٽ��
}
