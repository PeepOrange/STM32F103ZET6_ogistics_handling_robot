#ifndef __User_main_H__
#define __User_main_H__

#include <includes.h>
#include "config.h"



typedef struct 
{
    uint8_t up_Steering_angle;
    uint8_t mid_Steering_angle;
    uint8_t down_Steering_angle;
    uint8_t hand_Steering_angle;
}
Steering_engine_angle_Struct;

extern Steering_engine_angle_Struct  Steering_engine_angle;
extern OS_MEM   mem;
extern uint8_t ucArray [ 4 ] [ 4 ];   //�����ڴ������С


//���Key1 ��������
extern  OS_TCB   Key1_Scan_TCB;
static void    Key1_Scan(void *p_arg);
#define  Key1_Scan_PRIO  3
#define  Key1_Scan_STK_SIZE 128
static CPU_STK   Key1_Scan_STK[Key1_Scan_STK_SIZE];  


//USART1_Get ��������
extern  OS_TCB  USART1_Get_TCB;
static void    USART1_Get(void *p_arg);
#define  USART1_Get_PRIO  4
#define  USART1_Get_STK_SIZE 64
static CPU_STK   USART1_Get_STK[Key1_Scan_STK_SIZE];  



//ѭ��+PID ��������   
extern  OS_TCB  Run_TCB;
static void    Run(void *p_arg);
#define  Run_PRIO  1
#define  Run_STK_SIZE 256
static CPU_STK   Run_STK[Run_STK_SIZE];  



//��λ ��������   
extern  OS_TCB  Postion_TCB;
static void    Postion(void *p_arg);
#define  Postion_PRIO  1
#define  Postion_STK_SIZE 256
static CPU_STK   Postion_STK[Postion_STK_SIZE];  






//�����������е���
void User_main(void);



#endif



