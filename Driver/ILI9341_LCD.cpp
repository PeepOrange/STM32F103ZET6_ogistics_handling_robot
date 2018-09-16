#include "ILI9341_LCD.h"
#include "ILI9341_LCD_Font.h"



//ILI9341_LCD��������
#define      CMD_SetCoordinateX		 		    0x2A	     //����X����
#define      CMD_SetCoordinateY		 		    0x2B	     //����Y����
#define      CMD_SetPixel						0x2C	     //�������

#define      FSMC_Addr_ILI9341_CMD         ( ( uint32_t ) 0x60000000 )		//FSMC_Bank1_NORSRAM����LCD��������ĵ�ַ
#define      FSMC_Addr_ILI9341_DATA        ( ( uint32_t ) 0x60020000 )		//FSMC_Bank1_NORSRAM����LCD���ݲ����ĵ�ַ      





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
	ILI9341_Write_Data(0xC8);    /*����  ���Ͻǵ� (���)�����½� (�յ�)ɨ�跽ʽ*/
	

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
	//������飬ֻ������0-7
	if (mode >7)
		return;

	//����ģʽ����LCD_SCAN_MODE��ֵ����Ҫ���ڴ�����ѡ��������
	//LCD_SCAN_MODE = mode;

	//����ģʽ����XY��������ؿ��
	if (mode % 2 == 0)
	{
		//0 2 4 6ģʽ��X�������ؿ��Ϊ240��Y����Ϊ320
		this->LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		this->LCD_Y_LENGTH = ILI9341_MORE_PIXEL;
	}
	else
	{
		//1 3 5 7ģʽ��X�������ؿ��Ϊ320��Y����Ϊ240
		this->LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		this->LCD_Y_LENGTH = ILI9341_LESS_PIXEL;
	}

	//0x36��������ĸ�3λ����������GRAMɨ�跽��	
	ILI9341_Write_Cmd(0x36);
	ILI9341_Write_Data(0x08 | (mode << 5));//����ucOption��ֵ����LCD��������0-7��ģʽ
	ILI9341_Write_Cmd(CMD_SetCoordinateX);
	ILI9341_Write_Data(0x00);		/* x ��ʼ�����8λ */
	ILI9341_Write_Data(0x00);		/* x ��ʼ�����8λ */
	ILI9341_Write_Data(((this->LCD_X_LENGTH - 1) >> 8) & 0xFF); /* x ���������8λ */
	ILI9341_Write_Data((this->LCD_X_LENGTH - 1) & 0xFF);				/* x ���������8λ */

	ILI9341_Write_Cmd(CMD_SetCoordinateY);
	ILI9341_Write_Data(0x00);		/* y ��ʼ�����8λ */
	ILI9341_Write_Data(0x00);		/* y ��ʼ�����8λ */
	ILI9341_Write_Data(((this->LCD_Y_LENGTH - 1) >> 8) & 0xFF);	/* y ���������8λ */
	ILI9341_Write_Data((this->LCD_Y_LENGTH - 1) & 0xFF);				/* y ���������8λ */
	
	ILI9341_Write_Cmd(CMD_SetPixel);/* write gram start */
}

void ILI9341_Lcd::Init_Gpio()
{
    //�����������
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
    
    //����FSMC������ģ��8080ʱ��,�첽
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;

	/* ʹ��FSMCʱ��*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 2/72M=28ns
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ��
	//���ݱ���ʱ�䣨DATAST��+ 1��HCLK = 5/72M=70ns	
	readWriteTiming.FSMC_DataSetupTime = 0x04;	 //���ݽ���ʱ��
	 //ģʽB,�첽NOR FLASHģʽ����ILI9341��8080ʱ��ƥ��
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_B;//ѡ����Ƶ�ģʽ

	/*����������ģʽB�޹�*/
	//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
													 //��������ת�����ڣ������ڸ���ģʽ��NOR����
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	//����ʱ�ӷ�Ƶ��������ͬ�����͵Ĵ洢��
	readWriteTiming.FSMC_CLKDivision = 0x00;
	//���ݱ���ʱ�䣬������ͬ���͵�NOR	
	readWriteTiming.FSMC_DataLatency = 0x00;


	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;		//ѡ�����Bank1_NORSRAM1�ڴ�
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	//�����������ַ���߲�����
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;			//NOR���ʹ洢��
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;	//16λ���ݿ��
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;	 //��֧��ͻ������ģʽ��ֻ��ͬ��ͨ�ſ��ã�
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait= FSMC_AsynchronousWait_Disable;  //ȡ��ͬ������ʱ�ĵȴ��ź�
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;	//ͨ��ʱ�ȴ��ź�Ϊ�͵�ƽ
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;					//��֧�ֶ����ͻ������ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; //�ȴ��ź��ڵȴ�ǰ��Ч
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;		//дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;				//������ȴ��źŲ���
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;	//ȡ����չģʽ
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;					//��ʹ��дͻ��ģʽ
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);


	/* ʹ�� FSMC_Bank1_NORSRAM1 */
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
	ILI9341_Write_Cmd(CMD_SetCoordinateX); 				 /* ����X���� */
	ILI9341_Write_Data(X >> 8);	 /* �ȸ�8λ��Ȼ���8λ */
	ILI9341_Write_Data(X & 0xff);	 /* ������ʼ��ͽ�����*/
	ILI9341_Write_Data((X + Width - 1) >> 8);
	ILI9341_Write_Data((X + Width - 1) & 0xff);

	ILI9341_Write_Cmd(CMD_SetCoordinateY); 			     /* ����Y����*/
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
	
	//��ascii���ƫ�ƣ���ģ������ASCII���ǰ32����ͼ�η��ţ�
	ucRelativePositon = cChar - ' ';
	
	//��ģ�׵�ַ
	/*ascii���ƫ��ֵ����ÿ����ģ���ֽ����������ģ��ƫ��λ��*/
	Pfont = (uint8_t *)&ASCII8x16_Table[ucRelativePositon * 16];
	
	//������ʾ����
	ILI9341_Lcd::OpenWindow( X, Y, 8, 16);
	
	ILI9341_Write_Cmd ( CMD_SetPixel );			

	//���ֽڶ�ȡ��ģ����
	//����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
	for ( byteCount = 0; byteCount < 16; byteCount++ )
	{
			//һλһλ����Ҫ��ʾ����ɫ
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



