#include "W25Q64.h"


//#define  W25X16_ID              0xEF3015   //W25X16
//#define  W25Q16_ID              0xEF4015	 //W25Q16
//#define  W25Q128_ID              0XEF4018   //W25Q128


/*�����-��ͷ*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

/* WIP(busy)��־��FLASH�ڲ�����д�� */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF




W25Q64::W25Q64(W25Q64_Gpio *W25Q64_GPIO)
{
    this->gpio =W25Q64_GPIO;
}

void W25Q64::Init_Gpio()
{
    this->gpio->CS->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    this->gpio->MISO->mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    this->gpio->MOSI->mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    this->gpio->SCK->mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
}


inline void W25Q64::CS_Set()
{
    this->gpio->CS->set();
}    

inline void W25Q64::CS_Reset()
{
    this->CS_Reset();
}

void W25Q64::Init_SPI3()
{
    SPI W25Q64_SPI3;
    W25Q64_SPI3.inti(SPI3,SPI_Direction_2Lines_FullDuplex,SPI_Mode_Master,SPI_DataSize_8b,SPI_CPOL_High,SPI_CPHA_2Edge,SPI_NSS_Soft,SPI_BaudRatePrescaler_4,SPI_FirstBit_MSB,7);
}


void W25Q64::Init()
{
    W25Q64::Init_Gpio();
    W25Q64::CS_Set();
    W25Q64::Init_SPI3();
}

void W25Q64::Enable_Write()
{
    SPI W25Q64_SPI3;
    W25Q64::CS_Reset();
    W25Q64_SPI3.send_8b(SPI3,W25X_WriteEnable);
    W25Q64::CS_Set();
}

void W25Q64::Wait_WriteEnd()
{
   uint8_t FLASH_Status = 0;
   SPI W25Q64_SPI3;
  W25Q64::CS_Reset();

  /* ���� ��״̬�Ĵ��� ���� */
   W25Q64_SPI3.send_8b(SPI3,W25X_ReadStatusReg);

  /* ��FLASHæµ����ȴ� */
  do
  {
    /* ��ȡFLASHоƬ��״̬�Ĵ��� */ 
     W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&FLASH_Status); 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* ����д���־ */

  W25Q64::CS_Set();   
}


void W25Q64::SectorErase(uint32_t SectorAddr)
{
  SPI W25Q64_SPI3;
    /* ����FLASHдʹ������ */
  W25Q64::Enable_Write();
  W25Q64::Wait_WriteEnd();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  W25Q64::CS_Reset();
  /* ������������ָ��*/
  W25Q64_SPI3.send_8b(SPI3,W25X_SectorErase); 
  /*���Ͳ���������ַ�ĸ�λ*/
  W25Q64_SPI3.send_8b(SPI3,(SectorAddr & 0xFF0000) >> 16);     
  /* ���Ͳ���������ַ����λ */
  W25Q64_SPI3.send_8b(SPI3,(SectorAddr & 0xFF00) >> 8); 
  /* ���Ͳ���������ַ�ĵ�λ */
  W25Q64_SPI3.send_8b(SPI3,SectorAddr & 0xFF); 
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  W25Q64::CS_Set();
  /* �ȴ��������*/
  W25Q64::Wait_WriteEnd();    
}


void W25Q64::BulkErase()
{
  SPI W25Q64_SPI3;
    /* ����FLASHдʹ������ */
  W25Q64::Enable_Write();
  W25Q64::CS_Reset();  
  W25Q64_SPI3.send_8b(SPI3,W25X_ChipErase); 
  W25Q64::CS_Set(); 
  W25Q64::Wait_WriteEnd();    
}


void W25Q64::PageWrite(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length)
{
  SPI W25Q64_SPI3;
  /* ����FLASHдʹ������ */
  W25Q64::Enable_Write();
  /* ѡ��FLASH: CS�͵�ƽ */
   W25Q64::CS_Reset();  
  /* дҳдָ��*/
  W25Q64_SPI3.send_8b(SPI3,W25X_PageProgram);
  /*����д��ַ�ĸ�λ*/
  W25Q64_SPI3.send_8b(SPI3,(WriteAddr & 0xFF0000) >> 16);   
  /*����д��ַ����λ*/
  W25Q64_SPI3.send_8b(SPI3,(WriteAddr & 0xFF00) >> 8); 
  /*����д��ַ�ĵ�λ*/
  W25Q64_SPI3.send_8b(SPI3,WriteAddr & 0xFF); 

  if(Data_Length > FLASH_PageSize)
  {
     Data_Length = FLASH_PageSize;
  }
  /* д������*/
  while (Data_Length--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    W25Q64_SPI3.send_8b(SPI3,*p_Data);
    /* ָ����һ�ֽ����� */
    p_Data++;
  }
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  W25Q64::CS_Set(); 
  W25Q64::Wait_WriteEnd();   
}


void W25Q64::Write(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = FLASH_PageSize - Addr;
	/*�����Ҫд��������ҳ*/
  NumOfPage =  Data_Length / FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = Data_Length % FLASH_PageSize;
	
	/* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      W25Q64::PageWrite(p_Data, WriteAddr, Data_Length);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        W25Q64::PageWrite(p_Data, WriteAddr, FLASH_PageSize);
        WriteAddr +=  FLASH_PageSize;
        p_Data += FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      W25Q64::PageWrite(p_Data, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��һҳд����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
        W25Q64::PageWrite(p_Data, WriteAddr, count);
				
        WriteAddr +=  count;
        p_Data += count;
				/*��дʣ�������*/
        W25Q64::PageWrite(p_Data, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {
        W25Q64::PageWrite(p_Data, WriteAddr, Data_Length);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      Data_Length -= count;
      NumOfPage =  Data_Length / FLASH_PageSize;
      NumOfSingle = Data_Length % FLASH_PageSize;
			
			/* ��д��count�����ݣ�Ϊ��������һ��Ҫд�ĵ�ַ���� */
      W25Q64::PageWrite(p_Data, WriteAddr, count);
			
			/* ���������ظ���ַ�������� */
      WriteAddr +=  count;
      p_Data += count;
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        W25Q64::PageWrite(p_Data, WriteAddr, FLASH_PageSize);
        WriteAddr +=  FLASH_PageSize;
        p_Data += FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        W25Q64::PageWrite(p_Data, WriteAddr, NumOfSingle);
      }
    }
  }    
}


void W25Q64::Read(uint8_t* p_Data,uint32_t ReadAddr,uint16_t Data_Length)
{
  SPI W25Q64_SPI3;
    
  /* ѡ��FLASH: CS�͵�ƽ */
  W25Q64::CS_Reset();

  /* ���� �� ָ�� */
  W25Q64_SPI3.send_8b(SPI3,W25X_ReadData);

  /* ���� �� ��ַ��λ */
  W25Q64_SPI3.send_8b(SPI3,(ReadAddr & 0xFF0000) >> 16);

  /* ���� �� ��ַ��λ */
  W25Q64_SPI3.send_8b(SPI3,(ReadAddr& 0xFF00) >> 8);

  /* ���� �� ��ַ��λ */
  W25Q64_SPI3.send_8b(SPI3,ReadAddr & 0xF);
	
	/* ��ȡ���� */
  while (Data_Length--) /* while there is data to be read */
  {
    /* ��ȡһ���ֽ�*/
     W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,p_Data);
    /* ָ����һ���ֽڻ����� */
    p_Data++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  W25Q64::CS_Set();   
}


void W25Q64::PowerDown_Mode(FunctionalState ok)
{
    SPI W25Q64_SPI3;
    W25Q64::CS_Reset();
    W25Q64_SPI3.send_8b(SPI3,(ok? W25X_PowerDown:W25X_ReleasePowerDown));
    W25Q64::CS_Set(); 
}

void W25Q64::Get_DeviceID( uint8_t* ID)
{
  SPI W25Q64_SPI3;
  /* Select the FLASH: Chip Select low */
  W25Q64::CS_Reset();

  /* Send "RDID " instruction */
  W25Q64_SPI3.send_8b(SPI3,W25X_DeviceID);
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte);
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte);
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte);
  /* Read a byte from the FLASH */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,ID);
  /* Deselect the FLASH: Chip Select high */
  W25Q64::CS_Set(); 
}

void W25Q64::Get_ID( uint32_t* ID)
{

  uint8_t Temp0=0,Temp1=0,Temp2=0;
  SPI W25Q64_SPI3;

  /* ��ʼͨѶ��CS�͵�ƽ */
  W25Q64::CS_Reset();

  /* ����JEDECָ���ȡID */
  W25Q64_SPI3.send_8b(SPI3,W25X_JedecDeviceID);

  /* ��ȡһ���ֽ����� */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&Temp0);
  /* ��ȡһ���ֽ����� */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&Temp1);
  /* ��ȡһ���ֽ����� */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&Temp2);

 /* ֹͣͨѶ��CS�ߵ�ƽ */
  W25Q64::CS_Set(); 

  /*�����������������Ϊ�����ķ���ֵ*/
  *ID =(uint32_t) ((uint32_t)Temp0 << 16) | ( (uint16_t)Temp1 << 8) | Temp2;
}    





void Get_ID()
{
    W25Q64_Gpio gpio;
    
    gpio.CS= new GPIO(GPIOC,GPIO_Pin_0);
    gpio.MISO=new GPIO(GPIOA,GPIO_Pin_6);
    gpio.MOSI=new GPIO(GPIOA,GPIO_Pin_7);
    gpio.SCK=new GPIO(GPIOA,GPIO_Pin_5);
    
    W25Q64 w25q64(&gpio);
    
    
}





