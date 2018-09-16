#ifndef __W25Q64__H__
#define __W25Q64__H__

#include "stm32f10x.h"                  // Device header
#include  "spi.h"
#include  "gpio.h"

//W25Q64    SCK	<--->PA5    MISO<--->PA6    MOSI<--->PA7    CS	<--->PC0
//写/读小数 
// Write/Read((void*)double_buffer, FLASH_PageSize*1, sizeof(double_buffer));


#define  W25Q64_ID                      0XEF4017    //W25Q64 ID

#define FLASH_PageSize              256         //每页占的字节数

//8MByte分为128Bolck,1Bolck分为16个扇区，1个扇区16页
//256Byte * 16页 * 16 扇区 *128块 =8388608 Byte = 8 *2^10 KB =8MB
//2048个扇区 1个扇区占4096Byte 

struct W25Q64_Gpio
{
    GPIO    *SCK;
    GPIO    *MISO;
    GPIO    *MOSI;
    GPIO    *CS; 
};


class W25Q64
{
    public:
        W25Q64(W25Q64_Gpio *W25Q64_GPIO);
        void Init();
        void SectorErase(uint32_t SectorAddr);  //擦除扇区  
        void BulkErase();                       //整片擦除      
        void Write(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length);    //向WriteAddr写入Data_Length个字节，先擦除扇区
        void Read(uint8_t* p_Data,uint32_t ReadAddr,uint16_t Data_Length);      //向ReadAddr读取Data_Length个字节存储到*p_Data
        void PowerDown_Mode(FunctionalState ok);                                //掉电模式设置
        void Get_DeviceID( uint8_t* ID);                                         //查询设备ID
        void Get_ID( uint32_t* ID);                                             //查询ID，可用于检测W25Q64是否正常
    
    private:
        W25Q64_Gpio *gpio;
        void Init_Gpio();
        inline void CS_Set();
        inline void CS_Reset();
        void  Init_SPI3();
        void  Enable_Write();
        void  Wait_WriteEnd();
        void PageWrite(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length);   //按页写入数据, Data_Length不大于SPI_FLASH_PerWritePageSize
};

extern "C" void Get_ID();





#endif









