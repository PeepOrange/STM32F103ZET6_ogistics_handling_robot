#ifndef __flash_H_
#define __flash_H_
#include "stdint.h"

 /* STM32 大容量产品每页大小 2KByte，中、小容量产品每页大小 1KByte */
 #if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) ||defined (STM32F10X_CL) || defined (STM32F10X_XL)
 #define FLASH_PAGE_SIZE ((uint16_t)0x800)//2048
 #else
 #define FLASH_PAGE_SIZE ((uint16_t)0x400)//1024
 #endif

#define  START_ADDR ((uint32_t)0x08000800)			//内部flash与代码最大内存不重合的开始地址，此地址可在map文件中的Memory Map of the image找到代码占用的内存
#define  END_ADDR   ((uint32_t)0x08010000)			//内部flash最大内存


#define IRQ_OFF()     __disable_irq()         //关闭总中断
#define IRQ_ON()      __enable_irq()          //打开总中断


//总页数是  (END_ADDR-START_ADDR)/FLASH_PAGE_SIZE    
//我的单片机是64页（包括代码块）



void flash_erase(uint8_t FLASH_PAGE);															//擦除扇区  	                      FLASH_PAGE可为[0,63-x]（x为代码占用扇区数量）
void flash_write(uint8_t FLASH_PAGE,uint8_t num,uint32_t Data);		//向扇区第num内存写入字大小数据 	  FLASH_PAGE可为[0,63-x]（x为代码占用扇区数量），num[0,255]
uint32_t flash_read(uint8_t FLASH_PAGE,uint8_t num);							//读取扇区第num内存的字大小数据			FLASH_PAGE可为[0,63-x]（x为代码占用扇区数量），num[0,255]			



#endif

