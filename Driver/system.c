#include "system.h"
void system_init()  //ϵͳ��ʼ��
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//˳��Ҫ�ȴ򿪸���ʱ�ӣ��ٹر�JTAGE
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//��ֹJTAG���ܣ���PB3��PB4��Ϊ��ͨIO��ʹ��  
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);    //��ʱ��2��ȫ��ӳ��  A15 B3  B10  B11
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//��ֹJTAG���ܣ���PB3��PB4��Ϊ��ͨIO��ʹ��
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);    //��ʱ��1��ȫ��ӳ��  E9  E11 E13  E14
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG,ENABLE);	 //ʹ�ܶ˿�ʱ��
}   

void SysRestart(void) //ϵͳ����
{
	__disable_fault_irq();  
	NVIC_SystemReset();
}


void delay_us(__IO uint32_t us)
{		
	uint32_t i;
	SysTick_Config(SystemCoreClock/1000000);  //����ϵͳ�δ�ʱ���ж� �����ֵ  72M/1e6  ÿ�� 1/72M��s����1������ÿ1us����һ���ж� 
	for (i=0; i<us; i++) 
	{
	while ( !((SysTick->CTRL)&(1<<16)) );     //������������0ʱ CRTL�Ĵ�����BIT16����λ
	}

	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;   //�ر�ϵͳ�δ�ʱ���ж�
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
