#ifndef	__spi_h_
#define	__spi_h_
#include "stm32f10x.h"                  // Device header

/*
引脚 	SPI编号
			SPI1 	SPI2 	SPI3
NSS 	PA4 	PB12 	PA15		低电平表示片选有效，高电平表示通讯失效，NSS有几个从机就有几个
CLK 	PA5 	PB13	PB3 
MISO 	PA6 	PB14 	PB4 
MOSI 	PA7 	PB15 	PB5
时钟	APB2	APB1	APB1

*/

/*
SPIx		SPI编号
可选用	SPI1	SPI2	SPI3

SPI_Direction
SPI_Direction_2Lines_FullDuplex		双线全双工
SPI_Direction_2Lines_RxOnly				双线只接收
SPI_Direction_1Line_Rx						单线只接收
SPI_Direction_1Line_Tx						单线只发送

SPI_Mode
SPI_Mode_Master		主机模式
SPI_Mode_Slave		从机模式
两者最大区别主机产生SCK信号，而从机接受SCK信号

SPI_DataSize
SPI_DataSize_8b		1个字节
SPI_DataSize_16b	两个字节

SPI_CPOL
SPI_CPOL_High			空闲时SCK高电平
SPI_CPOL_Low			空闲时SCK低电平

SPI_CPHA
SPI_CPHA_1Edge		SCK奇数边沿采集
SPI_CPHA_2Edge		SCK偶数边沿采集

SPI_NSS
SPI_NSS_Hard			NSS硬件产生 
SPI_NSS_Soft 			NSS软件产生（自己配置）

SPI_BaudRatePrescaler
SPI_BaudRatePrescaler_2				对时钟2分频
SPI_BaudRatePrescaler_4				对时钟4分频
SPI_BaudRatePrescaler_8				对时钟8分频
SPI_BaudRatePrescaler_16			对时钟16分频
SPI_BaudRatePrescaler_32			对时钟32分频
SPI_BaudRatePrescaler_64			对时钟64分频
SPI_BaudRatePrescaler_128			对时钟128分频
SPI_BaudRatePrescaler_256			对时钟256分频

SPI_FirstBit
SPI_FirstBit_MSB		高位数据先行
SPI_FirstBit_LSB		低位数据先行

SPI_CRCPolynomial
SPI 的 CRC 校验中的多项式，若使用 CRC 校验时，就使用这个成员的参数(多项式)，来计算 CRC 的值。
没有crc检验的话，此位可以为任意值[0,7]
*/



#define SPI_timeout 0xff       //通信时超时

class SPI
{
	
    public:
    void inti			//初始化SPI,初始化完成后将SCK/MOSI/MISO 引脚初始化成复用推挽模式，CS采用软件控制，故初始化为普通推挽输出模式
	(
	SPI_TypeDef* SPIx,
	uint16_t SPI_Direction,										//设置 SPI 的单双向模式
	uint16_t SPI_Mode,												//设置 SPI 的主/从机端模式 
	uint16_t SPI_DataSize,										//设置 SPI 的数据帧长度，可选 8/16 位
	uint16_t SPI_CPOL,												//设置时钟极性 CPOL，可选高/低电平
	uint16_t SPI_CPHA,												//设置时钟相位，可选奇/偶数边沿采样
	uint16_t SPI_NSS,													//设置 NSS 引脚由 SPI 硬件控制还是软件控制
	uint16_t SPI_BaudRatePrescaler,						//设置时钟分频因子，fpclk/分频数=fSCK
	uint16_t SPI_FirstBit,										//设置 MSB/LSB 先行
	uint16_t SPI_CRCPolynomial								//设置 CRC 校验的表达式
	);

    FunctionalState send_16b(SPI_TypeDef* SPIx,uint16_t data,uint16_t* reply);                  //发送数据（16位），并且读取返回的数据
    FunctionalState	send_8b(SPI_TypeDef* SPIx,uint8_t data,uint8_t* reply);						//发送数据 (8位)，并且读取返回的数据
    FunctionalState	send_8b(SPI_TypeDef* SPIx,uint8_t data);						            //发送数据 (8位)
    FunctionalState send_16b(SPI_TypeDef* SPIx,uint16_t data);                                  //发送数据（16位）


//软件NSS则自行封装一层启动和结束，分别为拉低和拉高，然后send_16b、read_16b在中间
};






#endif
