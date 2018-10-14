#ifndef __User_main_H__
#define __User_main_H__

#include <includes.h>
#include "config.h"


extern OS_MEM   mem;
extern uint8_t ucArray [ 70 ] [ 4 ];   //�����ڴ������С



//���Key1 ��������
extern  OS_TCB   Key1_Scan_TCB;
static void    Key1_Scan(void *p_arg);
#define  Key1_Scan_PRIO  3
#define  Key1_Scan_STK_SIZE 64
static CPU_STK   Key1_Scan_STK[Key1_Scan_STK_SIZE];  


//USART1_Get ��������
extern  OS_TCB  USART1_Get_TCB;
static void    USART1_Get(void *p_arg);
#define  USART1_Get_PRIO  4
#define  USART1_Get_STK_SIZE 64
static CPU_STK   USART1_Get_STK[Key1_Scan_STK_SIZE];  










//�����������е���
void User_main(void);



#endif



