#ifndef __tim_H
#define __tim_H
#include "stm32f10x.h"                  // Device header

#define	TIM_channel1	1
#define	TIM_channel2	2
#define	TIM_channel3	3
#define	TIM_channel4	4


/*	定时器可以从上往下兼容	即说通用定时器可以兼容基础定时器配置
高级定时器		TIM1	TIM8							16位		向上/向下		4个比较/捕获通道		有互补输出
通用定时器		TIM2	TIM3	TIM4	TIM5	16位		向上/向下		4个比较/捕获通道		无互补输出
基本定时器		TIM6	TIM7							16位		向上				 无比较/捕获通道		无互补输出
*/

/*
			高级定时器 							通用定时器
			TIM1 						TIM8 		TIM2 				TIM5 		TIM3 					TIM4
CH1 	PA8/PE9 				PC6 		PA0/PA15 		PA0 		PA6/PC6/PB4 	PB6/PD12
CH1N 	PB13/PA7/PE8 		PA7
CH2 	PA9/PE11 				PC7 		PA1/PB3 		PA1 		PA7/PC7/PB5 	PB7/PD13
CH2N 	PB14/PB0/PE10 	PB0
CH3 	PA10/PE13 			PC8 		PA2/PB10 		PA2 		PB0/PC8 			PB8/PD14
CH3N 	PB15/PB1/PE12 	PB1
CH4 	PA11/PE14 			PC9 		PA3/PB11 		PA3 		PB1/PC9 			PB9/PD15
ETR 	PA12/PE7 				PA0 		PA0/PA15 						PD2 					PE0
BKIN 	PB12/PA6/PE15 	PA6

*/

/*各定时器基本功能	高级定时器兼容低级定时器的功能
基本定时器		可以设置一个时间，等到这个时间就发生中断
通用定时器		可以生成PWM（周期、频率可控），可以捕获PWM或脉冲	
高级定时器		可以生成互补输出，可以产生死区，可以产生断路（刹车）
*/

/*
base()参数说明

	TIMx		
	基础定时器支持所用定时器
	
	RCC_APB1Periph
	定时器时钟周期		RCC_APB1Periph_TIMx		如：	RCC_APB1Periph_TIM6	/	RCC_APB1Periph_TIM7
	
	NewState
	使能							DISABLE		/		ENABLE

	TIM_Prescaler
	预分频器		[1-65536]	
	时钟源经该预分频器才是定时器时钟		正常时钟源为72Mhz，所以配置为72即为1us记数一次

	TIM_Period
	时长				[1-65536]
	设定定时器的时长，即定时器记到这个数字产生中断
	
	NVIC_IRQChannel	
	 TIM1_BRK_IRQn 			TIM1断开中断
	 TIM1_UP_IRQn    		TIM1更新中断
	 TIM1_TRG_COM_IRQn  TIM1触发和通信中断 
	 TIM1_CC_IRQn 			TIM1捕获比较中断
	 TIM2_IRQn   				TIM2全局中断
	 TIM3_IRQn   				TIM3全局中断
	 TIM4_IRQn   				TIM4全局中断
	
	NVIC_IRQChannelPreemptionPriority
	抢占优先级						因为system_inti()设置为2;2 所以	此值为[0,3]
	
	NVIC_IRQChannelSubPriority
	子优先级							同上，可以设置为[0,3],优先级具体看EXTI.h文件的注释
	
*/



class TIM_Base
{
	public:
			void base																						//基本定时器配置 自行在stm32f10x_it.cpp中编写中断函数，当溢出时产生中断
			(
			TIM_TypeDef* TIMx,																	//定时器编号 
			uint32_t RCC_APB1Periph,														//定时器时钟设置
			FunctionalState NewState,														//使能定时器
			uint16_t TIM_Prescaler,															//定时器预分频
			uint16_t TIM_Period	,																//定时器记数点
			uint8_t NVIC_IRQChannel,														//中断源
			uint8_t NVIC_IRQChannelPreemptionPriority,					//抢占优先级
			uint8_t NVIC_IRQChannelSubPriority									//子优先级
			);		
			//	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE)	初始化完成后调用此函数打开定时器，让定时工作，之后也可关闭，使定时器暂停
			//	STM32F10X_MD	这块板子无TIM6和TIM7的中断
			//	STM32F10X_MD	只有TIM1、TIM2、TIM3、TIM4
	
};


/*
	base()参数说明
	TIMx												定时器编号
	通用计时器除了TIM6、TIM7基础计时器，其余都支持
	
	RCC_APB1Periph							定时器时钟
	RCC_APB1Periph_TIMx		如：	RCC_APB1Periph_TIM6	/	RCC_APB1Periph_TIM7
	
	TIM_Prescaler								定时器预分频			
	预分频器		[1-65536]	
	时钟源经该预分频器才是定时器时钟		如果	TIM_ClockDivision为TIM_CKD_DIV1	正常时钟源为72Mhz，所以配置为72即为1us记数一次	

	TIM_CounterMode							定时器记数方式
	TIM_CounterMode_Up									向上计数
	TIM_CounterMode_Down								向下计数
	TIM_CounterMode_CenterAligned1			中央对齐模式1：计数器交替地向上和向下计数。输出比较中断标志位，只在计数器向下计数时被设置
	TIM_CounterMode_CenterAligned2			中央对齐模式2: 计数器交替地向上和向下计数。输出比较中断标志位，只在计数器向上计数时被设置
	TIM_CounterMode_CenterAligned3			中央对齐模式3: 计数器交替地向上和向下计数。输出比较中断标志位，只在计数器向下和向上计数时均被设置
	
	TIM_Period									定时器周期
	时长				[1-65536]
	设定定时器的周期	

	TIM_ClockDivision						时钟分频
	TIM_CKD_DIV1								1分频
	TIM_CKD_DIV2								2分频
	TIM_CKD_DIV4								4分频
	设置定时器时钟 CK_INT 频率与死区发生器以及数字滤波器采样时钟频率分频比。可以选择 1、2、4分频。
	产生普通的PWM设置为	TIM_CKD_DIV1	即可
*/


/*
out_pwm_inti()
		TIM_TypeDef* TIMx																			//定时器编号
		通用计时器除了TIM6、TIM7基础计时器，其余都支持		
		
		uint8_t  TIM_Channel																	//定时器输出通道
		TIM_channel1			通道1
		TIM_channel2			通道2
		TIM_channel3			通道3
		TIM_channel4			通道4
		
		uint16_t TIM_OCMode								//比较输出模式
		TIM_OCMode_Timing									输出比较时间模式 (输出引脚冻结无效)
		TIM_OCMode_Active									输出比较主动模式   (匹配时设置输出引脚为有效电平，当计数值为比较/捕获寄存器值相同时，强制输出为高电平)
		TIM_OCMode_Inactive								输出比较非主动模式 (匹配时设置输出引脚为无效电平，当计数值为比较/捕获寄存器值相同时，强制输出为低电平)
		TIM_OCMode_Toggle									输出比较触发模式    (翻转。当计数值与比较/捕获寄存器值相同时，翻转输出引脚的电平)
		TIM_OCMode_PWM1										向上计数时，当TIMx_CNT（已经记得数） < TIMx_CCR（TIM_Pulse设置的值）时，输出电平有效，否则为无效向下计数时，当TIMx_CNT > TIMx_CCR时，输出电平无效，否则为有效
		TIM_OCMode_PWM2										与上面相反
		
		uint16_t TIM_OutputState															//比较输出使能
		TIM_OutputState_Disable			取消输出到引脚
		TIM_OutputState_Enable			使能输出到引脚（正常）
		
		uint32_t TIM_Pulse																		//脉冲宽度
		PWM的脉冲宽度，与TIM_Period共同计算可以得出频率和占空比
		[0,65535]     占空比为TIM_Pulse/TIM_Period , 因此TIM_Pulse不能超过TIM_Period，即不可能超过100%
		
		uint16_t TIM_OCPolarity																//输出极性
		它决定着定时器通道有效电平，是高电平有效还是低电平有效
		TIM_OCPolarity_High		高电平有效
		TIM_OCPolarity_Low		低电平有效
		
		uint16_t TIM_OCIdleState															//空闲状态下比较输出状态
		TIM_OCIdleState_Set				空闲时置位
		TIM_OCIdleState_Reset			空闲时复位
*/


/*
in_pulse_inti()参数说明

	TIMx					定时器编号	如TIM1

	TIM_Channel		输入通道
	TIM_Channel_1		通道1
	TIM_Channel_2		通道2
	TIM_Channel_3		通道3
	TIM_Channel_4		通道4
		
	TIM_ICPolarity						捕获触发
	TIM_ICPolarity_Rising			上升沿触发
	TIM_ICPolarity_Falling		下降沿触发
	TIM_ICPolarity_BothEdge		双边沿触发
	
	TIM_ICSelection							输入捕获选择
	TIM_ICSelection_DirectTI		直接映射同通道	ICx映射到TIx
	TIM_ICSelection_IndirectTI	间接映射到另外一个通道	ICx映射到TIy		1对2，3对4		
	TIM_ICSelection_TRC					映射到同通道的TRC上
	
	TIM_ICPrescaler
	TIM_ICPSC_DIV1				一个边沿算一个有效边沿
	TIM_ICPSC_DIV2				二个边沿算一个有效边沿
	TIM_ICPSC_DIV4				四个边沿算一个有效边沿
	TIM_ICPSC_DIV8				八个边沿算一个有效边沿
	
	TIM_ICFilter
	[0,15]		输入捕获滤波器,防止杂波,正常为0
	
	TIM_FLAG						中断标志位
	TIM_FLAG_Update			更新事件标志
	TIM_FLAG_CC1				IC1捕获事件产生标志
	TIM_FLAG_CC2				IC2捕捉事件产生标志
	TIM_FLAG_CC3				IC3捕捉事件产生标志
	TIM_FLAG_CC4				IC4捕捉事件产生标志
	TIM_FLAG_COM				COM事件(当捕获/比较控制位：CCxE、CCxNE、OCxM已被更新)产生标志
	TIM_FLAG_Trigger		触发事件(当从模式控制器处于除门控模式外的其它模式时，在TRGI输入端检测到有效边沿，或门控模式下的任一边沿)产生标志
	TIM_FLAG_Break			刹车事件标志
	TIM_FLAG_CC1OF			捕获/比较1重复捕获产生标记
	TIM_FLAG_CC2OF			捕获/比较2重复捕获产生标记
	TIM_FLAG_CC3OF			捕获/比较3重复捕获产生标记
	TIM_FLAG_CC4OF			捕获/比较4重复捕获产生标记
	
	 NVIC_IRQChannel		中断源
	 TIM1_BRK_IRQn 			TIM1断开中断
	 TIM1_UP_IRQn    		TIM1更新中断
	 TIM1_TRG_COM_IRQn  TIM1触发和通信中断 
	 TIM1_CC_IRQn 			TIM1捕获比较中断
	 TIM2_IRQn   				TIM2全局中断
	 TIM3_IRQn   				TIM3全局中断
	 TIM4_IRQn   				TIM4全局中断
	
	 NVIC_IRQChannelPreemptionPriority
	 抢占优先级						因为system_inti()设置为2;2 所以	此值为[0,3]
	
	 NVIC_IRQChannelSubPriority
	 子优先级							同上，可以设置为[0,3],优先级具体看EXTI.h文件的注释	 
*/

/*in_pwm_inti()参数说明

	TIMx					定时器编号	如TIM1

	TIM_Channel		输入通道
	TIM_Channel_1		通道1
	TIM_Channel_2		通道2
	TIM_Channel_3		通道3
	TIM_Channel_4		通道4
		
	TIM_ICPolarity						捕获触发
	TIM_ICPolarity_Rising			上升沿触发
	TIM_ICPolarity_Falling		下降沿触发
	TIM_ICPolarity_BothEdge		双边沿触发
	
	TIM_ICSelection							输入捕获选择
	TIM_ICSelection_DirectTI		直接映射同通道	ICx映射到TIx
	TIM_ICSelection_IndirectTI	间接映射到另外一个通道	ICx映射到TIy		1对2，3对4		
	TIM_ICSelection_TRC					映射到同通道的TRC上
	
	TIM_ICPrescaler
	TIM_ICPSC_DIV1				一个边沿算一个有效边沿
	TIM_ICPSC_DIV2				二个边沿算一个有效边沿
	TIM_ICPSC_DIV4				四个边沿算一个有效边沿
	TIM_ICPSC_DIV8				八个边沿算一个有效边沿
	
	TIM_IT
	TIM_FLAG_Update			更新事件标志（不常用）
	TIM_FLAG_CC1				IC1捕获事件产生标志
	TIM_FLAG_CC2				IC2捕捉事件产生标志
	TIM_FLAG_CC3				IC3捕捉事件产生标志
	TIM_FLAG_CC4				IC4捕捉事件产生标志	
	
	
	TIM_ICFilter
	[0,15]		输入捕获滤波器,防止杂波,正常为0


*/




class		TIM_Currency																														
{
	// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE)	调用此函数打开定时器，让定时工作，之后也可关闭，使定时器暂停,默认开启
	public:
		void base																							//通用计时器配置	
		(
			TIM_TypeDef* TIMx,																	//定时器编号 
			uint32_t RCC_APB1Periph,														//定时器时钟设置
			uint16_t TIM_Prescaler,															//定时器预分频
			uint16_t TIM_CounterMode,														//定时器记数方式
			uint16_t TIM_Period,																//定时器周期
			uint16_t TIM_ClockDivision													//时钟分频			
		);
		
		void out_pwm_inti																			//输出PWM初始化，需要把相对应的引脚设置为GPIO_Mode_AF_PP	复用推挽输出，并且先初始化base()函数
		(
		TIM_TypeDef* TIMx,																		//定时器编号
		uint8_t  TIM_Channel,																	//定时器输出通道
		uint16_t TIM_OCMode,																	//比较输出模式
		uint16_t TIM_OutputState,															//比较输出使能
		uint32_t TIM_Pulse,																		//脉冲宽度
		uint16_t TIM_OCPolarity,															//输出极性
		uint16_t TIM_OCIdleState															//空闲状态下比较输出状态
		);
		
		void out_pwm																					//变化PWM的周期和占空比,先初始化out_pwm_inti()才能使用
		(			
			TIM_TypeDef* TIMx,							//TIM编号		TIMx							
			uint8_t  TIM_Channel,						//TIM通道		TIM_channelx
			uint16_t TIM_Period,						//TIM周期		[0,65535]
			uint32_t TIM_Pulse							//TIM占空比	[0,65535]
		);
		
		void	out_pwm_per																			//只改变PWM的占空比，先初始化out_pwm_inti()才能使用
		(
			TIM_TypeDef* TIMx,
			uint8_t  TIM_Channel,
			uint32_t TIM_Pulse
		);
		
		void	in_pulse_inti																		//捕捉输入脉冲，需要先初始化base()函数，对应引脚初始化为	GPIO_Mode_IN_FLOATING	浮空输入
		(
			TIM_TypeDef* TIMx,																	//定时器编号 
			uint16_t TIM_Channel,																//输入通道
			uint16_t TIM_ICPolarity,														//输入捕获触发
			uint16_t TIM_ICSelection,														//输入捕获选择
			uint16_t TIM_ICPrescaler,														//输入捕获预分频器
			uint16_t TIM_ICFilter,															//输入捕获滤波器
			uint16_t TIM_FLAG,																	//中断标志位	正常为	TIM_FLAG_CCx	x为你的通道号
			uint8_t NVIC_IRQChannel,														//中断源
			uint8_t NVIC_IRQChannelPreemptionPriority,					//抢占优先级
			uint8_t NVIC_IRQChannelSubPriority									//子优先级
		);
		
		
		
		void	in_pwm_inti																			//捕捉输入PWM，需要先初始化base()函数，对应引脚初始化 GPIO_Mode_AF_PP 复用推挽输出
		(
		TIM_TypeDef* TIMx,																		//定时器编号
		uint16_t TIM_Channel,																	//输入通道
		uint16_t TIM_ICPolarity,															//输入捕获触发
		uint16_t TIM_ICSelection,															//输入捕获选择
		uint16_t TIM_ICPrescaler,															//输入捕获预分频器
		uint16_t TIM_IT,																			//主捕获通道（测量周期那个通道）捕获中断
		uint16_t TIM_ICFilter																	//输入捕获滤波器
		);
		//测量的PWM周期最高只能为	72M/TIM_Period	，TIM_Period在base()常设置位72，所以最长周期只能为1ms
		
};



/*高级定时器功能特点刹车和死区和互补输出，使用不多，用时再说
以下为参考
	// 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述
	// 这里配置的死区时间为 152ns
	TIM_BDTRInitStructure.TIM_DeadTime = 11;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// 当 BKIN 引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	互补输出实现如同通用计时器的PWM输出，只不过将	TIM_OC1Init()要求的结构体补充完全即可


class	TIM_Senior
{
	public:
		void base																							//高级计时器配置	
		(
			TIM_TypeDef* TIMx,																	//定时器编号 
			uint32_t RCC_APB1Periph,														//定时器时钟设置
			uint16_t TIM_Prescaler,															//定时器预分频
			uint16_t TIM_CounterMode,														//定时器记数方式
			uint16_t TIM_Period,																//定时器周期
			uint16_t TIM_ClockDivision													//时钟分频			
		);
};
*/



#endif
