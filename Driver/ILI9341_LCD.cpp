#include "ILI9341_LCD.h"
#include "ILI9341_LCD_Font.h"



//ILI9341_LCD常用命令
#define      CMD_SetCoordinateX		 		    0x2A	     //设置X坐标
#define      CMD_SetCoordinateY		 		    0x2B	     //设置Y坐标
#define      CMD_SetPixel						0x2C	     //填充像素

#define      FSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x60000000 )		//FSMC_Bank1_NORSRAM用于LCD命令操作的地址
#define      FSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x60020000 )		//FSMC_Bank1_NORSRAM用于LCD数据操作的地址      





#ifdef __cplusplus
 extern "C"
{
#include "system.h" 
}
#endif


ILI9341_Lcd::ILI9341_Lcd(GPIO_ILI9341_Lcd * GPIO_Config)
{


	this->ILI9341_Lcd_GPIO = GPIO_Config;
	this->LCD_X_LENGTH = ILI9341_LESS_PIXEL;
	this->LCD_Y_LENGTH = ILI9341_MORE_PIXEL;
}


void ILI9341_Lcd::BackLed(FunctionalState enumState)
{
	if (enumState)
		this->ILI9341_Lcd_GPIO->ILI9341_BK->reset();
	else
		this->ILI9341_Lcd_GPIO->ILI9341_BK->set();
}

void ILI9341_Lcd::Reset()
{
	this->ILI9341_Lcd_GPIO->ILI9341_RST->reset();
	delay_us(40);
	this->ILI9341_Lcd_GPIO->ILI9341_RST->set();
	delay_us(40);

}

inline void ILI9341_Lcd::ILI9341_Write_Cmd(uint16_t Cmd)
{
	*(__IO uint16_t *) (FSMC_Addr_ILI9341_CMD) = Cmd;
}

inline void ILI9341_Lcd::ILI9341_Write_Data(uint16_t Data)
{
	*(__IO uint16_t *) (FSMC_Addr_ILI9341_DATA) = Data;
}

inline uint16_t ILI9341_Lcd::ILI9341_Read()
{
	return (*(__IO uint16_t *) (FSMC_Addr_ILI9341_DATA));
}

void ILI9341_Lcd::REG_Config()
{
	/*  Power control B (CFh)  */
	
	ILI9341_Write_Cmd(0xCF);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x81);
	ILI9341_Write_Data(0x30);

	/*  Power on sequence control (EDh) */
	
	ILI9341_Write_Cmd(0xED);
	ILI9341_Write_Data(0x64);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x12);
	ILI9341_Write_Data(0x81);

	/*  Driver timing control A (E8h) */
	
	ILI9341_Write_Cmd(0xE8);
	ILI9341_Write_Data(0x85);
	ILI9341_Write_Data(0x10);
	ILI9341_Write_Data(0x78);

	/*  Power control A (CBh) */
	
	ILI9341_Write_Cmd(0xCB);
	ILI9341_Write_Data(0x39);
	ILI9341_Write_Data(0x2C);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x34);
	ILI9341_Write_Data(0x02);

	/* Pump ratio control (F7h) */
	
	ILI9341_Write_Cmd(0xF7);
	ILI9341_Write_Data(0x20);

	/* Driver timing control B */
	
	ILI9341_Write_Cmd(0xEA);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x00);

	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	
	ILI9341_Write_Cmd(0xB1);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x1B);

	/*  Display Function Control (B6h) */
	
	ILI9341_Write_Cmd(0xB6);
	ILI9341_Write_Data(0x0A);
	ILI9341_Write_Data(0xA2);

	/* Power Control 1 (C0h) */
	
	ILI9341_Write_Cmd(0xC0);
	ILI9341_Write_Data(0x35);

	/* Power Control 2 (C1h) */
	
	ILI9341_Write_Cmd(0xC1);
	ILI9341_Write_Data(0x11);

	/* VCOM Control 1 (C5h) */
	ILI9341_Write_Cmd(0xC5);
	ILI9341_Write_Data(0x45);
	ILI9341_Write_Data(0x45);

	/*  VCOM Control 2 (C7h)  */
	ILI9341_Write_Cmd(0xC7);
	ILI9341_Write_Data(0xA2);

	/* Enable 3G (F2h) */
	ILI9341_Write_Cmd(0xF2);
	ILI9341_Write_Data(0x00);

	/* Gamma Set (26h) */
	ILI9341_Write_Cmd(0x26);
	ILI9341_Write_Data(0x01);
	

	/* Positive Gamma Correction */
	ILI9341_Write_Cmd(0xE0); //Set Gamma
	ILI9341_Write_Data(0x0F);
	ILI9341_Write_Data(0x26);
	ILI9341_Write_Data(0x24);
	ILI9341_Write_Data(0x0B);
	ILI9341_Write_Data(0x0E);
	ILI9341_Write_Data(0x09);
	ILI9341_Write_Data(0x54);
	ILI9341_Write_Data(0xA8);
	ILI9341_Write_Data(0x46);
	ILI9341_Write_Data(0x0C);
	ILI9341_Write_Data(0x17);
	ILI9341_Write_Data(0x09);
	ILI9341_Write_Data(0x0F);
	ILI9341_Write_Data(0x07);
	ILI9341_Write_Data(0x00);

	/* Negative Gamma Correction (E1h) */
	ILI9341_Write_Cmd(0XE1); //Set Gamma
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x19);
	ILI9341_Write_Data(0x1B);
	ILI9341_Write_Data(0x04);
	ILI9341_Write_Data(0x10);
	ILI9341_Write_Data(0x07);
	ILI9341_Write_Data(0x2A);
	ILI9341_Write_Data(0x47);
	ILI9341_Write_Data(0x39);
	ILI9341_Write_Data(0x03);
	ILI9341_Write_Data(0x06);
	ILI9341_Write_Data(0x06);
	ILI9341_Write_Data(0x30);
	ILI9341_Write_Data(0x38);
	ILI9341_Write_Data(0x0F);

	/* memory access control set */
	
	ILI9341_Write_Cmd(0x36);
	ILI9341_Write_Data(0xC8);    /*竖屏  左上角到 (起点)到右下角 (终点)扫描方式*/
	

	/* column address control set */
	ILI9341_Write_Cmd(CMD_SetCoordinateX);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0xEF);

	/* page address control set */
	
	ILI9341_Write_Cmd(CMD_SetCoordinateY);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x01);
	ILI9341_Write_Data(0x3F);

	/*  Pixel Format Set (3Ah)  */
	
	ILI9341_Write_Cmd(0x3a);
	ILI9341_Write_Data(0x55);

	/* Sleep Out (11h)  */
	ILI9341_Write_Cmd(0x11);
	delay_us(150);
	

	/* Display ON (29h) */
	ILI9341_Write_Cmd(0x29);

}



void ILI9341_Lcd::GramScan_Mode(uint8_t mode)
{
	//参数检查，只可输入0-7
	if (mode >7)
		return;

	//根据模式更新LCD_SCAN_MODE的值，主要用于触摸屏选择计算参数
	//LCD_SCAN_MODE = mode;

	//根据模式更新XY方向的像素宽度
	if (mode % 2 == 0)
	{
		//0 2 4 6模式下X方向像素宽度为240，Y方向为320
		this->LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		this->LCD_Y_LENGTH = ILI9341_MORE_PIXEL;
	}
	else
	{
		//1 3 5 7模式下X方向像素宽度为320，Y方向为240
		this->LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		this->LCD_Y_LENGTH = ILI9341_LESS_PIXEL;
	}

	//0x36命令参数的高3位可用于设置GRAM扫描方向	
	ILI9341_Write_Cmd(0x36);
	ILI9341_Write_Data(0x08 | (mode << 5));//根据ucOption的值设置LCD参数，共0-7种模式
	ILI9341_Write_Cmd(CMD_SetCoordinateX);
	ILI9341_Write_Data(0x00);		/* x 起始坐标高8位 */
	ILI9341_Write_Data(0x00);		/* x 起始坐标低8位 */
	ILI9341_Write_Data(((this->LCD_X_LENGTH - 1) >> 8) & 0xFF); /* x 结束坐标高8位 */
	ILI9341_Write_Data((this->LCD_X_LENGTH - 1) & 0xFF);				/* x 结束坐标低8位 */

	ILI9341_Write_Cmd(CMD_SetCoordinateY);
	ILI9341_Write_Data(0x00);		/* y 起始坐标高8位 */
	ILI9341_Write_Data(0x00);		/* y 起始坐标低8位 */
	ILI9341_Write_Data(((this->LCD_Y_LENGTH - 1) >> 8) & 0xFF);	/* y 结束坐标高8位 */
	ILI9341_Write_Data((this->LCD_Y_LENGTH - 1) & 0xFF);				/* y 结束坐标低8位 */
	
	ILI9341_Write_Cmd(CMD_SetPixel);/* write gram start */
}

void ILI9341_Lcd::Init_Gpio()
{
    //配置相关引脚
	ILI9341_Lcd_GPIO->ILI9341_D0->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D1->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D2->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D3->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D4->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D5->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D6->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D7->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D8->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D9->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D10->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D11->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D12->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D13->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D14->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_D15->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

	ILI9341_Lcd_GPIO->ILI9341_BK->mode(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_RST->mode(GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

	ILI9341_Lcd_GPIO->ILI9341_CS->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_DC->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_RD->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	ILI9341_Lcd_GPIO->ILI9341_WR->mode(GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

}




void ILI9341_Lcd::Init_FSMC()
{
    
    //配置FSMC，用于模拟8080时序,异步
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;

	/* 使能FSMC时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	//地址建立时间（ADDSET）为1个HCLK 2/72M=28ns
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间
	//数据保持时间（DATAST）+ 1个HCLK = 5/72M=70ns	
	readWriteTiming.FSMC_DataSetupTime = 0x04;	 //数据建立时间
	 //模式B,异步NOR FLASH模式，与ILI9341的8080时序匹配
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_B;//选择控制的模式

	/*以下配置与模式B无关*/
	//地址保持时间（ADDHLD）模式A未用到
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
													 //设置总线转换周期，仅用于复用模式的NOR操作
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	//设置时钟分频，仅用于同步类型的存储器
	readWriteTiming.FSMC_CLKDivision = 0x00;
	//数据保持时间，仅用于同步型的NOR	
	readWriteTiming.FSMC_DataLatency = 0x00;


	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;		//选择控制Bank1_NORSRAM1内存
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	//数据总线与地址总线不复用
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;			//NOR类型存储器
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//16位数据宽度
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	 //不支持突发访问模式（只有同步通信可用）
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait= FSMC_AsynchronousWait_Disable;  //取消同步传输时的等待信号
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;	//通信时等待信号为低电平
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;					//不支持对齐的突发访问模式
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; //等待信号在等待前有效
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;		//写使能
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;				//不允许等待信号插入
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;	//取消拓展模式
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;					//不使能写突发模式
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);


	/* 使能 FSMC_Bank1_NORSRAM1 */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);           
    
}



void ILI9341_Lcd::Init()
{
    ILI9341_Lcd::Init_Gpio();
    ILI9341_Lcd::Init_FSMC();
    ILI9341_Lcd::BackLed(ENABLE);
    ILI9341_Lcd::Reset();
    ILI9341_Lcd::REG_Config();
}


 void ILI9341_Lcd::OpenWindow(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{
	ILI9341_Write_Cmd(CMD_SetCoordinateX); 				 /* 设置X坐标 */
	ILI9341_Write_Data(X >> 8);	 /* 先高8位，然后低8位 */
	ILI9341_Write_Data(X & 0xff);	 /* 设置起始点和结束点*/
	ILI9341_Write_Data((X + Width - 1) >> 8);
	ILI9341_Write_Data((X + Width - 1) & 0xff);

	ILI9341_Write_Cmd(CMD_SetCoordinateY); 			     /* 设置Y坐标*/
	ILI9341_Write_Data(Y >> 8);
	ILI9341_Write_Data(Y & 0xff);
	ILI9341_Write_Data((Y + Height - 1) >> 8);
	ILI9341_Write_Data((Y + Height - 1) & 0xff);
}



void ILI9341_Lcd::DispChar ( uint16_t X, uint16_t Y, const char cChar ,uint16_t  TextColor,uint16_t BackColor)
{
	uint8_t  byteCount, bitCount;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
	
	//对ascii码表偏移（字模表不包含ASCII表的前32个非图形符号）
	ucRelativePositon = cChar - ' ';
	
	//字模首地址
	/*ascii码表偏移值乘以每个字模的字节数，求出字模的偏移位置*/
	Pfont = (uint8_t *)&ASCII8x16_Table[ucRelativePositon * 16];
	
	//设置显示窗口
	ILI9341_Lcd::OpenWindow( X, Y, 8, 16);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );			

	//按字节读取字模数据
	//由于前面直接设置了显示窗口，显示数据会自动换行
	for ( byteCount = 0; byteCount < 16; byteCount++ )
	{
			//一位一位处理要显示的颜色
			for ( bitCount = 0; bitCount < 8; bitCount++ )
			{
					if ( Pfont[byteCount] & (0x80>>bitCount) )
						ILI9341_Write_Data ( TextColor );			
					else
						ILI9341_Write_Data ( BackColor );
			}	
	}	
}



void ILI9341_Lcd::DispStringLine (  uint16_t line,  char * pStr,uint16_t  TextColor,uint16_t BackColor )
{
	uint16_t X = 0;
	
	while ( * pStr != '\0' )
	{
		if ( ( X  + 8 ) > this->LCD_X_LENGTH )
		{
			X = 0;
			line += 16;
		}
		
		if ( ( line  + 1 ) > this->LCD_Y_LENGTH )
		{
			X = 0;
			line = 0;
		}
		
		ILI9341_Lcd::DispChar ( X, line, * pStr,TextColor,BackColor);
		
		pStr ++;
		
		X += 8;
	}
	
}

void ILI9341_Lcd::Fill(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height,uint16_t Color)
{
    
    
    uint32_t i = 0;
    ILI9341_Lcd::OpenWindow( X, Y, Width, Height);

	ILI9341_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < Width*Height; i ++ )
		ILI9341_Write_Data ( Color );
    
}



void ILI9341_Lcd::FillPixel_Mode(uint16_t X, uint16_t Y, uint16_t Width, uint16_t Height)
{    
    ILI9341_Lcd::OpenWindow( X, Y, Width, Height);
    ILI9341_Write_Cmd ( CMD_SetPixel );	    
}


void ILI9341_Lcd::WritePixel(uint16_t RGB565)
{   
    ILI9341_Write_Data ( RGB565 );    
}



