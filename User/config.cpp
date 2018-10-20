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



 PID  LeftUp_PID_Mortor={0,0,0,0,Encoder_read_num_Proportion,Encoder_read_num_Integral,Encoder_read_num_Derivative};
 PID  LeftBack_PID_Mortor={0,0,0,0,Encoder_read_num_Proportion,Encoder_read_num_Integral,Encoder_read_num_Derivative};
 PID  RightUp_PID_Mortor={0,0,0,0,Encoder_read_num_Proportion,Encoder_read_num_Integral,Encoder_read_num_Derivative};
 PID  RightBack_PID_Mortor={0,0,0,0,Encoder_read_num_Proportion,Encoder_read_num_Integral,Encoder_read_num_Derivative};


    
    

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
    GPIO Key(GPIOE,GPIO_Pin_4);

    
    Key.mode(GPIO_Mode_IPU,GPIO_Speed_50MHz);
    
   EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4); 
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);    
    
}    
    
void Senor_Init()
{
    GPIO Senor1(GPIOD,GPIO_Pin_0); 
    Senor1.mode(GPIO_Mode_IPU,GPIO_Speed_50MHz);
    
    GPIO Senor2(GPIOD,GPIO_Pin_1); 
    Senor1.mode(GPIO_Mode_IPU,GPIO_Speed_50MHz);

    GPIO Senor3(GPIOD,GPIO_Pin_3); 
    Senor1.mode(GPIO_Mode_IPU,GPIO_Speed_50MHz);

    GPIO Senor4(GPIOD,GPIO_Pin_5); 
    Senor1.mode(GPIO_Mode_IPU,GPIO_Speed_50MHz);    
    
}     









void OutPWM_Motor_Init()
{
   TIM_TimeBaseInitTypeDef	tim_base;
    
	tim_base.TIM_Prescaler=72-1;        //72分频
	tim_base.TIM_Period=1000-1;         //周期1000us
	tim_base.TIM_CounterMode=TIM_CounterMode_Up;    //向上记数
	tim_base.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟不分频
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8, ENABLE);
	TIM_TimeBaseInit(TIM1,&tim_base );
    TIM_TimeBaseInit(TIM8,&tim_base );
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ClearFlag(TIM8, TIM_FLAG_Update);
    
    TIM_OCInitTypeDef	tim_oc_mode;
	tim_oc_mode.TIM_OCMode=TIM_OCMode_PWM1;             //PWM输出模式
	tim_oc_mode.TIM_OutputState=TIM_OutputState_Enable; 
	tim_oc_mode.TIM_Pulse=0;                            //一开始的PWM为0
	tim_oc_mode.TIM_OCPolarity=TIM_OCPolarity_High;     //高电平为有效电平
	tim_oc_mode.TIM_OCIdleState=TIM_OCIdleState_Reset;
    
    TIM_OC1Init(TIM1,&tim_oc_mode);         
    TIM_OC2Init(TIM1,&tim_oc_mode);        
    TIM_OC3Init(TIM1,&tim_oc_mode);        
    TIM_OC4Init(TIM1,&tim_oc_mode);    

    TIM_OC1Init(TIM8,&tim_oc_mode);         
    TIM_OC2Init(TIM8,&tim_oc_mode);        
    TIM_OC3Init(TIM8,&tim_oc_mode);        
    TIM_OC4Init(TIM8,&tim_oc_mode);   
    
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
    
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); 
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable); 
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); 
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); 
 
    TIM_CtrlPWMOutputs(TIM1,ENABLE);
    TIM_CtrlPWMOutputs(TIM8,ENABLE);        //高级定时器PWM输出设置
 
    TIM_Cmd(TIM1,ENABLE);       //开启定时器1  
    TIM_Cmd(TIM8,ENABLE);       //开启定时器8  
    
}



void GPIO_TIM1_Init()
{
    GPIO E11(GPIOE,GPIO_Pin_11);
    E11.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO E9(GPIOE,GPIO_Pin_9);
    E9.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO E13(GPIOE,GPIO_Pin_13);
    E13.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO E14(GPIOE,GPIO_Pin_14);
    E14.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);    
}


void GPIO_TIM8_Init()
{
    GPIO C6(GPIOC,GPIO_Pin_6);
    C6.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO C7(GPIOC,GPIO_Pin_7);
    C7.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    GPIO C8(GPIOC,GPIO_Pin_8);
    C8.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);    
    GPIO C9(GPIOC,GPIO_Pin_9);
    C9.mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);   
}


void PWM_Init()
{
    GPIO_TIM1_Init();
    GPIO_TIM8_Init();
    OutPWM_Motor_Init();
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
    
    oled.Printf(50,3,"Red  : ?");
    oled.Printf(50,5,"Blue : ?");
    oled.Printf(50,7,"Green: ?");
    

    oled.ShowChinese(30,0,0);
    oled.ShowChinese(50,0,1);
    oled.ShowChinese(70,0,2);
    oled.ShowChinese(90,0,3);
    
    
    
    delete oled_def.SCL;
    delete oled_def.SDA;
}




void Oled_PID(float UpLeft,float UpRight,float BackLeft,float BackRight)
{
    OLED_GPIO  oled_def;
    oled_def.ID_Adress=0x3c;//OLED的从机地址
    oled_def.SCL=new GPIO(GPIOD,GPIO_Pin_2);
    oled_def.SDA=new GPIO(GPIOD,GPIO_Pin_0);
    OLED oled(&oled_def);	

      oled.Printf(90,3,"%.2f",UpLeft);
      oled.Printf(90,4,"%.2f",UpRight);      
      oled.Printf(90,5,"%.2f",BackLeft);
      oled.Printf(90,6,"%.2f",BackRight);   
    delete oled_def.SCL;
    delete oled_def.SDA;    
    
}


void  Encoder_Inti()
{
	TIM_ICInitTypeDef       Encoder_ICInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5, ENABLE);
	
    GPIO EncoderA(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15);
    GPIO EncoderB(GPIOB,GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7);
    
	EncoderA.mode(GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
	EncoderB.mode(GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
	
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;
    TIM_TimeBaseStructure.TIM_Period=65535;
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
 
	TIM_EncoderInterfaceConfig(TIM2 , TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,  TIM_ICPolarity_Rising);		//设置为编码器模式3
	TIM_EncoderInterfaceConfig(TIM3 , TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,  TIM_ICPolarity_Rising);		//设置为编码器模式3
	TIM_EncoderInterfaceConfig(TIM4 , TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,  TIM_ICPolarity_Rising);		//设置为编码器模式3
	TIM_EncoderInterfaceConfig(TIM5 , TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,  TIM_ICPolarity_Rising);		//设置为编码器模式3


	TIM_ICStructInit(& Encoder_ICInitStruct);		//配置为通道1，上升沿触发，与上面相配
	
	Encoder_ICInitStruct.TIM_ICFilter=6;       //输入捕获滤波器
	Encoder_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_BothEdge;

	TIM_ICInit(TIM2, &Encoder_ICInitStruct);
	TIM_ICInit(TIM3, &Encoder_ICInitStruct);
	TIM_ICInit(TIM4, &Encoder_ICInitStruct);
	TIM_ICInit(TIM5, &Encoder_ICInitStruct);

	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	
	TIM_SetCounter(TIM2,0);
	TIM_SetCounter(TIM3,0);
	TIM_SetCounter(TIM4,0);
	TIM_SetCounter(TIM5,0);


//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   //若编码器记数超过最大65535，则开启中断在中断中处理超过部分

	TIM_Cmd(TIM2,ENABLE);    
	TIM_Cmd(TIM3,ENABLE);    
	TIM_Cmd(TIM4,ENABLE);    
	TIM_Cmd(TIM5,ENABLE);    
    
    
}



int Encoder_read(char TIMx)
{
	int Encoder_TIM;    
    if(TIMx==2)
    {
   Encoder_TIM= (short)TIM2 -> CNT;  
   TIM2 -> CNT=0;         
    }
    
    else if(TIMx==3)
    {
   Encoder_TIM= (short)TIM3 -> CNT;  
   TIM3-> CNT=0;           
    }
    
    else if(TIMx==4)
    {
   Encoder_TIM= (short)TIM4 -> CNT;  
   TIM4-> CNT=0;           
    }

    else if(TIMx==5)
    {
   Encoder_TIM= (short)TIM5 -> CNT;  
   TIM5-> CNT=0;           
    }    

   return Encoder_TIM;		
}


void UpLeft_PWM_Set(int16_t PWM)
{
    if(PWM>0)
    {
        TIM1->CCR1=PWM;
        TIM8->CCR1=0;
    }
    
    else
    {
        TIM8->CCR1=-PWM;
        TIM1->CCR1=0;        
        
    }   
}



void UpRight_PWM_Set(int16_t PWM)
{
    if(PWM>0)
    {
        TIM1->CCR3=PWM;
        TIM8->CCR3=0;
    }
    
    else
    {
        TIM8->CCR3=-PWM;
        TIM1->CCR3=0;        
        
    }   
}



void BackLeft_PWM_Set(int16_t PWM)
{
    if(PWM>0)
    {
        TIM1->CCR2=PWM;
        TIM8->CCR2=0;
    }
    
    else
    {
        TIM8->CCR2=-PWM;
        TIM1->CCR2=0;        
        
    }   
}


void BackRight_PWM_Set(int16_t PWM)
{
    if(PWM>0)
    {
        TIM1->CCR4=PWM;
        TIM8->CCR4=0;
    }
    
    else
    {
        TIM8->CCR4=-PWM;
        TIM1->CCR4=0;        
        
    }   
}



int16_t abs( int16_t x)
{
    return  (x>0?x:-x) ;
    
}

int16_t jud( int16_t x)
{
    return  (x>0?1:-1) ;
    
}





void PID_PWM_Adujust(int16_t PWM1,int16_t PWM2,int16_t PWM3,int16_t PWM4)       //左前  左后   右前  右后
{
    int16_t Set_PWM;
   
    LeftUp_PID_Mortor.goal_point=abs(PWM1)*K_PWM_Encoder;
    LeftUp_PID_Mortor.read_point=jud(PWM1)*Encoder_read(2);
    Set_PWM=PID_Out(&LeftUp_PID_Mortor)/K_PWM_Encoder;    
    if(Set_PWM>1000)
        Set_PWM=1000;
    else if(Set_PWM<0)
        Set_PWM=0;
    UpLeft_PWM_Set(Set_PWM * jud(PWM1)); 
    
    LeftBack_PID_Mortor.goal_point=abs(PWM2)*K_PWM_Encoder;
    LeftBack_PID_Mortor.read_point=jud(PWM2)*Encoder_read(3);
    Set_PWM=PID_Out(&LeftBack_PID_Mortor)/K_PWM_Encoder;    
    if(Set_PWM>1000)
        Set_PWM=1000;
    else if(Set_PWM<0)
        Set_PWM=0;
    BackLeft_PWM_Set(Set_PWM*jud(PWM2)); 


    RightUp_PID_Mortor.goal_point=abs(PWM3)*K_PWM_Encoder;
    RightUp_PID_Mortor.read_point=jud(PWM3)*Encoder_read(4);
    Set_PWM=PID_Out(&RightUp_PID_Mortor)/K_PWM_Encoder;    
    if(Set_PWM>1000)
        Set_PWM=1000;
    else if(Set_PWM<0)
        Set_PWM=0;
    UpRight_PWM_Set(Set_PWM*jud(PWM3)); 


    RightBack_PID_Mortor.goal_point=abs(PWM4)*K_PWM_Encoder;
    RightBack_PID_Mortor.read_point=jud(PWM4)*Encoder_read(5);
    Set_PWM=PID_Out(&RightBack_PID_Mortor)/K_PWM_Encoder;    
    if(Set_PWM>1000)
        Set_PWM=1000;
    else if(Set_PWM<0)
        Set_PWM=0;
    BackRight_PWM_Set(Set_PWM*jud(PWM4));     
    
}


void TIM6_Inti()					//开启定时器6，每次记数1us，记5000次数更新，即5ms，更新时产生中断，中断优先级为1：1
{
	
	TIM_TimeBaseInitTypeDef	TIM6_DriverStruct;
	NVIC_InitTypeDef	    TIM6_ITStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM6_DriverStruct.TIM_Prescaler=35;						//APB1时钟线是36MHZ,故为35
	TIM6_DriverStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM6_DriverStruct.TIM_Period=Reset_Time;
	TIM6_DriverStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	
	TIM6_ITStruct.NVIC_IRQChannel=TIM6_IRQn;
	TIM6_ITStruct.NVIC_IRQChannelCmd=ENABLE;
	TIM6_ITStruct.NVIC_IRQChannelPreemptionPriority=1;
	TIM6_ITStruct.NVIC_IRQChannelSubPriority=1;
	
	TIM_TimeBaseInit(TIM6, &TIM6_DriverStruct);
	TIM_ITConfig(TIM6,TIM_IT_Update ,ENABLE);
	NVIC_Init(&TIM6_ITStruct);
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_Cmd(TIM6,ENABLE);	
}







#ifdef __cplusplus
}
#endif 












