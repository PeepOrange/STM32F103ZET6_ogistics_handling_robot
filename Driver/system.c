#include "system.h"
void system_init()  //系统初始化
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//顺序，要先打开复用时钟，再关闭JTAGE
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//禁止JTAG功能，把PB3，PB4作为普通IO口使用  
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);    //定时器2完全重映射  A15 B3  B10  B11
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//禁止JTAG功能，把PB3，PB4作为普通IO口使用
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);    //定时器1完全重映射  E9  E11 E13  E14
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);	 //使能端口时钟
}   

void SysRestart(void) //系统重启
{
	__disable_fault_irq();  
	NVIC_SystemReset();
}


void delay_us(__IO uint32_t us)
{		
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);  //开启系统滴答定时器中断 填入初值  72M/1e6  每过 1/72M（s）减1，所以每1us进入一次中断 
	for (i=0; i<us; i++) 
	{
	while ( !((SysTick->CTRL)&(1<<16)) );     //当计数器减到0时 CRTL寄存器的BIT16会置位
	}

	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;   //关闭系统滴答定时器中断
}

void delay_ms(__IO uint32_t ms)
{	 		  	  
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000);
	for (i=0; i<ms; i++) 
	{
	while ( !((SysTick->CTRL)&(1<<16)) );
	}

	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}   



/*
void delay_init(void)
{
	time.msPeriod = 0;
	time.ticksPerUs = SystemCoreClock / 1e6;
	time.ticksPerMs = SystemCoreClock / 1e3;
	time.msPerPeriod = 10;
	SysTick_Config(SystemCoreClock/(1000/time.msPerPeriod));//????????,?????????
}
// SysTick???
void SysTick_Handler(void)
{
    time.msPeriod += time.msPerPeriod;
}
//??????,us?
uint64_t time_nowus(void)
{
    return time.msPeriod * (uint64_t)1000 + (SysTick->LOAD - SysTick->VAL) / time.ticksPerUs;
}
//??????,ms?
uint32_t time_nowms(void)
{
    return time.msPeriod + (SysTick->LOAD - SysTick->VAL) / time.ticksPerMs;
}

//??nus,nus?????us??	    								   
void delay_us(u32 delay)
{		
    uint64_t target = time_nowus() + delay - 2;
    while(time_nowus() <= target)
    ; // ???
}
//??ms,nms?????ms??	    	
void delay_ms(u16 delay)
{	 		  	  
	delay_us(delay * 1000); 	    
}   
//????
void delay(vu32 nCount)
{
  for(nCount; nCount!= 0;nCount--);
}
*/
