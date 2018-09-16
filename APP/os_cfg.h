/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2010; Micrium, Inc.; Weston, FL
*                          All rights reserved.  Protected by international copyright laws.
*
*                                                  CONFIGURATION  FILE
*
* File    : OS_CFG.H
* By      : JJL
* Version : V3.01.2
*
* LICENSING TERMS:
* ---------------
*               uC/OS-III is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
************************************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H


                                             /* ---------------------------- MISCELLANEOUS -------------------------- */
#define OS_CFG_APP_HOOKS_EN             1u   /* Enable (1) or Disable (0) application specific hooks                  */
#define OS_CFG_ARG_CHK_EN               1u   /* Enable (1) or Disable (0) argument checking                           */
#define OS_CFG_CALLED_FROM_ISR_CHK_EN   1u   /* Enable (1) or Disable (0) check for called from ISR                   */
#define OS_CFG_DBG_EN                   1u   /* Enable (1) debug code/variables                                       */
#define OS_CFG_ISR_POST_DEFERRED_EN     1u   /* Enable (1) or Disable (0) Deferred ISR posts                          */
#define OS_CFG_OBJ_TYPE_CHK_EN          1u   /* Enable (1) or Disable (0) object type checking                        */
#define OS_CFG_TS_EN                    1u   /* Enable (1) or Disable (0) time stamping                               */

#define OS_CFG_PEND_MULTI_EN            1u   /* 使能/禁用等待多个内核对象      */

#define OS_CFG_PRIO_MAX                32u   /* Defines the maximum number of task priorities (see OS_PRIO data type) */

#define OS_CFG_SCHED_LOCK_TIME_MEAS_EN  1u   /* Include code to measure scheduler lock time                           */
#define OS_CFG_SCHED_ROUND_ROBIN_EN     1u   /* Include code for Round-Robin scheduling                               */
#define OS_CFG_STK_SIZE_MIN            64u   /* Minimum allowable task stack size                                     */


                                             /* ----------------------------- EVENT FLAGS --------------------------- */
#define OS_CFG_FLAG_EN                  1u   /*     启用/禁用时间标志组            */
#define OS_CFG_FLAG_DEL_EN              1u   /*     启用/禁用 OSFlagDel()                                    */
#define OS_CFG_FLAG_MODE_CLR_EN         1u   /*     启用/禁用 标志位清零触发                      */
#define OS_CFG_FLAG_PEND_ABORT_EN       1u   /*     启用/禁用 OSFlagPendAbort()                              */


                                             /* -------------------------- MEMORY MANAGEMENT ------------------------ */
#define OS_CFG_MEM_EN                   1u   /* 使能/禁用 内存管理         */


                                             /* --------------------- MUTUAL EXCLUSION SEMAPHORES ------------------- */
#define OS_CFG_MUTEX_EN                 1u   /*  使能或禁用互斥信号量                 */
#define OS_CFG_MUTEX_DEL_EN             1u   /*  使能或禁用 OSMutexDel()                                   */
#define OS_CFG_MUTEX_PEND_ABORT_EN      1u   /*  使能或禁用 OSMutexPendAbort()                              */


                                             /* --------------------------- MESSAGE QUEUES -------------------------- */
#define OS_CFG_Q_EN                     1u   /* 使能或禁用消息队列                  */
#define OS_CFG_Q_DEL_EN                 1u   /*  使能或禁用 OSQDel()                                       */
#define OS_CFG_Q_FLUSH_EN               1u   /*  使能或禁用 OSQFlush()                                      */
#define OS_CFG_Q_PEND_ABORT_EN          1u   /*  使能或禁用 OSQPendAbort()                                 */


                                             /* ----------------------------- SEMAPHORES ---------------------------- */
#define OS_CFG_SEM_EN                   1u   /* 使能或禁用多值信号量             */
#define OS_CFG_SEM_DEL_EN               1u   /*  使能或禁用 OSSemDel()                                       */
#define OS_CFG_SEM_PEND_ABORT_EN        1u   /*  使能或禁用 OSSemPendAbort()                               */
#define OS_CFG_SEM_SET_EN               1u   /*  使能或禁用 OSSemSet()                                       */


                                             /* -------------------------- TASK MANAGEMENT -------------------------- */
#define OS_CFG_STAT_TASK_EN             1u   /* 使能或禁用 统计任务                         */
#define OS_CFG_STAT_TASK_STK_CHK_EN     1u   /* 使能或禁用 从统计任务检查堆栈                                */

#define OS_CFG_TASK_CHANGE_PRIO_EN      1u   /* 使能或禁用  OSTaskChangePrio                                  */
#define OS_CFG_TASK_DEL_EN              1u   /* 使能或禁用  OSTaskDel                                        */
#define OS_CFG_TASK_Q_EN                1u   /* 使能或禁用  OSTaskQxxxx                                      */
#define OS_CFG_TASK_Q_PEND_ABORT_EN     1u   /* 使能或禁用  OSTaskQPendAbort                                  */
#define OS_CFG_TASK_PROFILE_EN          1u   /* 使能或禁用  任务控制块的简况变量                            */
#define OS_CFG_TASK_REG_TBL_SIZE        1u   /* 定义任务指定的寄存器数目                                      */
#define OS_CFG_TASK_SEM_PEND_ABORT_EN   1u   /* 使能或禁用   OSTaskSemPendAbort                               */
#define OS_CFG_TASK_SUSPEND_EN          1u   /* 使能或禁用   OSTaskResume和OSTaskSuspend                 */


                                             /* -------------------------- TIME MANAGEMENT -------------------------- */
#define OS_CFG_TIME_DLY_HMSM_EN         1u   /*     Include code for OSTimeDlyHMSM()                                  */
#define OS_CFG_TIME_DLY_RESUME_EN       1u   /*     Include code for OSTimeDlyResume()                                */


                                             /* ------------------------- TIMER MANAGEMENT -------------------------- */
#define OS_CFG_TMR_EN                   1u   /* 使用/禁用软件定时器                */
#define OS_CFG_TMR_DEL_EN               1u   /* 使用/禁用OSTmrDel()函数            */

#endif
