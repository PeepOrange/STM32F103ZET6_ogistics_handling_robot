#include "flash.h"
#include "stm32f10x_flash.h"





void flash_erase(uint8_t FLASH_PAGE)
{
	IRQ_OFF();
	FLASH_Unlock();
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(START_ADDR +(FLASH_PAGE_SIZE * FLASH_PAGE));
	FLASH_Lock();
	IRQ_ON();
}

void flash_write(uint8_t FLASH_PAGE,uint8_t num,uint32_t Data)
{
	IRQ_OFF();
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR |FLASH_FLAG_WRPRTERR);
	FLASH_ProgramWord(START_ADDR +(FLASH_PAGE_SIZE * FLASH_PAGE)+4*num, Data);
	FLASH_Lock();
	IRQ_ON();
}

uint32_t flash_read(uint8_t FLASH_PAGE,uint8_t num)
{
	return (*(__IO uint32_t*)(START_ADDR +(FLASH_PAGE_SIZE * FLASH_PAGE)+4*num)) ;
}

