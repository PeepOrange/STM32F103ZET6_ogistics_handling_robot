#include "ESP8266.h"
#include <stdarg.h>
#include "Common_Function.h"
#include <stdio.h>  



#ifdef __cplusplus
extern "C"
{
#include "system.h"
#include <string.h>  
#include <stdbool.h>
};
#endif



STRUCT_USART3_Fram  strEsp8266_Fram_Record = { 0 };






ESP8266::ESP8266(ESP8266_Gpio *ESP8266_gpio)
{
    this->gpio=ESP8266_gpio;
}

void ESP8266::Init_Gpio()
{
    gpio->CH_PD->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    
    gpio->RST->mode(GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
    
    gpio->USART_Rx->mode(GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);
    
    gpio->USART_Tx->mode(GPIO_Mode_AF_PP,GPIO_Speed_50MHz);
    
}

void ESP8266::Init_USART3_IT()
{
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);   //开启串口3时钟
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = ESP8266_USART_BAUD_RATE;            //115200波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
    
    USART_ITConfig ( USART3, USART_IT_RXNE, ENABLE ); //使能串口接收中断 
	USART_ITConfig ( USART3, USART_IT_IDLE, ENABLE ); //使能串口总线空闲中断 	
        
    NVIC_InitTypeDef NVIC_InitStructure; 
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    
    USART_Cmd(USART3, ENABLE);
}


inline void ESP8266::CH_PD_Set()
{
    this->gpio->CH_PD->set(); 
}
inline void ESP8266::CH_PD_Reset()
{
    this->gpio->CH_PD->reset();
}
inline void ESP8266::RST_Set()
{ 
    this->gpio->RST->set();
}
inline void ESP8266::RST_Reset()
{
    this->gpio->RST->reset();
}


void ESP8266::Init()
{
    ESP8266::Init_Gpio();
    ESP8266::Init_USART3_IT();
    ESP8266::RST_Set();
    ESP8266::CH_PD_Reset();
}




bool  ESP8266::SentString(FunctionalState EnUnvarnishTx, char * pStr, u32 StrLength, ENUM_ID_NO Id)
{
 	char cStr [20];
	bool bRet = false;

	if ( EnUnvarnishTx )
	{
		USART3_Printf ( "%s", pStr );
		
		bRet = true;
		
	}

	else
	{
		if ( Id < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", Id, StrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", StrLength + 2 );
		
		ESP8266::Cmd ( cStr, "> ", 0, 100 );

		bRet = ESP8266::Cmd ( pStr, "SEND OK", 0, 500 );
    }	
	return bRet;   
}




bool ESP8266::Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime)
{
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	USART3_Printf ( "%s\r\n", cmd );
    
    

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	delay_ms( waittime );                 //延时
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
    
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );    
}


void ESP8266::Rst()
{
    
  	 ESP8266::RST_Reset();
	 delay_ms ( 500 ); 
	 ESP8266::RST_Set();  
    
}




void ESP8266::AT_Test()
{
	char count=0;
   
    CH_PD_Set();
	RST_Set();	
	delay_ms ( 1000 );
	while ( count < 10 )
	{
		if( ESP8266::Cmd( "AT", "OK", NULL, 500 ) ) return;
		ESP8266::Rst();
		++ count;
	}    
    
    
}

bool  ESP8266::Set_NetMode(ENUM_Net_Mode NetMode )
{
	switch ( NetMode )
	{
      case STA:
		return ESP8266::Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		return ESP8266::Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
      case STA_AP:
		return ESP8266::Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
    }    
}


bool ESP8266::JoinAP(char * pSSID, char * pPassWord)
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266::Cmd ( cCmd, "OK", NULL, 5000 );       
}


bool ESP8266::BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode PsdMode)
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, PsdMode );

	return ESP8266::Cmd ( cCmd, "OK", 0, 1000 );  
   
}


bool ESP8266::MultipleId(FunctionalState ok)
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( ok ? 1 : 0 ) );
	
	return ESP8266::Cmd ( cStr, "OK", 0, 500 );       
}


bool ESP8266::LinkServer(ENUM_NetPro enumE, char * ip, char * ComNum, ENUM_ID_NO id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case TCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case UDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

  return ESP8266::Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );    
}



bool ESP8266::ServerMode(FunctionalState enumMode, char * pPortNum, char * pTimeOver)
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266::Cmd ( cCmd1, "OK", 0, 500 ) &&ESP8266::Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );
		return ESP8266::Cmd ( cCmd1, "OK", 0, 500 );
	}    
      
}


ENUM_Link_Status    ESP8266::Get_LinkStatus_One()
{
	if ( ESP8266::Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
        
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return IP_Get;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return Link_Set;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return Link_Lost;		
	}
	return Status_Get_Fall;        
}




uint8_t  ESP8266::Get_LinkStatus_Mul()
{
	uint8_t ucIdLinkStatus = 0x00;	
	if ( ESP8266::Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	

	}
	return ucIdLinkStatus;        
}



bool ESP8266::Get_APIP(char * p_ApIp, uint8_t ArrayLength)
{   
	char uc;	
	char * pCh;	
    ESP8266::Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	else
		return 0;
	for ( uc = 0; uc < ArrayLength; uc ++ )
	{
		p_ApIp [ uc ] = * ( pCh + uc);
		
		if ( p_ApIp [ uc ] == '\"' )
		{
			p_ApIp [ uc ] = '\0';
			break;
		}
	}	
	return 1;    
}

bool ESP8266::UnvarnishSend(FunctionalState ok)
{
    if(ok)
    {
        if ( ! ESP8266::Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) )  return false ;		
        return ESP8266::Cmd ( "AT+CIPSEND", "OK", ">", 500 );               
    }
    else
    {
        delay_ms ( 1000 );
        USART3_Printf ( "+++" );
        delay_ms ( 500 );          
        return 1;
    }    
}


char* ESP8266::ReceiveString(FunctionalState EnUnvarnishTx)
{
	char * pRecStr = 0;
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( EnUnvarnishTx )    pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
			
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	}
	return pRecStr;    
}



bool ESP8266::Get_IP(char *p_StaIp)
{
	uint8_t uc, ucLen;
	char * pCh, * pCh1;
    
    ESP8266::Cmd ( "AT+CWLIF", "OK", 0, 100 );
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "," );
	
	if ( pCh )
	{
      pCh1 = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "AT+CWLIF\r\r\n" ) + 11;
	  ucLen = pCh - pCh1;
	}
	else    return 0;
    
	for ( uc = 0; uc < ucLen; uc ++ )
		p_StaIp [ uc ] = * ( pCh1 + uc);
	
	p_StaIp [ ucLen ] = '\0';
	
	return 1;    
}

bool ESP8266::Set_APIP(char * p_ApIp)
{
	char cCmd [ 30 ];
    
	sprintf ( cCmd, "AT+CIPAP=\"%s\"", p_ApIp );
	
    if ( ESP8266::Cmd ( cCmd, "OK", 0, 5000 ) )
		return 1;
	else 
		return 0;    
    
    
}


void ESP8266::Set_AP_Mode()
{
    ESP8266::AT_Test();
    ESP8266::Set_NetMode( AP );
    while ( ! ESP8266::Set_APIP( ESP8266_TcpServer_IP ) ); 
    while ( ! ESP8266::BuildAP ( ESP8266_BulitApSsid,ESP8266_BulitApPwd,ESP8266_BulitApEcn ) );	
    ESP8266::MultipleId(ENABLE);
    while ( !	ESP8266::ServerMode ( ENABLE, ESP8266_TcpServer_Port, ESP8266_TcpServer_OverTime ) );
}




void ESP8266::Set_STA_Mode()
{
    ESP8266::AT_Test();
    ESP8266::Set_NetMode( STA );
    while(! ESP8266::JoinAP(ESP8266_ApSsid, ESP8266_APPwd) );
    ESP8266::MultipleId(DISABLE);
    while ( !	ESP8266::LinkServer ( ESP8266_NetPro, ESP8266_Link_TcpServer_IP, ESP8266_TcpServer_Port, Single_ID_0 ) );
    while ( ! ESP8266::UnvarnishSend(ENABLE) );
}


void ESP8266::STA_Send(char *str,...)
{
    char cStr [ 100 ] = { 0 };
    sprintf ( cStr,str);
    ESP8266::SentString ( ENABLE, cStr, 0, Single_ID_0 );               //发送数据
}




#ifdef __cplusplus
 extern "C"
{
void USART3_IRQHandler()
{
    uint8_t ucCh;
	
	
	if ( USART_GetITStatus ( USART3, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( USART3 );
		
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;
	}
	 	 
	if ( USART_GetITStatus( USART3, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕(空闲中断)
	{
    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;		
    ucCh = USART_ReceiveData( USART3 );                                                  //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)			
    }	
}  ; //数据帧接收函数
}
#endif
































