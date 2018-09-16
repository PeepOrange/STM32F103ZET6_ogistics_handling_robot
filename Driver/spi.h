#ifndef	__spi_h_
#define	__spi_h_
#include "stm32f10x.h"                  // Device header

/*
���� 	SPI���
			SPI1 	SPI2 	SPI3
NSS 	PA4 	PB12 	PA15		�͵�ƽ��ʾƬѡ��Ч���ߵ�ƽ��ʾͨѶʧЧ��NSS�м����ӻ����м���
CLK 	PA5 	PB13	PB3 
MISO 	PA6 	PB14 	PB4 
MOSI 	PA7 	PB15 	PB5
ʱ��	APB2	APB1	APB1

*/

/*
SPIx		SPI���
��ѡ��	SPI1	SPI2	SPI3

SPI_Direction
SPI_Direction_2Lines_FullDuplex		˫��ȫ˫��
SPI_Direction_2Lines_RxOnly				˫��ֻ����
SPI_Direction_1Line_Rx						����ֻ����
SPI_Direction_1Line_Tx						����ֻ����

SPI_Mode
SPI_Mode_Master		����ģʽ
SPI_Mode_Slave		�ӻ�ģʽ
�������������������SCK�źţ����ӻ�����SCK�ź�

SPI_DataSize
SPI_DataSize_8b		1���ֽ�
SPI_DataSize_16b	�����ֽ�

SPI_CPOL
SPI_CPOL_High			����ʱSCK�ߵ�ƽ
SPI_CPOL_Low			����ʱSCK�͵�ƽ

SPI_CPHA
SPI_CPHA_1Edge		SCK�������زɼ�
SPI_CPHA_2Edge		SCKż�����زɼ�

SPI_NSS
SPI_NSS_Hard			NSSӲ������ 
SPI_NSS_Soft 			NSS����������Լ����ã�

SPI_BaudRatePrescaler
SPI_BaudRatePrescaler_2				��ʱ��2��Ƶ
SPI_BaudRatePrescaler_4				��ʱ��4��Ƶ
SPI_BaudRatePrescaler_8				��ʱ��8��Ƶ
SPI_BaudRatePrescaler_16			��ʱ��16��Ƶ
SPI_BaudRatePrescaler_32			��ʱ��32��Ƶ
SPI_BaudRatePrescaler_64			��ʱ��64��Ƶ
SPI_BaudRatePrescaler_128			��ʱ��128��Ƶ
SPI_BaudRatePrescaler_256			��ʱ��256��Ƶ

SPI_FirstBit
SPI_FirstBit_MSB		��λ��������
SPI_FirstBit_LSB		��λ��������

SPI_CRCPolynomial
SPI �� CRC У���еĶ���ʽ����ʹ�� CRC У��ʱ����ʹ�������Ա�Ĳ���(����ʽ)�������� CRC ��ֵ��
û��crc����Ļ�����λ����Ϊ����ֵ[0,7]
*/



#define SPI_timeout 0xff       //ͨ��ʱ��ʱ

class SPI
{
	
    public:
    void inti			//��ʼ��SPI,��ʼ����ɺ�SCK/MOSI/MISO ���ų�ʼ���ɸ�������ģʽ��CS����������ƣ��ʳ�ʼ��Ϊ��ͨ�������ģʽ
	(
	SPI_TypeDef* SPIx,
	uint16_t SPI_Direction,										//���� SPI �ĵ�˫��ģʽ
	uint16_t SPI_Mode,												//���� SPI ����/�ӻ���ģʽ 
	uint16_t SPI_DataSize,										//���� SPI ������֡���ȣ���ѡ 8/16 λ
	uint16_t SPI_CPOL,												//����ʱ�Ӽ��� CPOL����ѡ��/�͵�ƽ
	uint16_t SPI_CPHA,												//����ʱ����λ����ѡ��/ż�����ز���
	uint16_t SPI_NSS,													//���� NSS ������ SPI Ӳ�����ƻ����������
	uint16_t SPI_BaudRatePrescaler,						//����ʱ�ӷ�Ƶ���ӣ�fpclk/��Ƶ��=fSCK
	uint16_t SPI_FirstBit,										//���� MSB/LSB ����
	uint16_t SPI_CRCPolynomial								//���� CRC У��ı��ʽ
	);

    FunctionalState send_16b(SPI_TypeDef* SPIx,uint16_t data,uint16_t* reply);                  //�������ݣ�16λ�������Ҷ�ȡ���ص�����
    FunctionalState	send_8b(SPI_TypeDef* SPIx,uint8_t data,uint8_t* reply);						//�������� (8λ)�����Ҷ�ȡ���ص�����
    FunctionalState	send_8b(SPI_TypeDef* SPIx,uint8_t data);						            //�������� (8λ)
    FunctionalState send_16b(SPI_TypeDef* SPIx,uint16_t data);                                  //�������ݣ�16λ��


//���NSS�����з�װһ�������ͽ������ֱ�Ϊ���ͺ����ߣ�Ȼ��send_16b��read_16b���м�
};






#endif
