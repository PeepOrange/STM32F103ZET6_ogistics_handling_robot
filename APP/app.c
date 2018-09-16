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

    OSInit(&err);                                               /* 初始化uC/OS-III                                     */
    //创建起始任务
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* 任务控制块地址                               */
                 (CPU_CHAR   *)"App Task Start",                //任务名字
                 (OS_TASK_PTR ) AppTaskStart,                   //任务函数
                 (void       *) 0,                              //传递给任务函数的实参（形参p_arg）
                 (OS_PRIO     ) APP_TASK_START_PRIO,            //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],             //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,   //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,        //任务的堆栈空间（单位：size(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                             //任务可接受的最大消息数
                 (OS_TICK     ) 0u,                             //任务的时间片节拍数（默认值0表示 OS_CFG_TICK_RATE_HZ/10  即10ms ）
                 (void       *) 0,                              //任务拓展（0表示不拓展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),     //任务选项
                 (OS_ERR     *)&err);                           //返回错误类型

    OSStart(&err);                                              /*启动多任务管理（交给uC/OS-III管理） */
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

    BSP_Init();                                                 /* 初始化硬件                             */
    CPU_Init();                                                 //初始化CPU组件（时间戳、关中断时间测量和主机名）

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* 获取CPU内核时钟频率(Systick工作时钟)                  */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* 根据用户定义的时钟频率节拍计算Systick定时器的基础值                    */
    OS_CPU_SysTickInit(cnts);                                   /* 启动Systick定时器并且填入初值              */

    Mem_Init();                                                 /* 初始化内存管理组件（堆内存池和内存池表）                 */

#if OS_CFG_STAT_TASK_EN > 0u                                    /* 如果使能统计任务功能（默认使能）        */
    OSStatTaskCPUUsageInit(&err);  //计算没有应用任务（只有空闲任务）运行时cpu最大容量（OS_Stat_IdleCtrMax）,为后面计算cpu使用率做准备                           
#endif

    CPU_IntDisMeasMaxCurReset();                                //复位当前最大关中断时间
    
    User_main();
          
    OSTaskDel(&AppTaskStartTCB,&err);              

}

















