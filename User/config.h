#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header



/*****PID参数设置******/
#define K_PWM_Encoder                   0.02           //脉冲效率比    Encoder/PWM = K_PWM_Encoder (Reset_Time时)
#define Encoder_read_num_Proportion		20.0			//比例常数(Kp)
#define Encoder_read_num_Integral		10.0			//积分常数(Ki)	Ki=(Kp*T)/Ti
#define Encoder_read_num_Derivative		0.1			    //微分常数(Kd)	Kd=(Kp*Td)/T
#define Goal_LeftUp_PWM                 200             //左前轮目标PWM(-1000,1000)
#define Goal_LeftBack_PWM               200             //左后轮目标PWM(-1000,1000)
#define Goal_RightUp_PWM                200             //右前轮目标PWM(-1000,1000)
#define Goal_RightBack_PWM              200             //右后轮目标PWM(-1000,1000)
#define Reset_Time                      10000           //刷新PID调节时间，单位us [0,65535]
/*****PID参数调节*****/






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
extern int16_t LeftUp_PWM;                              
extern int16_t LeftBack_PWM;                            
extern int16_t RightUp_PWM;                             
extern int16_t RightBack_PWM;                           
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
void Oled_PID(float UpLeft,float UpRight,float BackLeft,float BackRight);
    
    

//BSP初始化函数
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;        //初始化产生PWM的IO口及其Timer
extern void system_init(void) ;
void OLED_Init(void);       //初始化OLED并且显示基本信息 
void  Encoder_Inti(void);       //初始化编码器的IO口和Timer
void TIM6_Inti(void);					//开启定时器6



#ifdef __cplusplus
}
#endif 








#endif
