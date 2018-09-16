#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"                  // Device header
#include "stdint.h"
#include "gpio.h"

//中断函数在stm32f10x_it.h中编写


/*

inti_mode()参数说明

EXTI_Line       
	
	中断/事件线  /  输入源
	EXTI_Line0 					P0  P为(A,B,C,D,E,F,G,H,I)  A0 B0 ...
	EXTI_Line1 					P1  P为(A,B,C,D,E,F,G,H,I)  A1 B1 ...
	EXTI_Line2 					P2  P为(A,B,C,D,E,F,G,H,I)  A2 B2 ...
	EXTI_Line3			
	EXTI_Line4			
	EXTI_Line5 			
	EXTI_Line6 			
	EXTI_Line7 			    '''''
	EXTI_Line8 			
	EXTI_Line9			
	EXTI_Line10			
	EXTI_Line11			
	EXTI_Line12 			
	EXTI_Line13			
	EXTI_Line14 			
	EXTI_Line15 					P15 P为(A,B,C,D,E,F,G,H,I)  A15 B15 ...
	EXTI_Line16 					PVD输出
	EXTI_Line17					RTC闹钟事件
	EXTI_Line18 				  USB唤醒事件
	EXTI_Line19 					以太网唤醒事件

EXTIMode_TypeDef   
	
	中断模式
	EXTI_Mode_Interrupt  产生中断
	EXTI_Mode_Event    	 产生事件

	事件：是靠脉冲发生器产生一个脉冲,进而由硬件自动完成这个事件产生的结果,当然相应的联动部件需要先设置好,比如引起DMA操作,AD转换等;
	中断：是靠软件实现直接进入中断程序
	简单举例：外部I/O触发AD转换,来测量外部物品的重量;如果使用传统的中断通道,需要I/O触发产生外部中断,外部中断服务程序启动AD转换,AD转换完成中断服务程序提交最后结果;
						要是使用事件通道,I/O触发产生事件,然后联动触发AD转换,AD转换完成中断服务程序提交最后结果;相比之下,后者不要软件参与AD触发,并且响应速度也更块;
						要是使用事件触发DMA操作,就完全不用软件参与就可以完成某些联动任务了。

EXTITrigger_TypeDef   
  
	中断触发类型
  EXTI_Trigger_Rising    上升沿触发（低电平变为高电平）
  EXTI_Trigger_Falling   下降沿触发
  EXTI_Trigger_Rising_Falling   以上两者都触发

EXTI_LineCmd   
	
	使能
	DISABLE  关闭
	ENABLE   开启
*/




/*
inti_turn()参数说明


NVIC_IRQChannel
	
	中断源
	EXTI0_IRQn  		EXTI0线中断
	EXTI1_IRQn			EXTI1线中断
	EXTI2_IRQn			EXTI2线中断
	EXTI3_IRQn			EXTI3线中断
	EXTI4_IRQn			EXTI4线中断
	EXTI9_5_IRQn		[EXTI5,EXTI9]线中断
	EXTI15_10_IRQn	[EXTI10,EXTI5]线中断

NVIC_IRQChannelPreemptionPriority

	抢占优先级	
	因为leon_init();中设置了NVIC_PriorityGroup_2分组，所以主优先为[0,3]

NVIC_IRQChannelSubPriority

	子优先级
	同上，也是[0,3]

	优先级的判断：优先级越小优先级越高，先比抢占优先级，若相同，再比子优先级，若也相同，则有硬件号决定，系统滴答时钟优先级15化为11，11，则抢占优先级为3，子优先级也为3

NVIC_IRQChannelCmd

	使能
	DISABLE  关闭
	ENABLE   开启

*/


class EXTI_GPIO
{
	public:
	void inti_pin(GPIO *exti_gpio);
	//   中断引脚初始化即外部中断引脚   初始化数字输入上拉引脚
	void inti_souce(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);  //GPIO_PortSourceGPIO[A-G]  GPIO_PinSource[0-15]   例如 inti_souce(GPIO_PortSourceGPIOE,GPIO_PinSource1)
	//   中断引脚源初始化即外部中断引脚
	void inti_mode(uint32_t EXTI_Line , EXTIMode_TypeDef EXTI_Mode , EXTITrigger_TypeDef EXTI_Trigger , FunctionalState EXTI_LineCmd);  
	//  中断初始化   中断/事件线	          EXTI模式	                            触发类型                        EXTI使能
	void inti_turn(uint8_t NVIC_IRQChannel , uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd);  
	//中断顺序初始化        配置中断源											配置抢占优先级													配置子优先级									中断通道使能
	
	
};



#endif
