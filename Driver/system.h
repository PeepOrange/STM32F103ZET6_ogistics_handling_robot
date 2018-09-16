#ifndef __system_H
#define __system_H 			   
#include "stm32f10x.h" 


//Sysclk系统时钟72M
//HCLK(AHB)72M  HCLK1(APB1)36M  HCLK2(APB2)72M
//cpu主频率     低速外设I2C1/2   高速外设 GPIO
//系统滴答定时器优先级15
//中断组别设置为	NVIC_PriorityGroup_2	抢占优先级为[0,3]	子优先级为[0,3]

void system_init(void);//系统初始化
void SysRestart(void); //系统重启 
void delay_us(__IO uint32_t us);   //利用定时器延时1us  
void delay_ms(__IO uint32_t ms);   //利用定时器延时1ms



#endif
