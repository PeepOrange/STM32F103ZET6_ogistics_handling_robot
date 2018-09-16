#include "OV7725.h"
#include "ILI9341_LCD.h"




 //OV7725ID地址 7位
#define OV7725_ID   0x21   

/* 寄存器宏定义 */
#define REG_GAIN      0x00
#define REG_BLUE      0x01
#define REG_RED       0x02
#define REG_GREEN     0x03
#define REG_BAVG      0x05
#define REG_GAVG      0x06
#define REG_RAVG      0x07
#define REG_AECH      0x08
#define REG_COM2      0x09
#define REG_PID       0x0A
#define REG_VER       0x0B
#define REG_COM3      0x0C
#define REG_COM4      0x0D
#define REG_COM5      0x0E
#define REG_COM6      0x0F
#define REG_AEC       0x10
#define REG_CLKRC     0x11
#define REG_COM7      0x12
#define REG_COM8      0x13
#define REG_COM9      0x14
#define REG_COM10     0x15
#define REG_REG16     0x16
#define REG_HSTART    0x17
#define REG_HSIZE     0x18
#define REG_VSTRT     0x19
#define REG_VSIZE     0x1A
#define REG_PSHFT     0x1B
#define REG_MIDH      0x1C
#define REG_MIDL      0x1D
#define REG_LAEC      0x1F
#define REG_COM11     0x20
#define REG_BDBase    0x22
#define REG_BDMStep   0x23
#define REG_AEW       0x24
#define REG_AEB       0x25
#define REG_VPT       0x26
#define REG_REG28     0x28
#define REG_HOutSize  0x29
#define REG_EXHCH     0x2A
#define REG_EXHCL     0x2B
#define REG_VOutSize  0x2C
#define REG_ADVFL     0x2D
#define REG_ADVFH     0x2E
#define REG_YAVE      0x2F
#define REG_LumHTh    0x30
#define REG_LumLTh    0x31
#define REG_HREF      0x32
#define REG_DM_LNL    0x33
#define REG_DM_LNH    0x34
#define REG_ADoff_B   0x35
#define REG_ADoff_R   0x36
#define REG_ADoff_Gb  0x37
#define REG_ADoff_Gr  0x38
#define REG_Off_B     0x39
#define REG_Off_R     0x3A
#define REG_Off_Gb    0x3B
#define REG_Off_Gr    0x3C
#define REG_COM12     0x3D
#define REG_COM13     0x3E
#define REG_COM14     0x3F
#define REG_COM16     0x41
#define REG_TGT_B     0x42
#define REG_TGT_R     0x43
#define REG_TGT_Gb    0x44
#define REG_TGT_Gr    0x45
#define REG_LC_CTR    0x46
#define REG_LC_XC     0x47
#define REG_LC_YC     0x48
#define REG_LC_COEF   0x49
#define REG_LC_RADI   0x4A
#define REG_LC_COEFB  0x4B 
#define REG_LC_COEFR  0x4C
#define REG_FixGain   0x4D
#define REG_AREF1     0x4F
#define REG_AREF6     0x54
#define REG_UFix      0x60
#define REG_VFix      0x61
#define REG_AWBb_blk  0x62
#define REG_AWB_Ctrl0 0x63
#define REG_DSP_Ctrl1 0x64
#define REG_DSP_Ctrl2 0x65
#define REG_DSP_Ctrl3 0x66
#define REG_DSP_Ctrl4 0x67
#define REG_AWB_bias  0x68
#define REG_AWBCtrl1  0x69
#define REG_AWBCtrl2  0x6A
#define REG_AWBCtrl3  0x6B
#define REG_AWBCtrl4  0x6C
#define REG_AWBCtrl5  0x6D
#define REG_AWBCtrl6  0x6E
#define REG_AWBCtrl7  0x6F
#define REG_AWBCtrl8  0x70
#define REG_AWBCtrl9  0x71
#define REG_AWBCtrl10 0x72
#define REG_AWBCtrl11 0x73
#define REG_AWBCtrl12 0x74
#define REG_AWBCtrl13 0x75
#define REG_AWBCtrl14 0x76
#define REG_AWBCtrl15 0x77
#define REG_AWBCtrl16 0x78
#define REG_AWBCtrl17 0x79
#define REG_AWBCtrl18 0x7A
#define REG_AWBCtrl19 0x7B
#define REG_AWBCtrl20 0x7C
#define REG_AWBCtrl21 0x7D 
#define REG_GAM1      0x7E
#define REG_GAM2      0x7F
#define REG_GAM3      0x80
#define REG_GAM4      0x81
#define REG_GAM5      0x82
#define REG_GAM6      0x83
#define REG_GAM7      0x84
#define REG_GAM8      0x85
#define REG_GAM9      0x86
#define REG_GAM10     0x87
#define REG_GAM11     0x88
#define REG_GAM12     0x89
#define REG_GAM13     0x8A
#define REG_GAM14     0x8B
#define REG_GAM15     0x8C
#define REG_SLOP      0x8D
#define REG_DNSTh     0x8E
#define REG_EDGE0     0x8F
#define REG_EDGE1     0x90
#define REG_DNSOff    0x91
#define REG_EDGE2     0x92
#define REG_EDGE3     0x93
#define REG_MTX1      0x94
#define REG_MTX2      0x95
#define REG_MTX3      0x96
#define REG_MTX4      0x97
#define REG_MTX5      0x98
#define REG_MTX6      0x99
#define REG_MTX_Ctrl  0x9A
#define REG_BRIGHT    0x9B
#define REG_CNST      0x9C
#define REG_UVADJ0    0x9E
#define REG_UVADJ1    0x9F
#define REG_SCAL0     0xA0
#define REG_SCAL1     0xA1
#define REG_SCAL2     0xA2
#define REG_SDE       0xA6
#define REG_USAT      0xA7
#define REG_VSAT      0xA8
#define REG_HUECOS    0xA9
#define REG_HUESIN    0xAA
#define REG_SIGN      0xAB
#define REG_DSPAuto   0xAC
#define SCCB_WriteByte(x,y)   OV7725::write(y,OV7725_ID,x)
#define SCCB_ReadByte(x,y,z)  OV7725::read(x,y,OV7725_ID,z) 




typedef struct Reg
{
	uint8_t Address;			   /*寄存器地址*/
	uint8_t Value;		           /*寄存器值*/
}Reg_Info;

/* 寄存器参数配置 */
Reg_Info Sensor_Config[] =
{
	{REG_CLKRC,     0x00}, /*clock config*/
	{REG_COM7,      0x46}, /*QVGA RGB565 */
	{REG_HSTART,    0x3f},
	{REG_HSIZE,     0x50},
	{REG_VSTRT,     0x03},
	{REG_VSIZE,     0x78},
	{REG_HREF,      0x00},
	{REG_HOutSize,  0x50},
	{REG_VOutSize,  0x78},
	{REG_EXHCH,     0x00},
	

	/*DSP control*/
	{REG_TGT_B,     0x7f},
	{REG_FixGain,   0x09},
	{REG_AWB_Ctrl0, 0xe0},
	{REG_DSP_Ctrl1, 0xff},
	{REG_DSP_Ctrl2, 0x20},
	{REG_DSP_Ctrl3,	0x00},
	{REG_DSP_Ctrl4, 0x00},

	/*AGC AEC AWB*/
	{REG_COM8,		0xf0},
	{REG_COM4,		0x81}, /*Pll AEC CONFIG*/
	{REG_COM6,		0xc5},
	{REG_COM9,		0x21},
	{REG_BDBase,	0xFF},
	{REG_BDMStep,	0x01},
	{REG_AEW,		0x34},
	{REG_AEB,		0x3c},
	{REG_VPT,		0xa1},
	{REG_EXHCL,		0x00},
	{REG_AWBCtrl3,  0xaa},
	{REG_COM8,		0xff},
	{REG_AWBCtrl1,  0x5d},

	{REG_EDGE1,		0x0a},
	{REG_DNSOff,	0x01},
	{REG_EDGE2,		0x01},
	{REG_EDGE3,		0x01},

	{REG_MTX1,		0x5f},
	{REG_MTX2,		0x53},
	{REG_MTX3,		0x11},
	{REG_MTX4,		0x1a},
	{REG_MTX5,		0x3d},
	{REG_MTX6,		0x5a},
	{REG_MTX_Ctrl,  0x1e},

	{REG_BRIGHT,	0x00},
	{REG_CNST,		0x25},
	{REG_USAT,		0x65},
	{REG_VSAT,		0x65},
	{REG_UVADJ0,	0x81},
  //{REG_SDE,		  0x20},	//黑白
	{REG_SDE,		  0x06},	//彩色	调节SDE这个寄存器还可以实现其他效果
	
    /*GAMMA config*/
	{REG_GAM1,		0x0c},
	{REG_GAM2,		0x16},
	{REG_GAM3,		0x2a},
	{REG_GAM4,		0x4e},
	{REG_GAM5,		0x61},
	{REG_GAM6,		0x6f},
	{REG_GAM7,		0x7b},
	{REG_GAM8,		0x86},
	{REG_GAM9,		0x8e},
	{REG_GAM10,		0x97},
	{REG_GAM11,		0xa4},
	{REG_GAM12,		0xaf},
	{REG_GAM13,		0xc5},
	{REG_GAM14,		0xd7},
	{REG_GAM15,		0xe8},
	{REG_SLOP,		0x20},

	{REG_HUECOS,	0x80},
	{REG_HUESIN,	0x80},
	{REG_DSPAuto,	0xff},
	{REG_DM_LNL,	0x00},
	{REG_BDBase,	0x99},
	{REG_BDMStep,	0x03},
	{REG_LC_RADI,	0x00},
	{REG_LC_COEF,	0x13},
	{REG_LC_XC,		0x08},
	{REG_LC_COEFB,  0x14},
	{REG_LC_COEFR,  0x17},
	{REG_LC_CTR,	0x05},
	
	{REG_COM3,		0xd0},/*Horizontal mirror image*/

	/*night mode auto frame rate control*/
	{REG_COM5,		0xf5},	 /*在夜视环境下，自动降低帧率，保证低照度画面质量*/
  //{REG_COM5,		0x31},	/*夜视环境帧率不变*/
};

uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);	  /*结构体数组成员数目*/




OV7725::OV7725(OV7725_GPIO *gpio):IIC_CS(gpio->SCL,gpio->SDA) 
{
	this->P_OV7725_Gpio=gpio;
}



void OV7725::Init_Gpio()
{
    //IIC模拟SCCB通信管脚配置
    P_OV7725_Gpio->SCL->mode(GPIO_Mode_Out_OD,GPIO_Speed_50MHz);
    P_OV7725_Gpio->SDA->mode(GPIO_Mode_Out_OD,GPIO_Speed_50MHz);
    
    //FIFO 控制GPIO配置
    P_OV7725_Gpio->FIFO_OE->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    P_OV7725_Gpio->FIFO_WRST->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    P_OV7725_Gpio->FIFO_RRST->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    P_OV7725_Gpio->FIFO_RCLK->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    P_OV7725_Gpio->FIFO_WE->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    P_OV7725_Gpio->FIFO_VSYNC->mode(GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
    
    //FIFO 数据GPIO配置
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(P_OV7725_Gpio->FIFO_DATA_Port, &GPIO_InitStructure);    

    
    P_OV7725_Gpio->FIFO_OE->reset();  /*拉低使FIFO输出使能*/
    P_OV7725_Gpio->FIFO_WE->set();    /*拉高使FIFO写允许*/    
        
}



FunctionalState OV7725::Init()
{
    uint8_t Read_IDCode = 0;	
    uint16_t i = 0;
    
    OV7725::Init_Gpio();
    
    if(OV7725::write(0x80,OV7725_ID,0x12)!=ENABLE)       //复位
        return DISABLE;
    if(OV7725::read(&Read_IDCode,1,OV7725_ID,0x0b)!=ENABLE) //获取ID号
        return DISABLE;
    
    if(Read_IDCode==OV7725_ID || Read_IDCode ==(OV7725_ID<<1) )
    {
		for( i = 0 ; i < OV7725_REG_NUM ; i++ )
		{
			if(OV7725::write(Sensor_Config[i].Value,OV7725_ID,Sensor_Config[i].Address)!=ENABLE)
            return    DISABLE;             
		}                       
    }    
    else
    return DISABLE;   
    return ENABLE;      
}



void  OV7725::Set_StyleMode(uint8_t StyleMode)
{
    	switch(StyleMode)
	{
		case 0://正常
			SCCB_WriteByte(0xa6, 0x06);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0x80);
		break;
		
		case 1://黑白
			SCCB_WriteByte(0xa6, 0x26);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0x80);
		break;	
		
		case 2://偏蓝
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0xa0);
			SCCB_WriteByte(0x61, 0x40);	
		break;	
		
		case 3://复古
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x40);
			SCCB_WriteByte(0x61, 0xa0);	
		break;	
		
		case 4://偏红
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0xc0);		
		break;	
		
		case 5://偏绿
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x60);
			SCCB_WriteByte(0x61, 0x60);		
		break;	
		
		case 6://反相
			SCCB_WriteByte(0xa6, 0x46);
		break;					
	}   
}


void OV7725::Set_LightMode(uint8_t Lightmode)
{
	switch(Lightmode)
	{
		case 0:	//Auto，自动模式
			SCCB_WriteByte(0x13, 0xff); //AWB on 
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;
		case 1://sunny，晴天
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x5a);
			SCCB_WriteByte(0x02, 0x5c);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 2://cloudy，多云
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x58);
			SCCB_WriteByte(0x02, 0x60);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 3://office，办公室
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x84);
			SCCB_WriteByte(0x02, 0x4c);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 4://home，家里
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x96);
			SCCB_WriteByte(0x02, 0x40);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		
		case 5://night，夜晚
			SCCB_WriteByte(0x13, 0xff); //AWB on
			SCCB_WriteByte(0x0e, 0xe5);
			break;
	}  
}


void OV7725::Set_ColorSaturation(int8_t Sat)
{    
 		SCCB_WriteByte(REG_USAT, (Sat+4)<<4); 
		SCCB_WriteByte(REG_VSAT, (Sat+4)<<4);          
}



void OV7725::Set_Brightness(int8_t Bri)
{
	uint8_t BRIGHT_Value,SIGN_Value;	
	
	switch(Bri)
	{
		case 4:
				BRIGHT_Value = 0x48;
				SIGN_Value = 0x06;
        break;
		
		case 3:
				BRIGHT_Value = 0x38;
				SIGN_Value = 0x06;		
		break;	
		
		case 2:
				BRIGHT_Value = 0x28;
				SIGN_Value = 0x06;			
		break;	
		
		case 1:
				BRIGHT_Value = 0x18;
				SIGN_Value = 0x06;			
		break;	
		
		case 0:
				BRIGHT_Value = 0x08;
				SIGN_Value = 0x06;			
		break;	
		
		case -1:
				BRIGHT_Value = 0x08;
				SIGN_Value = 0x0e;		
		break;	
		
		case -2:
				BRIGHT_Value = 0x18;
				SIGN_Value = 0x0e;		
		break;	
		
		case -3:
				BRIGHT_Value = 0x28;
				SIGN_Value = 0x0e;		
		break;	
		
		case -4:
				BRIGHT_Value = 0x38;
				SIGN_Value = 0x0e;		
		break;	
		
	}

		SCCB_WriteByte(REG_BRIGHT, BRIGHT_Value); //AWB on
		SCCB_WriteByte(REG_SIGN, SIGN_Value);        
}



void OV7725::Set_Contrast(int8_t Con)
{   
    SCCB_WriteByte(REG_CNST, (0x30-(4-Con)*4));   
}



void OV7725::Set_Window(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t QVGA_Or_VGA)
{
	uint8_t reg_raw,cal_temp;

	/***********QVGA or VGA *************/
	if(QVGA_Or_VGA == 0)
	{
		/*QVGA RGB565 */
		SCCB_WriteByte(REG_COM7,0x46); 
	}
	else
	{
			/*VGA RGB565 */
		SCCB_WriteByte(REG_COM7,0x06); 
	}

	/***************HSTART*********************/
	//读取寄存器的原内容，HStart包含偏移值，在原始偏移植的基础上加上窗口偏移	
	SCCB_ReadByte(&reg_raw,1,REG_HSTART);
	
	//x为窗口偏移，高8位存储在HSTART，低2位在HREF
	cal_temp = (reg_raw + (x>>2));	
	SCCB_WriteByte(REG_HSTART,cal_temp ); 
	
	/***************HSIZE*********************/
	//水平宽度，高8位存储在HSIZE，低2位存储在HREF
	SCCB_WriteByte(REG_HSIZE,width>>2);//HSIZE左移两位 
	
	
	/***************VSTART*********************/
	//读取寄存器的原内容，VStart包含偏移值，在原始偏移植的基础上加上窗口偏移	
	SCCB_ReadByte(&reg_raw,1,REG_VSTRT);	
	//y为窗口偏移，高8位存储在HSTART，低1位在HREF
	cal_temp = (reg_raw + (y>>1));	
	
	SCCB_WriteByte(REG_VSTRT,cal_temp);
	
	/***************VSIZE*********************/
	//垂直高度，高8位存储在VSIZE，低1位存储在HREF
	SCCB_WriteByte(REG_VSIZE,height>>1);//VSIZE左移一位
	
	/***************VSTART*********************/
	//读取寄存器的原内容	
	SCCB_ReadByte(&reg_raw,1,REG_HREF);	
	//把水平宽度的低2位、垂直高度的低1位，水平偏移的低2位，垂直偏移的低1位的配置添加到HREF
	cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2)|((x&0x03)<<4)|((y&0x01)<<6));	
	
	SCCB_WriteByte(REG_HREF,cal_temp);
	
	/***************HOUTSIZIE /VOUTSIZE*********************/
	SCCB_WriteByte(REG_HOutSize,width>>2);
	SCCB_WriteByte(REG_VOutSize,height>>1);
	
	//读取寄存器的原内容	
	SCCB_ReadByte(&reg_raw,1,REG_EXHCH);	
	cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2));	

	SCCB_WriteByte(REG_EXHCH,cal_temp);	    
    
    
}



void OV7725::Prepare()
{
    this->P_OV7725_Gpio->FIFO_RRST->reset();
    this->P_OV7725_Gpio->FIFO_RCLK->reset();
    this->P_OV7725_Gpio->FIFO_RCLK->set();
    this->P_OV7725_Gpio->FIFO_RRST->set();
    this->P_OV7725_Gpio->FIFO_RCLK->reset();
    this->P_OV7725_Gpio->FIFO_RCLK->set(); 
}


 uint16_t    OV7725::Read_FIFO_Pixel()
{
    uint16_t RGB565=0;
    this->P_OV7725_Gpio->FIFO_RCLK->reset();
    RGB565 = (this->P_OV7725_Gpio->FIFO_DATA_Port->IDR<<8) & 0xff00;
    this->P_OV7725_Gpio->FIFO_RCLK->set();
    this->P_OV7725_Gpio->FIFO_RCLK->reset();
    RGB565 |= (this->P_OV7725_Gpio->FIFO_DATA_Port->IDR) & 0x00ff;
    this->P_OV7725_Gpio->FIFO_RCLK->set();
    
    return    RGB565; 
}


void  OV7725::Display_ILI9341_LCD(uint16_t x,uint16_t y,uint16_t width,uint16_t height)                        //显示图像在屏幕上
{
	uint16_t i, j; 
    
    ILI9341_Lcd::FillPixel_Mode(x,y,width,height);      //进入填充模式
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			ILI9341_Lcd::WritePixel(OV7725::Read_FIFO_Pixel());	/* 从FIFO读出一个rgb565类型像素，写到ILI9341_LCD像素点上 */			
		}
	}          
}





















