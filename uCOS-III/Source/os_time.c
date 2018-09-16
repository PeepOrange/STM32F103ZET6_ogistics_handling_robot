/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TIME MANAGEMENT
*
* File    : OS_TIME.C
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
const  CPU_CHAR  *os_time__c = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                  DELAY TASK 'n' TICKS
*
* Description: This function is called to delay execution of the currently running task until the specified number of
*              system ticks expires.  This, of course, directly equates to delaying the current task for some time to
*              expire.  No delay will result if the specified delay is 0.  If the specified delay is greater than 0
*              then, a context switch will result.
*
* Arguments  : dly       is a value in 'clock ticks' that the task will either delay for or, the target match value
*                        of the tick counter (OSTickCtr).  Note that specifying 0 means the task is not to delay.
*
*                        depending on the option argument, the task will wake up when OSTickCtr reaches:
*
*                            OS_OPT_TIME_DLY      : OSTickCtr + dly
*                            OS_OPT_TIME_TIMEOUT  : OSTickCtr + dly
*                            OS_OPT_TIME_MATCH    : dly
*                            OS_OPT_TIME_PERIODIC : OSTCBCurPtr->TickCtrPrev + dly
*
*              opt       specifies whether 'dly' represents absolute or relative time; default option marked with *** :
*
*                        *** OS_OPT_TIME_DLY        specifies a relative time from the current value of OSTickCtr.
*                            OS_OPT_TIME_TIMEOUT    same as OS_OPT_TIME_DLY.
*                            OS_OPT_TIME_MATCH      indicates that 'dly' specifies the absolute value that OSTickCtr
*                                                   must reach before the task will be resumed.
*                            OS_OPT_TIME_PERIODIC   indicates that 'dly' specifies the periodic value that OSTickCtr
*                                                   must reach before the task will be resumed.
*
*              p_err     is a pointer to a variable that will contain an error code from this call.
*
*                            OS_ERR_NONE            the call was successful and the delay occurred.
*                            OS_ERR_OPT_INVALID     if you specified an invalid option for this function.
*                            OS_ERR_SCHED_LOCKED    can't delay when the scheduler is locked.
*                            OS_ERR_TIME_DLY_ISR    if you called this function from an ISR.
*                            OS_ERR_TIME_ZERO_DLY   if you specified a delay of zero.
*
* Returns    : none
************************************************************************************************************************
*/
//不允许在中断中调用该函数（长时间用周期性，短时间用延时性）
void  OSTimeDly (OS_TICK   dly,                             //延时的节拍数
                 OS_OPT    opt,                              //延时选项
                 OS_ERR   *p_err)                           //返回错误
{
    CPU_SR_ALLOC();                                         //用于临界段保存CPU 状态寄存器SR



#ifdef OS_SAFETY_CRITICAL                                   //如果使能安全检测（默认使能）
    if (p_err == (OS_ERR *)0) {                             //错误类型实际参数为空
        OS_SAFETY_CRITICAL_EXCEPTION();                     //执行安全检测异常函数
        return;                                             //返回，不执行延时操作
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u                      //如果使能中断中非法调用检测
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /*不允许延时在中断中调用                       */
       *p_err = OS_ERR_TIME_DLY_ISR;                        //错误类型为“在中断函数中延时”
        return;                                             //返回，不执行延时操作
    }
#endif

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0u) {       /* 如果调度器被锁            */
       *p_err = OS_ERR_SCHED_LOCKED;                        //错误类型为“调度器被锁”
        return;                                             //返回，不执行延时操作
    }       

    switch (opt) {                                         
        case OS_OPT_TIME_DLY:                               //相对时间
        case OS_OPT_TIME_TIMEOUT:                            //超时
        case OS_OPT_TIME_PERIODIC:                          //周期性延时
             if (dly == (OS_TICK)0u) {                      /* 如果dly为0                                     */
                *p_err = OS_ERR_TIME_ZERO_DLY;              //返回错误类型“0延时”
                 return;                                    //返回，不执行操作
             }
             break;

        case OS_OPT_TIME_MATCH:                             //绝对时间
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;                    //返回错误类型“选项非法”
             return;
    }

    OS_CRITICAL_ENTER();                                    //进入临界段
    OSTCBCurPtr->TaskState = OS_TASK_STATE_DLY;             //修改当前任务状态为延时态
    OS_TickListInsert(OSTCBCurPtr,                          //将当前任务插入节拍列表
                      dly,
                      opt,
                      p_err);
    if (*p_err != OS_ERR_NONE) {                            //如果插入节拍列表失败
         OS_CRITICAL_EXIT_NO_SCHED();                       //退出临界段
         return;                                            //返回，不执行延时操作
    }
    OS_RdyListRemove(OSTCBCurPtr);                          /* 从就绪列表移除当前任务                   */
    OS_CRITICAL_EXIT_NO_SCHED();                            //退出临界段（无调度）
    OSSched();                                              /* 任务切换                                */
   *p_err = OS_ERR_NONE;                                    //错误类型为无错误
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                             DELAY TASK FOR SPECIFIED TIME
*
* Description: This function is called to delay execution of the currently running task until some time expires.  This
*              call allows you to specify the delay time in HOURS, MINUTES, SECONDS and MILLISECONDS instead of ticks.
*
* Arguments  : hours     specifies the number of hours that the task will be delayed (max. is 999 if the tick rate is
*                        1000 Hz or less otherwise, a higher value would overflow a 32-bit unsigned counter).
*
*              minutes   specifies the number of minutes (max. 59 if 'opt' is OS_OPT_TIME_HMSM_STRICT)
*
*              seconds   specifies the number of seconds (max. 59 if 'opt' is OS_OPT_TIME_HMSM_STRICT)
*
*              milli     specifies the number of milliseconds (max. 999 if 'opt' is OS_OPT_TIME_HMSM_STRICT)
*
*              opt       specifies time delay bit-field options logically OR'd; default options marked with *** :
*
*                        *** OS_OPT_TIME_DLY        specifies a relative time from the current value of OSTickCtr.
*                            OS_OPT_TIME_TIMEOUT    same as OS_OPT_TIME_DLY.
*                            OS_OPT_TIME_MATCH      indicates that the delay specifies the absolute value that OSTickCtr
*                                                   must reach before the task will be resumed.
*                            OS_OPT_TIME_PERIODIC   indicates that the delay specifies the periodic value that OSTickCtr
*                                                   must reach before the task will be resumed.
*
*                        *** OS_OPT_TIME_HMSM_STRICT            strictly allow only hours        (0...99)
*                                                                                   minutes      (0...59)
*                                                                                   seconds      (0...59)
*                                                                                   milliseconds (0...999)
*                            OS_OPT_TIME_HMSM_NON_STRICT        allow any value of  hours        (0...999)
*                                                                                   minutes      (0...9999)
*                                                                                   seconds      (0...65535)
*                                                                                   milliseconds (0...4294967295)
*
*              p_err     is a pointer to a variable that will receive an error code from this call.
*
*                            OS_ERR_NONE                        If the function returns from the desired delay
*                            OS_ERR_OPT_INVALID                 If you specified an invalid option for 'opt'
*                            OS_ERR_SCHED_LOCKED                Can't delay when the scheduler is locked
*                            OS_ERR_TIME_DLY_ISR                If called from an ISR
*                            OS_ERR_TIME_INVALID_HOURS          If you didn't specify a valid value for 'hours'
*                            OS_ERR_TIME_INVALID_MINUTES        If you didn't specify a valid value for 'minutes'
*                            OS_ERR_TIME_INVALID_SECONDS        If you didn't specify a valid value for 'seconds'
*                            OS_ERR_TIME_INVALID_MILLISECONDS   If you didn't specify a valid value for 'milli'
*                            OS_ERR_TIME_ZERO_DLY               If hours, minutes, seconds and milli are all 0
*
* Returns    : none
*
* Note(s)    : 1) The resolution on the milliseconds depends on the tick rate.  For example, you can't do a 10 mS delay
*                 if the ticker interrupts every 100 mS.  In this case, the delay would be set to 0.  The actual delay
*                 is rounded to the nearest tick.
*
*              2) Although this function allows you to delay a task for many, many hours, it's not recommended to put
*                 a task to sleep for that long.
************************************************************************************************************************
*/
//与OSTimeDly类似，都是停止任务的运行，延迟一段时间再运行，特点是可以按时分秒进行延时，不可以在中断函数调用
#if OS_CFG_TIME_DLY_HMSM_EN > 0u
void  OSTimeDlyHMSM (CPU_INT16U   hours,                //延时小时数
                     CPU_INT16U   minutes,              //分钟数
                     CPU_INT16U   seconds,              //秒数
                     CPU_INT32U   milli,                //毫秒数
                     OS_OPT       opt,                  //延迟选项
                     OS_ERR      *p_err)                //返回错误类型
{
#if OS_CFG_ARG_CHK_EN > 0u                              //如果使能参数检测（默认使能）
    CPU_BOOLEAN  opt_invalid;                           //声明变量用于参数检测
    CPU_BOOLEAN  opt_non_strict;
#endif
    OS_OPT       opt_time;
    OS_RATE_HZ   tick_rate;
    OS_TICK      ticks;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL                               //如果使能安全检测（默认关闭）
    if (p_err == (OS_ERR *)0) {                         //如果错误类型实参为空
        OS_SAFETY_CRITICAL_EXCEPTION();                 //执行安全检测异常函数
        return;                                       //返回，不执行延时
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u                      //如果使能中断非法调用检测（默认使能）
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /*     如果延迟函数在中断中执行                   */
       *p_err = OS_ERR_TIME_DLY_ISR;                         //错误类型为“在中断函数中延迟”       
        return;                                           //返回，不执行延时      
    }
#endif

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0u) {       /* 如果调度器被锁              */
       *p_err = OS_ERR_SCHED_LOCKED;                        //错误类型为“调度器被锁”
        return;
    }

    opt_time = opt & OS_OPT_TIME_MASK;                      /* 检测延时选项中只与延时时间性质有关的位                           */
    switch (opt_time) {
        case OS_OPT_TIME_DLY:                               //相对时间
        case OS_OPT_TIME_TIMEOUT:                           //超时
        case OS_OPT_TIME_PERIODIC:                          //周期时间
             if (milli == (CPU_INT32U)0u) {                 /* 如果毫秒数为0                 */
                 if (seconds == (CPU_INT16U)0u) {           //如果秒数为0
                     if (minutes == (CPU_INT16U)0u) {       //如果分钟数为0
                         if (hours == (CPU_INT16U)0u) {     //如果小时数为0
                            *p_err = OS_ERR_TIME_ZERO_DLY;  //错误类型为“0延迟”
                             return;                        //返回，不执行延迟
                         }
                     }
                 }
             }
             break;

        case OS_OPT_TIME_MATCH:                             //绝对时间
             break;
    
        default:                                            //如果选项超出范围
            *p_err = OS_ERR_OPT_INVALID;                    //错误类型为“选项非法”
             return;                                        //返回，不执行延迟
    }

#if OS_CFG_ARG_CHK_EN > 0u                                  /* 如果使能参数检测（默认使能）                */
    opt_invalid = DEF_BIT_IS_SET_ANY(opt, ~OS_OPT_TIME_OPTS_MASK);      //检测除选项位以外的位是否被置位
    if (opt_invalid == DEF_YES) {                                       //如果被置位
       *p_err = OS_ERR_OPT_INVALID;                                     //错误类型为“选项非法”
        return;                                             //返回，不执行延迟操作
    }

    opt_non_strict = DEF_BIT_IS_SET(opt, OS_OPT_TIME_HMSM_NON_STRICT);  // 检测有关时间参数范围的选项位
    if (opt_non_strict != DEF_YES) {                                    //如果选项选择了 OS_OPT_TIME_HMSM_STRICT （即严格时间）
         if (milli   > (CPU_INT32U)999u) {                              //如果毫秒数>999
            *p_err = OS_ERR_TIME_INVALID_MILLISECONDS;                  //错误类型为“毫秒数不可用”
             return;                                                    //返回，不执行延迟操作
         }
         if (seconds > (CPU_INT16U)59u) {                               //如果秒数>59
            *p_err = OS_ERR_TIME_INVALID_SECONDS;                       //错误类型为“秒数不可用”
             return;                                                    //返回，不执行延迟操作
         }
         if (minutes > (CPU_INT16U)59u) {                               //如果分钟数>59
            *p_err = OS_ERR_TIME_INVALID_MINUTES;                       //错误类型为“分钟数不可用”
             return;                                                    //返回，不执行延迟操作
         }
         if (hours   > (CPU_INT16U)99u) {                               //如果小时数>99
            *p_err = OS_ERR_TIME_INVALID_HOURS;                         //错误类型为“小时数不可用”
             return;                                                    //返回，不执行延迟操作
         }
    } else {                                                            // 如果选项选择了 OS_OPT_TIME_HMSM_NON_STRICT （即非严格时间）   
         if (minutes > (CPU_INT16U)9999u) {                             //如果分钟数大于9999
            *p_err = OS_ERR_TIME_INVALID_MINUTES;                       //返回错误类型“分钟数不可用”
             return;                                                    //返回，不执行延迟操作
         }
         if (hours   > (CPU_INT16U)999u) {                              //如果小时数大于999
            *p_err = OS_ERR_TIME_INVALID_HOURS;                         //返回错误类型“小时数不可用”
             return;                                                    //返回，不执行延迟操作
         }
    }
#endif

    //将延时时间转换为时钟节拍数
    tick_rate = OSCfg_TickRate_Hz;          //获取时钟节拍的频率
    ticks     = ((OS_TICK)hours * (OS_TICK)3600u + (OS_TICK)minutes * (OS_TICK)60u + (OS_TICK)seconds) * tick_rate      //将延迟时间转换为时钟节拍数
              + (tick_rate * ((OS_TICK)milli + (OS_TICK)500u / tick_rate)) / (OS_TICK)1000u;

    if (ticks > (OS_TICK)0u) {                  //如果延迟节拍数>0
        OS_CRITICAL_ENTER();                    //进入临界段
        OSTCBCurPtr->TaskState = OS_TASK_STATE_DLY;     //修改当前任务的状态为延时态
        OS_TickListInsert(OSTCBCurPtr,                  //将当前任务插入节拍列表
                          ticks,                
                          opt_time,
                          p_err);
        if (*p_err != OS_ERR_NONE) {                    //如果插入节拍列表失败
             OS_CRITICAL_EXIT_NO_SCHED();               //退出临界段
             return;                                    //返回，不执行延迟
        }
        OS_RdyListRemove(OSTCBCurPtr);                      /* 将当前任务从就绪列表移除                    */
        OS_CRITICAL_EXIT_NO_SCHED();                        //退出临界段
        OSSched();                                          /* 任务切换                                 */
       *p_err = OS_ERR_NONE;                                //错误类型为“无错误”
    } else {                                                //如果延迟节拍为0
       *p_err = OS_ERR_TIME_ZERO_DLY;                       //错误类型为“0延迟”   
    }
}
#endif
/*$PAGE*/
/*
************************************************************************************************************************
*                                                RESUME A DELAYED TASK
*
* Description: This function is used resume a task that has been delayed through a call to either OSTimeDly() or
*              OSTimeDlyHMSM().  Note that cannot call this function to resume a task that is waiting for an event
*              with timeout.
*
* Arguments  : p_tcb    is a pointer to the TCB of the task to resume.
*
*              p_err    is a pointer to a variable that will receive an error code
*
*                           OS_ERR_NONE                  Task has been resumed
*                           OS_ERR_STATE_INVALID         Task is in an invalid state
*                           OS_ERR_TIME_DLY_RESUME_ISR   If called from an ISR
*                           OS_ERR_TIME_NOT_DLY          Task is not waiting for time to expire
*                           OS_ERR_TASK_SUSPENDED        Task cannot be resumed, it was suspended by OSTaskSuspend()
*
* Note(s)    : none
************************************************************************************************************************
*/

#if OS_CFG_TIME_DLY_RESUME_EN > 0u
void  OSTimeDlyResume (OS_TCB  *p_tcb,                      //用于直接结束其他任务的延迟
                       OS_ERR  *p_err)
{
    CPU_SR_ALLOC();     //用到临界段时用于保存CPU SR寄存器的值



#ifdef OS_SAFETY_CRITICAL               //如果使能安全检测
    if (p_err == (OS_ERR *)0) {         //错误实参为0
        OS_SAFETY_CRITICAL_EXCEPTION(); //执行安全检测异常函数
        return;                         //返回，函数执行失败
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u          //如果使能中断非法调用检测
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /*如果该函数在中断中被调用                       */
       *p_err = OS_ERR_TIME_DLY_RESUME_ISR;             //返回错误类型“在中断中结束延迟”
        return;                                             //返回，函数执行失败
    }
#endif

#if OS_CFG_ARG_CHK_EN > 0u                      //如果使能参数检测
    if (p_tcb == (OS_TCB *)0) {                             /* 如果任务为空       */
       *p_err = OS_ERR_TASK_NOT_DLY;                        //错误类型为“任务不在延时”
        return;                                             //返回，函数执行失败
    }
#endif

    CPU_CRITICAL_ENTER();                                    //关闭中断 
    if (p_tcb == OSTCBCurPtr) {                             /* 如果任务是当前任务       */
       *p_err = OS_ERR_TASK_NOT_DLY;                        //错误类型为“任务不在执行”
        CPU_CRITICAL_EXIT();                                //退出临界段（即打开中断）
        return;                                             //返回，函数执行失败    
    }

    switch (p_tcb->TaskState) {                             //根据状态执行
        case OS_TASK_STATE_RDY:                             /* 如果任务处于就绪态                    */
             CPU_CRITICAL_EXIT();                           //打开中断
            *p_err = OS_ERR_TASK_NOT_DLY;                   //错误类型为“任务不在延迟”
             break;                                         //返回，函数执行失败
    
        case OS_TASK_STATE_DLY:                             //如果任务处于延迟态    
             OS_CRITICAL_ENTER_CPU_EXIT();                  //进入临界段并且开中断
             p_tcb->TaskState = OS_TASK_STATE_RDY;          //任务状态改为就绪态
             OS_TickListRemove(p_tcb);                      /* 将任务从节拍列表中移除                             */
             OS_RdyListInsert(p_tcb);                      /* 将任务插入准备列表                                      */
             OS_CRITICAL_EXIT_NO_SCHED();                   //退出临界段（无调度）
            *p_err = OS_ERR_NONE;                           //错误类型为“无错误”
             break;

        case OS_TASK_STATE_PEND:                          //如果任务处于无期限等待态  
             CPU_CRITICAL_EXIT();                         //打开中断
            *p_err = OS_ERR_TASK_NOT_DLY;                   //错误类型为“任务不在延迟”
             break;                                         //返回，函数执行失败

        case OS_TASK_STATE_PEND_TIMEOUT:                    //如果任务为有期限等待态
             CPU_CRITICAL_EXIT();                           //打开中断
            *p_err = OS_ERR_TASK_NOT_DLY;                   //错误类型为“任务不在延迟”
             break;                                         //返回，函数执行失败

        case OS_TASK_STATE_SUSPENDED:                       //如果任务处于挂起态
             CPU_CRITICAL_EXIT();                           //打开中断
            *p_err = OS_ERR_TASK_NOT_DLY;                   //错误类型为“任务不在延迟”
             break;                                         //返回，函数执行失败

        case OS_TASK_STATE_DLY_SUSPENDED:                   //如果任务处于延迟中挂起态
             OS_CRITICAL_ENTER_CPU_EXIT();                  //进入临界段并且开中断
             p_tcb->TaskState = OS_TASK_STATE_SUSPENDED;    //修改任务为挂起态
             OS_TickListRemove(p_tcb);                      /* 将任务从节拍列表移除                             */
             OS_CRITICAL_EXIT_NO_SCHED();                   //退出临界段（无调度）
            *p_err            = OS_ERR_TASK_SUSPENDED;      //错误类型为“任务被挂起”
             break;                                         //

        case OS_TASK_STATE_PEND_SUSPENDED:                  //如果任务处于无期限的挂起态
             CPU_CRITICAL_EXIT();                           //打开中断
            *p_err = OS_ERR_TASK_NOT_DLY;                   //错误类型为“任务不在延迟”
             break;

        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:              //如果任务处于有期限的挂起态
             CPU_CRITICAL_EXIT();                               //打开中断
            *p_err = OS_ERR_TASK_NOT_DLY;                       //错误类型为“任务不在延迟态”
             break;

        default:                                            //如果任务超出范围
             CPU_CRITICAL_EXIT();                           //打开中断
            *p_err = OS_ERR_STATE_INVALID;                      //错误类型为“任务状态非法”
             break;
    }

    OSSched();                                              //任务切换
}
#endif
/*$PAGE*/
/*
************************************************************************************************************************
*                                               GET CURRENT SYSTEM TIME
*
* Description: This function is used by your application to obtain the current value of the counter which keeps track of
*              the number of clock ticks.
*
* Arguments  : p_err    is a pointer to a variable that will receive an error code
*
*                           OS_ERR_NONE           If the call was successful
*
* Returns    : The current value of OSTickCtr
************************************************************************************************************************
*/

OS_TICK  OSTimeGet (OS_ERR  *p_err)                 //返回值：当前节拍记数值
{
    OS_TICK  ticks;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL                       //如果使能安全检测
    if (p_err == (OS_ERR *)0) {                 //如果错误类型实参为0
        OS_SAFETY_CRITICAL_EXCEPTION();          //执行安全检测异常函数   
        return ((OS_TICK)0);                    //返回0，函数执行不成功
    }
#endif

    CPU_CRITICAL_ENTER();                   //关闭中断
    ticks = OSTickCtr;                      //获取当前时钟节拍计数值
    CPU_CRITICAL_EXIT();                    //打开中断
   *p_err = OS_ERR_NONE;                    //错误类型为“无错误”
    return (ticks);                         //返回当前时钟节拍记数值
}

/*
************************************************************************************************************************
*                                                   SET SYSTEM CLOCK
*
* Description: This function sets the counter which keeps track of the number of clock ticks.
*
* Arguments  : ticks    is the desired tick value
*
*              p_err    is a pointer to a variable that will receive an error code
*
*                           OS_ERR_NONE           If the call was successful
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeSet (OS_TICK   ticks,           //设置当前时钟节拍（谨慎使用）
                 OS_ERR   *p_err)
{
    CPU_SR_ALLOC();     //进入临界段时 用来保存cpu状态寄存器SR 的值



#ifdef OS_SAFETY_CRITICAL       //如果使能安全检测
    if (p_err == (OS_ERR *)0) {     //如果错误类型实参为空
        OS_SAFETY_CRITICAL_EXCEPTION();     //执行安全检测异常函数
        return;                         //返回，函数执行失败
    }
#endif

    CPU_CRITICAL_ENTER();           //关闭中断
    OSTickCtr = ticks;              //当前时钟节拍设置为ticks
    CPU_CRITICAL_EXIT();            //打开中断
   *p_err     = OS_ERR_NONE;        //错误类型为“无错误”
}

/*$PAGE*/
/*
************************************************************************************************************************
*                                                 PROCESS SYSTEM TICK
*
* Description: This function is used to signal to uC/OS-III the occurrence of a 'system tick' (also known as a
*              'clock tick').  This function should be called by the tick ISR.
*
* Arguments  : none
*
* Returns    : none
************************************************************************************************************************
*/

void  OSTimeTick (void)
{
    OS_ERR  err;
#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
    CPU_TS  ts;
#endif


    OSTimeTickHook();                                       /* 调用用户的钩子函数，即时钟节拍到来时的事件  */

#if OS_CFG_ISR_POST_DEFERRED_EN > 0u                        //如果使能中断发送延迟（默认使能）

    ts = OS_TS_GET();                                       /* 获得时间戳  */
    OS_IntQPost((OS_OBJ_TYPE) OS_OBJ_TYPE_TICK,             //任务信号量暂时发送到中断队列，退出中断后由优先级最高的延迟发布任务   
                (void      *)&OSRdyList[OSPrioCur],         //就绪发送到时钟节拍任务 OS_TickTask(),OS_TickTask()接受到该信号量就会继续执行
                (void      *) 0,                            //中断发送延迟可以减少中断时间，中断事件转为任务级，提高操作系统实时性
                (OS_MSG_SIZE) 0u,
                (OS_FLAGS   ) 0u,
                (OS_OPT     ) 0u,
                (CPU_TS     ) ts,
                (OS_ERR    *)&err);

#else                                                       //禁用中断发送延迟

   (void)OSTaskSemPost((OS_TCB *)&OSTickTaskTCB,            /* 直接发送信号量给Systic系统节拍任务    OS_TickTask() */
                       (OS_OPT  ) OS_OPT_POST_NONE,         //OS_TickTask()接收到信号量，进入就绪态，准备运行
                       (OS_ERR *)&err);                     


#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u                        //如果使能了时间片轮转调度器（默认使能）（同优先级任务）
    OS_SchedRoundRobin(&OSRdyList[OSPrioCur]);              //检测当前时间是否耗尽，如果耗尽就调用别的同优先级任务进行
#endif

#if OS_CFG_TMR_EN > 0u                                      //如果使能软件定时器（默认使能）
    OSTmrUpdateCtr--;                                       //软件定时器-1
    if (OSTmrUpdateCtr == (OS_CTR)0u) {                     //如果软件定时器减到0
        OSTmrUpdateCtr = OSTmrUpdateCnt;                    //重载软件定时器
        OSTaskSemPost((OS_TCB *)&OSTmrTaskTCB,              // 发送信号量给软件定时器任务     OS_TmrTask()                                 
                      (OS_OPT  ) OS_OPT_POST_NONE,
                      (OS_ERR *)&err);
    }
#endif

#endif
}
