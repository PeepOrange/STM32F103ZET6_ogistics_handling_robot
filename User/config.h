#ifndef __config_H
#define __config_H
#include "stm32f10x.h"                  // Device header




#ifdef __cplusplus
extern "C"
{
#endif 

//用c方式编译的代码

#ifdef __cplusplus
}
#endif 



#ifdef __cplusplus
extern "C"
{
#endif 

//用户外部函数    

#define Move_Speed 200        //移动时的PWM(0-1000)    
    
    
void LED1_Toggle(void);     //LED1翻转    
void LED2_Toggle(void);     //LED2翻转
void LED_ALL_OFF(void);     //所有LED关闭    
void Move_Stop(void);            //停止
void Move_Right(void);           //右平移
void Move_Left(void);            //左平移
void Move_Back(void);            //后退
void Move_Up(void);              //前进

















//BSP初始化函数
void LED_Init(void);
void Uart_Init(void);       
void Key_Init(void);       
void PWM_Init(void) ;
extern void system_init(void) ;
void OLED_Init(void);       //初始化OLED并且显示基本信息 
    


#ifdef __cplusplus
}
#endif 








#endif
