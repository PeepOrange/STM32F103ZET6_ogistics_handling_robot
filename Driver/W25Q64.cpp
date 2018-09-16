#include "W25Q64.h"


//#define  W25X16_ID              0xEF3015   //W25X16
//#define  W25Q16_ID              0xEF4015	 //W25Q16
//#define  W25Q128_ID              0XEF4018   //W25Q128


/*命令定义-开头*******************************/
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

/* WIP(busy)标志，FLASH内部正在写入 */
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

  /* 发送 读状态寄存器 命令 */
   W25Q64_SPI3.send_8b(SPI3,W25X_ReadStatusReg);

  /* 若FLASH忙碌，则等待 */
  do
  {
    /* 读取FLASH芯片的状态寄存器 */ 
     W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&FLASH_Status); 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */

  W25Q64::CS_Set();   
}


void W25Q64::SectorErase(uint32_t SectorAddr)
{
  SPI W25Q64_SPI3;
    /* 发送FLASH写使能命令 */
  W25Q64::Enable_Write();
  W25Q64::Wait_WriteEnd();
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  W25Q64::CS_Reset();
  /* 发送扇区擦除指令*/
  W25Q64_SPI3.send_8b(SPI3,W25X_SectorErase); 
  /*发送擦除扇区地址的高位*/
  W25Q64_SPI3.send_8b(SPI3,(SectorAddr & 0xFF0000) >> 16);     
  /* 发送擦除扇区地址的中位 */
  W25Q64_SPI3.send_8b(SPI3,(SectorAddr & 0xFF00) >> 8); 
  /* 发送擦除扇区地址的低位 */
  W25Q64_SPI3.send_8b(SPI3,SectorAddr & 0xFF); 
  /* 停止信号 FLASH: CS 高电平 */
  W25Q64::CS_Set();
  /* 等待擦除完毕*/
  W25Q64::Wait_WriteEnd();    
}


void W25Q64::BulkErase()
{
  SPI W25Q64_SPI3;
    /* 发送FLASH写使能命令 */
  W25Q64::Enable_Write();
  W25Q64::CS_Reset();  
  W25Q64_SPI3.send_8b(SPI3,W25X_ChipErase); 
  W25Q64::CS_Set(); 
  W25Q64::Wait_WriteEnd();    
}


void W25Q64::PageWrite(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length)
{
  SPI W25Q64_SPI3;
  /* 发送FLASH写使能命令 */
  W25Q64::Enable_Write();
  /* 选择FLASH: CS低电平 */
   W25Q64::CS_Reset();  
  /* 写页写指令*/
  W25Q64_SPI3.send_8b(SPI3,W25X_PageProgram);
  /*发送写地址的高位*/
  W25Q64_SPI3.send_8b(SPI3,(WriteAddr & 0xFF0000) >> 16);   
  /*发送写地址的中位*/
  W25Q64_SPI3.send_8b(SPI3,(WriteAddr & 0xFF00) >> 8); 
  /*发送写地址的低位*/
  W25Q64_SPI3.send_8b(SPI3,WriteAddr & 0xFF); 

  if(Data_Length > FLASH_PageSize)
  {
     Data_Length = FLASH_PageSize;
  }
  /* 写入数据*/
  while (Data_Length--)
  {
    /* 发送当前要写入的字节数据 */
    W25Q64_SPI3.send_8b(SPI3,*p_Data);
    /* 指向下一字节数据 */
    p_Data++;
  }
  /* 停止信号 FLASH: CS 高电平 */
  W25Q64::CS_Set(); 
  W25Q64::Wait_WriteEnd();   
}


void W25Q64::Write(uint8_t* p_Data,uint32_t WriteAddr,uint16_t Data_Length)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = FLASH_PageSize - Addr;
	/*计算出要写多少整数页*/
  NumOfPage =  Data_Length / FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = Data_Length % FLASH_PageSize;
	
	/* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0)
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      W25Q64::PageWrite(p_Data, WriteAddr, Data_Length);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    { 
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        W25Q64::PageWrite(p_Data, WriteAddr, FLASH_PageSize);
        WriteAddr +=  FLASH_PageSize;
        p_Data += FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      W25Q64::PageWrite(p_Data, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0)
    {
			/*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				/*先写满当前页*/
        W25Q64::PageWrite(p_Data, WriteAddr, count);
				
        WriteAddr +=  count;
        p_Data += count;
				/*再写剩余的数据*/
        W25Q64::PageWrite(p_Data, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {
        W25Q64::PageWrite(p_Data, WriteAddr, Data_Length);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      Data_Length -= count;
      NumOfPage =  Data_Length / FLASH_PageSize;
      NumOfSingle = Data_Length % FLASH_PageSize;
			
			/* 先写完count个数据，为的是让下一次要写的地址对齐 */
      W25Q64::PageWrite(p_Data, WriteAddr, count);
			
			/* 接下来就重复地址对齐的情况 */
      WriteAddr +=  count;
      p_Data += count;
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        W25Q64::PageWrite(p_Data, WriteAddr, FLASH_PageSize);
        WriteAddr +=  FLASH_PageSize;
        p_Data += FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
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
    
  /* 选择FLASH: CS低电平 */
  W25Q64::CS_Reset();

  /* 发送 读 指令 */
  W25Q64_SPI3.send_8b(SPI3,W25X_ReadData);

  /* 发送 读 地址高位 */
  W25Q64_SPI3.send_8b(SPI3,(ReadAddr & 0xFF0000) >> 16);

  /* 发送 读 地址中位 */
  W25Q64_SPI3.send_8b(SPI3,(ReadAddr& 0xFF00) >> 8);

  /* 发送 读 地址低位 */
  W25Q64_SPI3.send_8b(SPI3,ReadAddr & 0xF);
	
	/* 读取数据 */
  while (Data_Length--) /* while there is data to be read */
  {
    /* 读取一个字节*/
     W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,p_Data);
    /* 指向下一个字节缓冲区 */
    p_Data++;
  }

  /* 停止信号 FLASH: CS 高电平 */
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

  /* 开始通讯：CS低电平 */
  W25Q64::CS_Reset();

  /* 发送JEDEC指令，读取ID */
  W25Q64_SPI3.send_8b(SPI3,W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&Temp0);
  /* 读取一个字节数据 */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&Temp1);
  /* 读取一个字节数据 */
  W25Q64_SPI3.send_8b(SPI3,Dummy_Byte,&Temp2);

 /* 停止通讯：CS高电平 */
  W25Q64::CS_Set(); 

  /*把数据组合起来，作为函数的返回值*/
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





