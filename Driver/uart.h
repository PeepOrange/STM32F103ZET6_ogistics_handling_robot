#ifndef __uart_H
#define __uart_H
#include "stm32f10x.h"                  // Device header
#include "gpio.h"

#define USART_Mode_ALL   USART_Mode_Rx|USART_Mode_Tx

/*
inti_mode  //异步通信只要此函数初始化即可  若同步通信要在调用	USART_ClockInitTypeDef 结构体初始化
	串口通讯模式初始化
	USART_BaudRate 波特率
	常见值： 2400、9600、19200、115200
	
	USART_WordLength	字长
	无奇偶校检位 USART_WordLength_8b	8bit
	有奇偶校检位 USART_WordLength_9b	9bit
	
	USART_StopBits	停止位
	USART_StopBits_1    1位		常见
	USART_StopBits_0_5  0.5位 
	USART_StopBits_2		2位	
	USART_StopBits_1_5	1.5位
	
	USART_Parity		校检位
	USART_Parity_No			无校检
	USART_Parity_Even		偶校检
	USART_Parity_Odd		奇校检
	
	USART_Mode		USART模式	
	USART_Mode_Rx		接受模式
	USART_Mode_Tx		发送模式
	USART_Mode_ALL	接受/发送模式
	
	USART_HardwareFlowControl						硬件流控制
	USART_HardwareFlowControl_None			不使能硬件流
	USART_HardwareFlowControl_RTS				使能RTS
	USART_HardwareFlowControl_CTS				使能CTS
	USART_HardwareFlowControl_RTS_CTS		RTS和CTS都使能
	
*/




/*
inti_it_turn()参数说明


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
	
	
USART_IT	
	中断触发方式
	 USART_IT_PE      奇偶错误中断          
	 USART_IT_TXE     发送中断      
	 USART_IT_TC      发送完成中断          
	 USART_IT_RXNE    接受中断       
	 USART_IT_IDLE    空闲主线中断          
	 USART_IT_LBD     LIN中断探测中断      
	 USART_IT_CTS     CTS中断                 
	 USART_IT_ORE     溢出错误中断                     
	 USART_IT_NE      噪音错误中断                  
	 USART_IT_FE      帧错误中断        

*/

class UART
{
	public:

	UART(GPIO *RX,GPIO *TX);   //开启USART1时钟，并且令RX为数字输入浮空  TX为数字推挽输出  TX和RX是uart的tx和rx引脚  TX1:PA9  RX1:PA10
	void inti_mode(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
	//  串口通讯模式初始化     波特率										字长										停止位										校检位								USART模式									硬件流控制
	static void sent_byte(uint8_t ch);   //USART1发送一个字节
	static void sent_str(const char *str);  //USART1发送一个字符串 
	static uint16_t receive_data();  //USART1收到的数据
	void inti_it_turn( uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority, FunctionalState NVIC_IRQChannelCmd,uint16_t USART_IT);  
	//    串口中断开启    				配置抢占优先级													配置子优先级											中断通道使能									中断触发方式
	
	private:
	GPIO *RX;
	GPIO *TX;
		
	
};




#endif
