#include "config.h"
#include "gpio.h"
#include "oled.h"
#include "EXTI.h"
#include "uart.h"
#include "dma.h"
#include "iic_cs.h"
#include "adc.h"
#include "tim.h"
#include "dac.h"
#include "spi.h"
#include "PID.h"
#include "flash.h"
#include "ILI9341_LCD.h"
#include "OV7725.h"
#include "ESP8266.h"
#include "W25Q64.h"

#ifdef __cplusplus
extern "C"
{
#include "system.h"
#include "stm32f10x_it.h"
#include "stm32f10x.h"                  // Device header
};
#endif



#ifdef __cplusplus
extern "C"
{
#endif 


    
    
    

void LED_Init()
{

    
    GPIO LED1(GPIOE,GPIO_Pin_5);
    GPIO LED2(GPIOE,GPIO_Pin_6);

    
    LED1.mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    LED2.mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

    LED1.set();

    LED2.set();
    
    
}    
    
    
void LED_ALL_OFF()
{
    GPIO LED1(GPIOE,GPIO_Pin_5);
    GPIO LED2(GPIOE,GPIO_Pin_6);
    
    LED1.set();
    LED2.set();
}    
    
    
void LED1_Toggle()
{
    
    GPIO LED1(GPIOE,GPIO_Pin_5);
    LED1.toggle();
    
}
    
void LED2_Toggle()
{
    
    GPIO LED2(GPIOE,GPIO_Pin_6);
    LED2.toggle();
    
}


    
void Uart_Init()
{
 
    GPIO Tx(GPIOA,GPIO_Pin_9);
    GPIO Rx(GPIOA,GPIO_Pin_10);
    UART uart1(&Rx,&Tx);
    uart1.inti_mode(115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_ALL,USART_HardwareFlowControl_None);
    uart1.inti_it_turn(1,0,ENABLE,USART_IT_RXNE);
    
}    
    
    
void Key_Init()
{
    GPIO Key1(GPIOA,GPIO_Pin_0);
    GPIO Key2(GPIOE,GPIO_Pin_4);
    
    Key1.mode(GPIO_Mode_IPD,GPIO_Speed_50MHz);
    Key2.mode(GPIO_Mode_IPU,GPIO_Speed_50MHz);
    
   EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
    
    //按键 Key1 A0 中断函数配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //按键 Key2 E4 中断函数配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStructure);
    
    //按键 Key1 A0 中断源配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;     //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);    
    
    //按键 Key2 E4 中断源配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4); 
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);
}    
    
    

void OutPWM_TIM3_Init()
{
   TIM_TimeBaseInitTypeDef	tim_base;
    
	tim_base.TIM_Prescaler=72-1;        //72分频
	tim_base.TIM_Period=1000-1;         //周期1000us
	tim_base.TIM_CounterMode=TIM_CounterMode_Up;    //向上记数
	tim_base.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟不分频
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInit(TIM3,&tim_base );
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
 
    
    TIM_OCInitTypeDef	tim_oc_mode;
	tim_oc_mode.TIM_OCMode=TIM_OCMode_PWM1;             //PWM输出模式
	tim_oc_mode.TIM_OutputState=TIM_OutputState_Enable; 
	tim_oc_mode.TIM_Pulse=0;                            //一开始的PWM为0
	tim_oc_mode.TIM_OCPolarity=TIM_OCPolarity_High;     //高电平为有效电平
	tim_oc_mode.TIM_OCIdleState=TIM_OCIdleState_Reset;
    
    TIM_OC1Init(TIM3,&tim_oc_mode);         //A6
    TIM_OC2Init(TIM3,&tim_oc_mode);         //A7
    TIM_OC3Init(TIM3,&tim_oc_mode);         //B0
    TIM_OC4Init(TIM3,&tim_oc_mode);         //B1
    
    TIM_Cmd(TIM3,ENABLE);       //开启定时器3  
    
}



void GPIO_TIM3_Init()
{
    GPIO A6(GPIOA,GPIO_Pin_6);
    A6.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO A7(GPIOA,GPIO_Pin_7);
    A7.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO B0(GPIOB,GPIO_Pin_0);
    B0.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);    
    GPIO B1(GPIOB,GPIO_Pin_1);
    B1.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);   
}




void OutPWM_TIM4_Init()
{
   TIM_TimeBaseInitTypeDef	tim_base;
    
	tim_base.TIM_Prescaler=72-1;        //72分频
	tim_base.TIM_Period=1000-1;         //周期1000us
	tim_base.TIM_CounterMode=TIM_CounterMode_Up;    //向上记数
	tim_base.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟不分频
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInit(TIM4,&tim_base );
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
 
    
    TIM_OCInitTypeDef	tim_oc_mode;
	tim_oc_mode.TIM_OCMode=TIM_OCMode_PWM1;             //PWM输出模式
	tim_oc_mode.TIM_OutputState=TIM_OutputState_Enable; 
	tim_oc_mode.TIM_Pulse=0;                            //一开始的PWM为0
	tim_oc_mode.TIM_OCPolarity=TIM_OCPolarity_High;     //高电平为有效电平
	tim_oc_mode.TIM_OCIdleState=TIM_OCIdleState_Reset;
    
    TIM_OC1Init(TIM4,&tim_oc_mode);         //B6
    TIM_OC2Init(TIM4,&tim_oc_mode);         //B7
    TIM_OC3Init(TIM4,&tim_oc_mode);         //B8
    TIM_OC4Init(TIM4,&tim_oc_mode);         //B9
    
    TIM_Cmd(TIM4,ENABLE);       //开启定时器4   
    
}



void GPIO_TIM4_Init()
{
    GPIO B6(GPIOB,GPIO_Pin_6);
    B6.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO B7(GPIOB,GPIO_Pin_7);
    B7.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO B8(GPIOB,GPIO_Pin_8);
    B8.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);    
    GPIO B9(GPIOB,GPIO_Pin_9);
    B9.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);   
}





void PWM_Init()
{
    GPIO_TIM3_Init();

    OutPWM_TIM3_Init(); 

    GPIO_TIM4_Init (); 
    
    OutPWM_TIM4_Init();
    
}



void Move_Up(  )
{
    
    TIM3->CCR1 =Move_Speed;
    TIM3->CCR2 =Move_Speed;
    TIM3->CCR3 =Move_Speed;
    TIM3->CCR4 =Move_Speed;
    
    TIM4->CCR1=0;
    TIM4->CCR2=0;
    TIM4->CCR3=0;
    TIM4->CCR4=0;
 
}




void Move_Back(  )
{
    
    TIM4->CCR1 =Move_Speed;
    TIM4->CCR2 =Move_Speed;
    TIM4->CCR3 =Move_Speed;
    TIM4->CCR4 =Move_Speed;
    
    TIM3->CCR1=0;
    TIM3->CCR2=0;
    TIM3->CCR3=0;
    TIM3->CCR4=0;
 
}




void Move_Left(  )
{
    
    TIM4->CCR1 =Move_Speed;
    TIM3->CCR2 =Move_Speed;
    TIM3->CCR3 =Move_Speed;
    TIM4->CCR4 =Move_Speed;
    
    TIM3->CCR1=0;
    TIM4->CCR2=0;
    TIM4->CCR3=0;
    TIM3->CCR4=0;
 
}



void Move_Right(  )
{
    
    TIM3->CCR1 =Move_Speed;
    TIM4->CCR2 =Move_Speed;
    TIM4->CCR3 =Move_Speed;
    TIM3->CCR4 =Move_Speed;
    
    TIM4->CCR1=0;
    TIM3->CCR2=0;
    TIM3->CCR3=0;
    TIM4->CCR4=0;
 
}

void Move_Stop()
{
    
    TIM3->CCR1 =0;
    TIM4->CCR2 =0;
    TIM4->CCR3 =0;
    TIM3->CCR4 =0;
    
    TIM4->CCR1=0;
    TIM3->CCR2=0;
    TIM3->CCR3=0;
    TIM4->CCR4=0;    
    
}



void OLED_Init()
{
    OLED_GPIO  oled_def;
    oled_def.ID_Adress=0x3c;//OLED的从机地址
    oled_def.SCL=new GPIO(GPIOD,GPIO_Pin_2);
    oled_def.SDA=new GPIO(GPIOD,GPIO_Pin_0);
    OLED oled(&oled_def);								 
    oled.Init_Gpio();
    oled.Inti();
    
    oled.Printf(50,2,"Red  :");
    oled.Printf(50,4,"Blue :");
    oled.Printf(50,6,"Green:");
    
    delete oled_def.SCL;
    delete oled_def.SDA;
}








#ifdef __cplusplus
}
#endif 












