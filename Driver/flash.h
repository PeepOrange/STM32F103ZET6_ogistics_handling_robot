#ifndef __flash_H_
#define __flash_H_
#include "stdint.h"

 /* STM32 ��������Ʒÿҳ��С 2KByte���С�С������Ʒÿҳ��С 1KByte */
 #if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) ||defined (STM32F10X_CL) || defined (STM32F10X_XL)
 #define FLASH_PAGE_SIZE ((uint16_t)0x800)//2048
 #else
 #define FLASH_PAGE_SIZE ((uint16_t)0x400)//1024
 #endif

#define  START_ADDR ((uint32_t)0x08000800)			//�ڲ�flash���������ڴ治�غϵĿ�ʼ��ַ���˵�ַ����map�ļ��е�Memory Map of the image�ҵ�����ռ�õ��ڴ�
#define  END_ADDR   ((uint32_t)0x08010000)			//�ڲ�flash����ڴ�


#define IRQ_OFF()     __disable_irq()         //�ر����ж�
#define IRQ_ON()      __enable_irq()          //�����ж�


//��ҳ����  (END_ADDR-START_ADDR)/FLASH_PAGE_SIZE    
//�ҵĵ�Ƭ����64ҳ����������飩



void flash_erase(uint8_t FLASH_PAGE);															//��������  	                      FLASH_PAGE��Ϊ[0,63-x]��xΪ����ռ������������
void flash_write(uint8_t FLASH_PAGE,uint8_t num,uint32_t Data);		//��������num�ڴ�д���ִ�С���� 	  FLASH_PAGE��Ϊ[0,63-x]��xΪ����ռ��������������num[0,255]
uint32_t flash_read(uint8_t FLASH_PAGE,uint8_t num);							//��ȡ������num�ڴ���ִ�С����			FLASH_PAGE��Ϊ[0,63-x]��xΪ����ռ��������������num[0,255]			



#endif

