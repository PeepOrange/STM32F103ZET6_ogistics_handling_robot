#ifndef __W25Q64__H__
#define __W25Q64__H__

#include "stm32f10x.h"                  // Device header
#include  "spi.h"
#include  "gpio.h"

//W25Q64    SCK	<--->PA5    MISO<--->PA6    MOSI<--->PA7    CS	<--->PC0
//д/��С�� 
// Write/Read((void*)double_buffer, FLASH_PageSize*1, sizeof(double_buffer));


#define  W25Q64_ID                      0XEF4017    //W25Q64 ID

#define FLASH_PageSize              256         //ÿҳռ���ֽ���

//8MByte��Ϊ128Bolck,1Bolck��Ϊ16��������1������16ҳ
//256Byte * 16ҳ * 16 ���� *128�� =8388608 Byte = 8 *2^10 KB =8MB
//2048������ 1������ռ4096Byte 

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
        void SectorErase(uint32_t SectorAddr);  //��������  
        void BulkErase();                       //��Ƭ����      
        void Write(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length);    //��WriteAddrд��Data_Length���ֽڣ��Ȳ�������
        void Read(uint8_t* p_Data,uint32_t ReadAddr,uint16_t Data_Length);      //��ReadAddr��ȡData_Length���ֽڴ洢��*p_Data
        void PowerDown_Mode(FunctionalState ok);                                //����ģʽ����
        void Get_DeviceID( uint8_t* ID);                                         //��ѯ�豸ID
        void Get_ID( uint32_t* ID);                                             //��ѯID�������ڼ��W25Q64�Ƿ�����
    
    private:
        W25Q64_Gpio *gpio;
        void Init_Gpio();
        inline void CS_Set();
        inline void CS_Reset();
        void  Init_SPI3();
        void  Enable_Write();
        void  Wait_WriteEnd();
        void PageWrite(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length);   //��ҳд������, Data_Length������SPI_FLASH_PerWritePageSize
};

extern "C" void Get_ID();





#endif









