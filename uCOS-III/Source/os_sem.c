/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                 SEMAPHORE MANAGEMENT
*
* File    : OS_SEM.C
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
const  CPU_CHAR  *os_sem__c = "$Id: $";
#endif


#if OS_CFG_SEM_EN > 0u
/*
************************************************************************************************************************
*                                                  CREATE A SEMAPHORE
*
* Description: This function creates a semaphore.
*
* Arguments  : p_sem         is a pointer to the semaphore to initialize.  Your application is responsible for
*                            allocating storage for the semaphore.
*
*              p_name        is a pointer to the name you would like to give the semaphore.
*
*              cnt           is the initial value for the semaphore.
*                            If used to share resources, you should initialize to the number of resources available.
*                            If used to signal the occurrence of event(s) then you should initialize to 0.
*
*              p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE                    if the call was successful
*                                OS_ERR_CREATE_ISR              if you called this function from an ISR
*                                OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the semaphore after you
*                                                                 called OSSafetyCriticalStart().
*                                OS_ERR_NAME                    if 'p_name' is a NULL pointer
*                                OS_ERR_OBJ_CREATED             if the semaphore has already been created
*                                OS_ERR_OBJ_PTR_NULL            if 'p_sem'  is a NULL pointer
*                                OS_ERR_OBJ_TYPE                if 'p_sem' has already been initialized to a different
*                                                               object type
*
* Returns    : none
************************************************************************************************************************
*/

void  OSSemCreate (OS_SEM      *p_sem,          //多值信号量指针
                   CPU_CHAR    *p_name,         //多值信号量名字
                   OS_SEM_CTR   cnt,            // 如果是分享资源，则设置为资源数目 ，如果是标志位，设置为0 
                   OS_ERR      *p_err)          //返回的错误类型
{
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL                       //如果使能安全检测
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();             //执行安全检测异常函数
        return;
    }
#endif

#ifdef OS_SAFETY_CRITICAL_IEC61508                  //如果使能安全关键
    if (OSSafetyCriticalStartFlag == DEF_TRUE) {    //如果调用 OSSafetyCriticalStart() 后再创建多值信号量
       *p_err = OS_ERR_ILLEGAL_CREATE_RUN_TIME;     //错误类型为“非法创建内核对象”
        return;
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u              //如果使能中断调用检测
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* 如果在中断中调用                 */
       *p_err = OS_ERR_CREATE_ISR;                  //在中断函数中创建多值信号量
        return;
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u                          //如果使能参数检测
    if (p_sem == (OS_SEM *)0) {                             /* 如果 多值信号量 指针为空                                       */
       *p_err = OS_ERR_OBJ_PTR_NULL;            //错误类型为“多值信号量对象为空”
        return;
    }
#endif

    OS_CRITICAL_ENTER();        //进入临界段
    p_sem->Type    = OS_OBJ_TYPE_SEM;                       /* 初始多值信号量               */
    p_sem->Ctr     = cnt;                                   /* 设置信号量的cent                                   */
    p_sem->TS      = (CPU_TS)0;
    p_sem->NamePtr = p_name;                                /* 设置多值信号量的名字                         */
    OS_PendListInit(&p_sem->PendList);                      /* 初始化该多值信号量的列表                          */

#if OS_CFG_DBG_EN > 0u              //如果使能调试代码、变量
    OS_SemDbgListAdd(p_sem);        //将该多值信号量加入双向调试链表
#endif
    OSSemQty++;         //多值信号量个数+1

    OS_CRITICAL_EXIT_NO_SCHED();            //退出临界段(无调度)
   *p_err = OS_ERR_NONE;            //错误类型为“无错误”
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                  DELETE A SEMAPHORE
*
* Description: This function deletes a semaphore.
*
* Arguments  : p_sem         is a pointer to the semaphore to delete
*
*              opt           determines delete options as follows:
*
*                                OS_OPT_DEL_NO_PEND          Delete semaphore ONLY if no task pending
*                                OS_OPT_DEL_ALWAYS           Deletes the semaphore even if tasks are waiting.
*                                                            In this case, all the tasks pending will be readied.
*
*              p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE                 The call was successful and the semaphore was deleted
*                                OS_ERR_DEL_ISR              If you attempted to delete the semaphore from an ISR
*                                OS_ERR_OBJ_PTR_NULL         If 'p_sem' is a NULL pointer.
*                                OS_ERR_OBJ_TYPE             If 'p_sem' is not pointing at a semaphore
*                                OS_ERR_OPT_INVALID          An invalid option was specified
*                                OS_ERR_TASK_WAITING         One or more tasks were waiting on the semaphore
*
* Returns    : == 0          if no tasks were waiting on the semaphore, or upon error.
*              >  0          if one or more tasks waiting on the semaphore are now readied and informed.
*
* Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of the semaphore
*                 MUST check the return code of OSSemPend().
*              2) OSSemAccept() callers will not know that the intended semaphore has been deleted.
*              3) Because ALL tasks pending on the semaphore will be readied, you MUST be careful in applications where
*                 the semaphore is used for mutual exclusion because the resource(s) will no longer be guarded by the
*                 semaphore.
************************************************************************************************************************
*/

#if OS_CFG_SEM_DEL_EN > 0u
OS_OBJ_QTY  OSSemDel (OS_SEM  *p_sem,
                      OS_OPT   opt,
                      OS_ERR  *p_err)
{
    OS_OBJ_QTY     cnt;
    OS_OBJ_QTY     nbr_tasks;
    OS_PEND_DATA  *p_pend_data;
    OS_PEND_LIST  *p_pend_list;
    OS_TCB        *p_tcb;
    CPU_TS         ts;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_OBJ_QTY)0);
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to delete a semaphore from an ISR          */
       *p_err = OS_ERR_DEL_ISR;
        return ((OS_OBJ_QTY)0);
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u
    if (p_sem == (OS_SEM *)0) {                             /* Validate 'p_sem'                                       */
       *p_err = OS_ERR_OBJ_PTR_NULL;
        return ((OS_OBJ_QTY)0);
    }
    switch (opt) {                                          /* Validate 'opt'                                         */
        case OS_OPT_DEL_NO_PEND:
        case OS_OPT_DEL_ALWAYS:
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;
             return ((OS_OBJ_QTY)0);
    }
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_sem->Type != OS_OBJ_TYPE_SEM) {                   /* Make sure semaphore was created                        */
       *p_err = OS_ERR_OBJ_TYPE;
        return ((OS_OBJ_QTY)0);
    }
#endif

    CPU_CRITICAL_ENTER();
    p_pend_list = &p_sem->PendList;
    cnt         = p_pend_list->NbrEntries;
    nbr_tasks   = cnt;
    switch (opt) {
        case OS_OPT_DEL_NO_PEND:                            /* Delete semaphore only if no task waiting               */
             if (nbr_tasks == (OS_OBJ_QTY)0) {
#if OS_CFG_DBG_EN > 0u
                 OS_SemDbgListRemove(p_sem);
#endif
                 OSSemQty--;
                 OS_SemClr(p_sem);
                 CPU_CRITICAL_EXIT();
                *p_err = OS_ERR_NONE;
             } else {
                 CPU_CRITICAL_EXIT();
                *p_err = OS_ERR_TASK_WAITING;
             }
             break;

        case OS_OPT_DEL_ALWAYS:                             /* Always delete the semaphore                            */
             OS_CRITICAL_ENTER_CPU_EXIT();
             ts = OS_TS_GET();                              /* Get local time stamp so all tasks get the same time    */
             while (cnt > 0u) {                             /* Remove all tasks on the pend list                      */
                 p_pend_data = p_pend_list->HeadPtr;
                 p_tcb       = p_pend_data->TCBPtr;
                 OS_PendObjDel((OS_PEND_OBJ *)((void *)p_sem),
                               p_tcb,
                               ts);
                 cnt--;
             }
#if OS_CFG_DBG_EN > 0u
             OS_SemDbgListRemove(p_sem);
#endif
             OSSemQty--;
             OS_SemClr(p_sem);
             OS_CRITICAL_EXIT_NO_SCHED();
             OSSched();                                     /* Find highest priority task ready to run                */
            *p_err = OS_ERR_NONE;
             break;

        default:
             CPU_CRITICAL_EXIT();
            *p_err = OS_ERR_OPT_INVALID;
             break;
    }
    return ((OS_OBJ_QTY)nbr_tasks);
}
#endif

/*$PAGE*/
/*
************************************************************************************************************************
*                                                  PEND ON SEMAPHORE
*
* Description: This function waits for a semaphore.
*
* Arguments  : p_sem         is a pointer to the semaphore
*
*              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will wait for the    等待超时时间，0位无限等待，只有opt设置为OS_OPT_PEND_BLOCKING有效
*                            resource up to the amount of time (in 'ticks') specified by this argument.  If you specify
*                            0, however, your task will wait forever at the specified semaphore or, until the resource
*                            becomes available (or the event occurs).
*
*              opt           determines whether the user wants to block if the semaphore is not available or not:
*
*                                OS_OPT_PEND_BLOCKING        如果不能立即获得信号量，堵塞当前任务一直等待直到获得信号量
*                                OS_OPT_PEND_NON_BLOCKING    如果不能立即获得信号量，不堵塞当前任务，不继续信号量
*
*              p_ts          is a pointer to a variable that will receive the timestamp of when the semaphore was posted
*                            or pend aborted or the semaphore deleted.  If you pass a NULL pointer (i.e. (CPU_TS*)0)
*                            then you will not get the timestamp.  In other words, passing a NULL pointer is valid
*                            and indicates that you don't need the timestamp.   用于储存一个时间戳（可以为null）
*
*              p_err         is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE               The call was successful and your task owns the resource
*                                                          or, the event you are waiting for occurred.
*                                OS_ERR_OBJ_DEL            If 'p_sem' was deleted
*                                OS_ERR_OBJ_PTR_NULL       If 'p_sem' is a NULL pointer.
*                                OS_ERR_OBJ_TYPE           If 'p_sem' is not pointing at a semaphore
*                                OS_ERR_OPT_INVALID        If you specified an invalid value for 'opt'
*                                OS_ERR_PEND_ABORT         If the pend was aborted by another task
*                                OS_ERR_PEND_ISR           If you called this function from an ISR and the result
*                                                          would lead to a suspension.
*                                OS_ERR_PEND_WOULD_BLOCK   If you specified non-blocking but the semaphore was not
*                                                          available.
*                                OS_ERR_SCHED_LOCKED       If you called this function when the scheduler is locked
*                                OS_ERR_STATUS_INVALID     Pend status is invalid
*                                OS_ERR_TIMEOUT            The semaphore was not received within the specified
*                                                          timeout.
*
*
* Returns    : The current value of the semaphore counter or 0 if not available.
************************************************************************************************************************
*/

OS_SEM_CTR  OSSemPend (OS_SEM   *p_sem,
                       OS_TICK   timeout,   //等待超时时间
                       OS_OPT    opt,       //选项
                       CPU_TS   *p_ts,
                       OS_ERR   *p_err)
{
    OS_SEM_CTR    ctr;          
    OS_PEND_DATA  pend_data;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL               //安全检测
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();     //执行安全检测异常函数
        return ((OS_SEM_CTR)0);
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u          //中断调用检测
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* 如果在中断中调用                       */
       *p_err = OS_ERR_PEND_ISR;            //错误类型为“在中断中等待”
        return ((OS_SEM_CTR)0);                 
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u              //参数检测
    if (p_sem == (OS_SEM *)0) {                             /* Validate 'p_sem'                                       */
       *p_err = OS_ERR_OBJ_PTR_NULL;                //错误类型“内核对象为空”
        return ((OS_SEM_CTR)0);
    }
    switch (opt) {                                          /* 根据选项分类处理                                     */
        case OS_OPT_PEND_BLOCKING:                          //堵塞
        case OS_OPT_PEND_NON_BLOCKING:                      //不堵塞
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;                    //选项非法
             return ((OS_SEM_CTR)0);
    }
#endif
    
#if OS_CFG_OBJ_TYPE_CHK_EN > 0u                         //对象类型检测
    if (p_sem->Type != OS_OBJ_TYPE_SEM) {                   /* sem 不是多值信号量类型                       */
       *p_err = OS_ERR_OBJ_TYPE;                        //返回错误：内核对象类型错误
        return ((OS_SEM_CTR)0);
    }
#endif

    if (p_ts != (CPU_TS *)0) {                              //如果ts非空
       *p_ts  = (CPU_TS)0;                                  /*清空，用于返回时间戳                    */
    }
    CPU_CRITICAL_ENTER();                                       //关闭中断
    if (p_sem->Ctr > (OS_SEM_CTR)0) {                       /* 如果资源可用                                  */
        p_sem->Ctr--;                                       /* 资源数目-1                               */
        if (p_ts != (CPU_TS *)0) {                              //如果 ts非空
           *p_ts  = p_sem->TS;                              /*      获取该信号量最后一次发布的时间戳                       */
        }
        ctr   = p_sem->Ctr;                         //获取信号量当前的资源数目
        CPU_CRITICAL_EXIT();                        //开启中断
       *p_err = OS_ERR_NONE;                        //错误类型为“无错误”
        return (ctr);
    }

    if ((opt & OS_OPT_PEND_NON_BLOCKING) != (OS_OPT)0) {    /* 如果没有资源可用而且选择了不堵塞任务              */
        ctr   = p_sem->Ctr;                                 /* 获取信号量的资源数                                                     */
        CPU_CRITICAL_EXIT();                                //打开中断
       *p_err = OS_ERR_PEND_WOULD_BLOCK;                    //错误类型为“等待渴求堵塞”
        return (ctr);
    } else {                                                /* Yes                                                    */
        if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {    /* Can't pend when the scheduler is locked                */
            CPU_CRITICAL_EXIT();
           *p_err = OS_ERR_SCHED_LOCKED;
            return ((OS_SEM_CTR)0);
        }
    }
                                                            /* Lock the scheduler/re-enable interrupts                */
    OS_CRITICAL_ENTER_CPU_EXIT();
    OS_Pend(&pend_data,                                     /* Block task pending on Semaphore                        */
            (OS_PEND_OBJ *)((void *)p_sem),
            OS_TASK_PEND_ON_SEM,
            timeout);

    OS_CRITICAL_EXIT_NO_SCHED();

    OSSched();                                              /* Find the next highest priority task ready to run       */

    CPU_CRITICAL_ENTER();
    switch (OSTCBCurPtr->PendStatus) {
        case OS_STATUS_PEND_OK:                             /* We got the semaphore                                   */
             if (p_ts != (CPU_TS *)0) {
                *p_ts  =  OSTCBCurPtr->TS;
             }
            *p_err = OS_ERR_NONE;
             break;

        case OS_STATUS_PEND_ABORT:                          /* Indicate that we aborted                               */
             if (p_ts != (CPU_TS *)0) {
                *p_ts  =  OSTCBCurPtr->TS;
             }
            *p_err = OS_ERR_PEND_ABORT;
             break;

        case OS_STATUS_PEND_TIMEOUT:                        /* Indicate that we didn't get semaphore within timeout   */
             if (p_ts != (CPU_TS *)0) {
                *p_ts  = (CPU_TS  )0;
             }
            *p_err = OS_ERR_TIMEOUT;
             break;

        case OS_STATUS_PEND_DEL:                            /* Indicate that object pended on has been deleted        */
             if (p_ts != (CPU_TS *)0) {
                *p_ts  =  OSTCBCurPtr->TS;
             }
            *p_err = OS_ERR_OBJ_DEL;
             break;

        default:
            *p_err = OS_ERR_STATUS_INVALID;
             CPU_CRITICAL_EXIT();
             return ((OS_SEM_CTR)0);
    }
    ctr = p_sem->Ctr;
    CPU_CRITICAL_EXIT();
    return (ctr);
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                             ABORT WAITING ON A SEMAPHORE
*
* Description: This function aborts & readies any tasks currently waiting on a semaphore.  This function should be used
*              to fault-abort the wait on the semaphore, rather than to normally signal the semaphore via OSSemPost().
*
* Arguments  : p_sem     is a pointer to the semaphore
*
*              opt       determines the type of ABORT performed:
*
*                            OS_OPT_PEND_ABORT_1          ABORT wait for a single task (HPT) waiting on the semaphore
*                            OS_OPT_PEND_ABORT_ALL        ABORT wait for ALL tasks that are  waiting on the semaphore
*                            OS_OPT_POST_NO_SCHED         Do not call the scheduler
*
*              p_err     is a pointer to a variable that will contain an error code returned by this function.
*
*                            OS_ERR_NONE                  At least one task waiting on the semaphore was readied and
*                                                         informed of the aborted wait; check return value for the
*                                                         number of tasks whose wait on the semaphore was aborted.
*                            OS_ERR_OBJ_PTR_NULL          If 'p_sem' is a NULL pointer.
*                            OS_ERR_OBJ_TYPE              If 'p_sem' is not pointing at a semaphore
*                            OS_ERR_OPT_INVALID           If you specified an invalid option
*                            OS_ERR_PEND_ABORT_ISR        If you called this function from an ISR
*                            OS_ERR_PEND_ABORT_NONE       No task were pending
*
* Returns    : == 0          if no tasks were waiting on the semaphore, or upon error.
*              >  0          if one or more tasks waiting on the semaphore are now readied and informed.
************************************************************************************************************************
*/

#if OS_CFG_SEM_PEND_ABORT_EN > 0u
OS_OBJ_QTY  OSSemPendAbort (OS_SEM  *p_sem,
                            OS_OPT   opt,
                            OS_ERR  *p_err)
{
    OS_PEND_LIST  *p_pend_list;
    OS_TCB        *p_tcb;
    CPU_TS         ts;
    OS_OBJ_QTY     nbr_tasks;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_OBJ_QTY)0u);
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /* Not allowed to Pend Abort from an ISR                  */
       *p_err =  OS_ERR_PEND_ABORT_ISR;
        return ((OS_OBJ_QTY)0u);
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u
    if (p_sem == (OS_SEM *)0) {                             /* Validate 'p_sem'                                       */
       *p_err =  OS_ERR_OBJ_PTR_NULL;
        return ((OS_OBJ_QTY)0u);
    }
    switch (opt) {                                          /* Validate 'opt'                                         */
        case OS_OPT_PEND_ABORT_1:
        case OS_OPT_PEND_ABORT_ALL:
        case OS_OPT_PEND_ABORT_1   | OS_OPT_POST_NO_SCHED:
        case OS_OPT_PEND_ABORT_ALL | OS_OPT_POST_NO_SCHED:
             break;

        default:
            *p_err =  OS_ERR_OPT_INVALID;
             return ((OS_OBJ_QTY)0u);
    }
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_sem->Type != OS_OBJ_TYPE_SEM) {                   /* Make sure semaphore was created                        */
       *p_err =  OS_ERR_OBJ_TYPE;
        return ((OS_OBJ_QTY)0u);
    }
#endif

    CPU_CRITICAL_ENTER();
    p_pend_list = &p_sem->PendList;
    if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0u) {        /* Any task waiting on semaphore?                         */
        CPU_CRITICAL_EXIT();                                /* No                                                     */
       *p_err =  OS_ERR_PEND_ABORT_NONE;
        return ((OS_OBJ_QTY)0u);
    }

    OS_CRITICAL_ENTER_CPU_EXIT();
    nbr_tasks = 0u;
    ts        = OS_TS_GET();                                /* Get local time stamp so all tasks get the same time    */
    while (p_pend_list->NbrEntries > (OS_OBJ_QTY)0u) {
        p_tcb = p_pend_list->HeadPtr->TCBPtr;
        OS_PendAbort((OS_PEND_OBJ *)((void *)p_sem),
                     p_tcb,
                     ts);
        nbr_tasks++;
        if (opt != OS_OPT_PEND_ABORT_ALL) {                 /* Pend abort all tasks waiting?                          */
            break;                                          /* No                                                     */
        }
    }
    OS_CRITICAL_EXIT_NO_SCHED();

    if ((opt & OS_OPT_POST_NO_SCHED) == (OS_OPT)0u) {
        OSSched();                                          /* Run the scheduler                                      */
    }

   *p_err = OS_ERR_NONE;
    return (nbr_tasks);
}
#endif

/*$PAGE*/
/*
************************************************************************************************************************
*                                                 POST TO A SEMAPHORE
*
* Description: This function signals a semaphore
*
* Arguments  : p_sem    is a pointer to the semaphore
*
*              opt      determines the type of POST performed:
*
*                           OS_OPT_POST_1            POST and ready only the highest priority task waiting on semaphore   发布给等待该信号量的最高优先级任务
*                                                    (if tasks are waiting).
*                           OS_OPT_POST_ALL          POST to ALL tasks that are waiting on the semaphore                  发布给等待该信号量的所有任务
*
*                           OS_OPT_POST_NO_SCHED     Do not call the scheduler                                            发布给等待该信号量的最高优先级任务（不进行调度）
*
*                           Note(s): 1) OS_OPT_POST_NO_SCHED can be added with one of the other options.                  发布给等待该信号量的所有任务（不进行调度）
*
*              p_err    is a pointer to a variable that will contain an error code returned by this function.             
*
*                           OS_ERR_NONE          The call was successful and the semaphore was signaled.
*                           OS_ERR_OBJ_PTR_NULL  If 'p_sem' is a NULL pointer.
*                           OS_ERR_OBJ_TYPE      If 'p_sem' is not pointing at a semaphore
*                           OS_ERR_SEM_OVF       If the post would cause the semaphore count to overflow.
*
* Returns    : The current value of the semaphore counter or 0 upon error.
************************************************************************************************************************
*/

OS_SEM_CTR  OSSemPost (OS_SEM  *p_sem,
                       OS_OPT   opt,
                       OS_ERR  *p_err)
{
    OS_SEM_CTR  ctr;
    CPU_TS      ts;



#ifdef OS_SAFETY_CRITICAL                   //如果使能安全检测
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();     //执行安全检测异常函数
        return ((OS_SEM_CTR)0);
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u                  //如果使能参数检测
    if (p_sem == (OS_SEM *)0) {                             /* 如果sem对象为空                                     */
       *p_err  = OS_ERR_OBJ_PTR_NULL;                   //错误类型为“内核对象指针为空”
        return ((OS_SEM_CTR)0);
    }
    switch (opt) {                                          /* 根据OPT进行操作                                         */
        case OS_OPT_POST_1:
        case OS_OPT_POST_ALL:
        case OS_OPT_POST_1   | OS_OPT_POST_NO_SCHED:
        case OS_OPT_POST_ALL | OS_OPT_POST_NO_SCHED:
             break;

        default:
            *p_err =  OS_ERR_OPT_INVALID;               //超出OPT选项,错误类型为“选项非法”
             return ((OS_SEM_CTR)0u);           
    }
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u                 //如果使能对象类型检测
    if (p_sem->Type != OS_OBJ_TYPE_SEM) {                   /* 如果sem的类型不为 sem类型                      */
       *p_err = OS_ERR_OBJ_TYPE;            //错误类型“对象类型非法”
        return ((OS_SEM_CTR)0);
    }
#endif

    ts = OS_TS_GET();                                       /* 获取时间戳                                        */

#if OS_CFG_ISR_POST_DEFERRED_EN > 0u                //如果使能中断延迟发布
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* 如果在中断被调用                           */
        OS_IntQPost((OS_OBJ_TYPE)OS_OBJ_TYPE_SEM,           /* 将该信号量发布到中断信息队列                                     */
                    (void      *)p_sem,
                    (void      *)0,
                    (OS_MSG_SIZE)0,
                    (OS_FLAGS   )0,
                    (OS_OPT     )opt,
                    (CPU_TS     )ts,
                    (OS_ERR    *)p_err);
        return ((OS_SEM_CTR)0);
    }
#endif

    ctr = OS_SemPost(p_sem,                                 /* 将信号量按普通方式处理                                   */
                     opt,
                     ts,
                     p_err);

    return (ctr);           //返回信号当前记数值
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                    SET SEMAPHORE
*
* Description: This function sets the semaphore count to the value specified as an argument.  Typically, this value
*              would be 0 but of course, we can set the semaphore to any value.
*
*              You would typically use this function when a semaphore is used as a signaling mechanism
*              and, you want to reset the count value.
*
* Arguments  : p_sem     is a pointer to the semaphore
*
*              cnt       is the new value for the semaphore count.  You would pass 0 to reset the semaphore count.
*
*              p_err     is a pointer to a variable that will contain an error code returned by this function.
*
*                            OS_ERR_NONE           The call was successful and the semaphore value was set.
*                            OS_ERR_OBJ_PTR_NULL   If 'p_sem' is a NULL pointer.
*                            OS_ERR_OBJ_TYPE       If 'p_sem' is not pointing to a semaphore.
*                            OS_ERR_TASK_WAITING   If tasks are waiting on the semaphore.
*
* Returns    : None
************************************************************************************************************************
*/

#if OS_CFG_SEM_SET_EN > 0u
void  OSSemSet (OS_SEM      *p_sem,
                OS_SEM_CTR   cnt,
                OS_ERR      *p_err)
{
    OS_PEND_LIST  *p_pend_list;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Can't call this function from an ISR                   */
       *p_err = OS_ERR_SET_ISR;
        return;
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u
    if (p_sem == (OS_SEM *)0) {                             /* Validate 'p_sem'                                       */
       *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
#endif

#if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_sem->Type != OS_OBJ_TYPE_SEM) {                   /* Make sure semaphore was created                        */
       *p_err = OS_ERR_OBJ_TYPE;
        return;
    }
#endif

   *p_err = OS_ERR_NONE;
    CPU_CRITICAL_ENTER();
    if (p_sem->Ctr > (OS_SEM_CTR)0) {                       /* See if semaphore already has a count                   */
        p_sem->Ctr = cnt;                                   /* Yes, set it to the new value specified.                */
    } else {
        p_pend_list = &p_sem->PendList;                     /* No                                                     */
        if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) {     /*      See if task(s) waiting?                           */
            p_sem->Ctr = cnt;                               /*      No, OK to set the value                           */
        } else {
           *p_err      = OS_ERR_TASK_WAITING;
        }
    }
    CPU_CRITICAL_EXIT();
}
#endif

/*$PAGE*/
/*
************************************************************************************************************************
*                                           CLEAR THE CONTENTS OF A SEMAPHORE
*
* Description: This function is called by OSSemDel() to clear the contents of a semaphore
*

* Argument(s): p_sem      is a pointer to the semaphore to clear
*              -----
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_SemClr (OS_SEM  *p_sem)
{
    p_sem->Type    = OS_OBJ_TYPE_NONE;                      /* Mark the data structure as a NONE                      */
    p_sem->Ctr     = (OS_SEM_CTR)0;                         /* Set semaphore value                                    */
    p_sem->TS      = (CPU_TS    )0;                         /* Clear the time stamp                                   */
    p_sem->NamePtr = (CPU_CHAR *)((void *)"?SEM");
    OS_PendListInit(&p_sem->PendList);                      /* Initialize the waiting list                            */
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                        ADD/REMOVE SEMAPHORE TO/FROM DEBUG LIST
*
* Description: These functions are called by uC/OS-III to add or remove a semaphore to/from the debug list.
*
* Arguments  : p_sem     is a pointer to the semaphore to add/remove
*
* Returns    : none
*
* Note(s)    : These functions are INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/


#if OS_CFG_DBG_EN > 0u
void  OS_SemDbgListAdd (OS_SEM  *p_sem)     //将该多值信号量插入到多值信号量列表的最前端
{
    p_sem->DbgNamePtr               = (CPU_CHAR *)((void *)" ");        //先不指向任务的名称
    p_sem->DbgPrevPtr               = (OS_SEM   *)0;        //将 sem 作为列表的最前端
    if (OSSemDbgListPtr == (OS_SEM *)0) {       //如果列表为空
        p_sem->DbgNextPtr           = (OS_SEM   *)0;        //该信号量的下一个信号量为空
    } else {                                                //如果列表非空
        p_sem->DbgNextPtr           =  OSSemDbgListPtr;      //列表原本的首元素作为 sem的下一个元素
        OSSemDbgListPtr->DbgPrevPtr =  p_sem;                //原本列表的首元素的前面改为 sem
    }
    OSSemDbgListPtr                 =  p_sem;               // sem改为该列表的首元素
}



void  OS_SemDbgListRemove (OS_SEM  *p_sem)
{
    OS_SEM  *p_sem_next;
    OS_SEM  *p_sem_prev;


    p_sem_prev = p_sem->DbgPrevPtr;
    p_sem_next = p_sem->DbgNextPtr;

    if (p_sem_prev == (OS_SEM *)0) {
        OSSemDbgListPtr = p_sem_next;
        if (p_sem_next != (OS_SEM *)0) {
            p_sem_next->DbgPrevPtr = (OS_SEM *)0;
        }
        p_sem->DbgNextPtr = (OS_SEM *)0;

    } else if (p_sem_next == (OS_SEM *)0) {
        p_sem_prev->DbgNextPtr = (OS_SEM *)0;
        p_sem->DbgPrevPtr      = (OS_SEM *)0;

    } else {
        p_sem_prev->DbgNextPtr =  p_sem_next;
        p_sem_next->DbgPrevPtr =  p_sem_prev;
        p_sem->DbgNextPtr      = (OS_SEM *)0;
        p_sem->DbgPrevPtr      = (OS_SEM *)0;
    }
}
#endif

/*$PAGE*/
/*
************************************************************************************************************************
*                                                SEMAPHORE INITIALIZATION
*
* Description: This function is called by OSInit() to initialize the semaphore management.
*

* Argument(s): p_err        is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE     the call was successful
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_SemInit (OS_ERR  *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if OS_CFG_DBG_EN > 0u
    OSSemDbgListPtr = (OS_SEM *)0;
#endif

    OSSemQty        = (OS_OBJ_QTY)0;
   *p_err           = OS_ERR_NONE;
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                 POST TO A SEMAPHORE
*
* Description: This function signals a semaphore
*
* Arguments  : p_sem    is a pointer to the semaphore
*
*              opt      determines the type of POST performed:
*
*                           OS_OPT_POST_1            POST to a single waiting task
*                           OS_OPT_POST_ALL          POST to ALL tasks that are waiting on the semaphore
*
*                           OS_OPT_POST_NO_SCHED     Do not call the scheduler
*
*                           Note(s): 1) OS_OPT_POST_NO_SCHED can be added with one of the other options.
*
*              ts       is a timestamp indicating when the post occurred.
*
*              p_err    is a pointer to a variable that will contain an error code returned by this function.
*
*                           OS_ERR_NONE          The call was successful and the semaphore was signaled.
*                           OS_ERR_OBJ_PTR_NULL  If 'p_sem' is a NULL pointer.
*                           OS_ERR_OBJ_TYPE      If 'p_sem' is not pointing at a semaphore
*                           OS_ERR_SEM_OVF       If the post would cause the semaphore count to overflow.
*
* Returns    : The current value of the semaphore counter or 0 upon error.
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

OS_SEM_CTR  OS_SemPost (OS_SEM  *p_sem,
                        OS_OPT   opt,
                        CPU_TS   ts,
                        OS_ERR  *p_err)
{
    OS_OBJ_QTY     cnt;
    OS_SEM_CTR     ctr;
    OS_PEND_LIST  *p_pend_list;
    OS_PEND_DATA  *p_pend_data;
    OS_PEND_DATA  *p_pend_data_next;
    OS_TCB        *p_tcb;
    CPU_SR_ALLOC();



    CPU_CRITICAL_ENTER();
    p_pend_list = &p_sem->PendList;
    if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) {         /* Any task waiting on semaphore?                         */
        switch (sizeof(OS_SEM_CTR)) {
            case 1u:
                 if (p_sem->Ctr == DEF_INT_08U_MAX_VAL) {
                     CPU_CRITICAL_EXIT();
                    *p_err = OS_ERR_SEM_OVF;
                     return ((OS_SEM_CTR)0);
                 }
                 break;

            case 2u:
                 if (p_sem->Ctr == DEF_INT_16U_MAX_VAL) {
                     CPU_CRITICAL_EXIT();
                    *p_err = OS_ERR_SEM_OVF;
                     return ((OS_SEM_CTR)0);
                 }
                 break;

            case 4u:
                 if (p_sem->Ctr == DEF_INT_32U_MAX_VAL) {
                     CPU_CRITICAL_EXIT();
                    *p_err = OS_ERR_SEM_OVF;
                     return ((OS_SEM_CTR)0);
                 }
                 break;

            default:
                 break;
        }
        p_sem->Ctr++;                                       /* No                                                     */
        ctr       = p_sem->Ctr;
        p_sem->TS = ts;                                     /* Save timestamp in semaphore control block              */
        CPU_CRITICAL_EXIT();
       *p_err     = OS_ERR_NONE;
        return (ctr);
    }

    OS_CRITICAL_ENTER_CPU_EXIT();
    if ((opt & OS_OPT_POST_ALL) != (OS_OPT)0) {             /* Post message to all tasks waiting?                     */
        cnt = p_pend_list->NbrEntries;                      /* Yes                                                    */
    } else {
        cnt = (OS_OBJ_QTY)1;                                /* No                                                     */
    }
    p_pend_data = p_pend_list->HeadPtr;
    while (cnt > 0u) {
        p_tcb            = p_pend_data->TCBPtr;
        p_pend_data_next = p_pend_data->NextPtr;
        OS_Post((OS_PEND_OBJ *)((void *)p_sem),
                p_tcb,
                (void      *)0,
                (OS_MSG_SIZE)0,
                ts);
        p_pend_data = p_pend_data_next;
        cnt--;
    }
    ctr = p_sem->Ctr;
    OS_CRITICAL_EXIT_NO_SCHED();
    if ((opt & OS_OPT_POST_NO_SCHED) == (OS_OPT)0) {
        OSSched();                                          /* Run the scheduler                                      */
    }
   *p_err = OS_ERR_NONE;
    return (ctr);
}

#endif
