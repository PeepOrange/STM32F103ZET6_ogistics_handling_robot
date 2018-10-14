#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header



/*****PID参数设置******/
#define K_PWM_Encoder                   100.0           //脉冲效率比    Encoder/PWM = K_PWM_Encoder (5ms时)
#define Encoder_read_num_Proportion		30.0			//比例常数(Kp)
#define Encoder_read_num_Integral		10.0			//积分常数(Ki)	Ki=(Kp*T)/Ti
#define Encoder_read_num_Derivative		0.1			    //微分常数(Kd)	Kd=(Kp*Td)/T
//以下到it文件修改
extern uint16_t LeftUp_PWM;                              //默认的左前轮的PWM,[-1000,1000]
extern uint16_t LeftBack_PWM;                            //默认的左后轮的PWM,[-1000,1000]
extern uint16_t RightUp_PWM;                             //默认的右前轮的PWM,[-1000,1000]
extern uint16_t RightBack_PWM;                           //默认的右后轮的PWM,[-1000,1000]
/*****PID参数调节*****/


#define abs(x)     x>0?x:-x 
#define jud(x)     x>0?1:0

#ifdef __cplusplus
extern "C"
{
#endif 

//用c方式编译的代码
#include "PID.h"
extern PID  LeftUp_PID_Mortor;
extern PID  LeftBack_PID_Mortor;
extern PID  RightUp_PID_Mortor;
extern PID  RightBack_PID_Mortor;
#ifdef __cplusplus
}
#endif 






#ifdef __cplusplus
extern "C"
{
#endif 
    
//用户函数    
void LED1_Toggle(void);     //LED1翻转    
void LED2_Toggle(void);     //LED2翻转
void LED_ALL_OFF(void);     //所有LED关闭    
void PID_PWM_Adujust(int16_t PWM1,int16_t PWM2,int16_t PWM3,int16_t PWM4);       //左前  左后   右前  右后      PID调节PWM

    


//BSP初始化函数
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;        //初始化产生PWM的IO口及其Timer
extern void system_init(void) ;
void OLED_Init(void);       //初始化OLED并且显示基本信息 ，横向 90 ，竖向 2.4.6分别显示任务顺序数字
void  Encoder_Inti(void);       //初始化编码器的IO口和Timer
void TIM6_Inti(void);					//开启定时器6，每次记数1us，记5000次数更新，即5ms，更新时产生中断，中断优先级为1：1



#ifdef __cplusplus
}
#endif 








#endif
