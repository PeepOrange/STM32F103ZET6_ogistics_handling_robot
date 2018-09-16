#ifndef __ESP8266_H__
#define __ESP8266_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#include "stm32f10x.h"                  // Device header
#include "gpio.h"


//���ô���3��ESP8266ͨ��,�жϷ������ڶ�Ӧ�� .cpp  �ļ���
//��������֡ʱ�����ȹرմ����ж�֮���ٿ���


#define ESP8266_USART_BAUD_RATE      115200                  //USART3������



//Set_AP_Mode()  ����
#define ESP8266_TcpServer_IP         "192.168.123.169"      //������������IP��ַ
#define ESP8266_BulitApSsid          "ESP8266ģ��WIFI"      //�������ȵ������
#define ESP8266_BulitApPwd           "abcd12345"            //�������ȵ������ (����8λ)
#define ESP8266_BulitApEcn            WPA_PSK               //�������ȵ�ļ��ܷ�ʽ
#define ESP8266_TcpServer_Port       "8080"                 //�����������Ķ˿�   
#define ESP8266_TcpServer_OverTime   "1800"                 //��������ʱʱ�䣨��λ���룩



//Set_STA_Mode()  ����
#define ESP8266_ApSsid               "ESP8266ģ��WIFI"                //Ҫ���ӵ��ȵ������
#define ESP8266_APPwd                "abcd12345"                     //Ҫ���ӵ��ȵ����Կ
#define ESP8266_NetPro               TCP                             //����Э��
#define ESP8266_Link_TcpServer_IP    "192.168.0.11"                   //Ҫ���ӵķ������� IP
#define ESP8266_TcpServer_Port       "8080"                          //Ҫ���ӵķ������Ķ˿�




#define USART3_Printf( fmt, ... )           USART_printf ( USART3, fmt, ##__VA_ARGS__ ) 



struct ESP8266_Gpio
{
  GPIO  *CH_PD;
  GPIO  *RST;
  GPIO  *USART_Rx;
  GPIO  *USART_Tx;   
};

enum ENUM_Net_Mode  //����ģʽ
{
    STA,
    AP,
    STA_AP  
};

enum ENUM_AP_PsdMode    //���ܷ�ʽ
{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
};


enum ENUM_NetPro    //����Э��
{
    TCP,
    UDP,
};
	

enum ENUM_ID_NO     //ģ�����ӷ�������ID
{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
};

enum ENUM_Link_Status    //����״̬
{
    IP_Get,
    Link_Set,
    Link_Lost,
    Status_Get_Fall,
};




#define RX_BUF_MAX_LEN     1024                              //�����ջ����ֽ���
struct  STRUCT_USART3_Fram                                  //��������֡�Ĵ���ṹ��
{
  char  Data_RX_BUF [ RX_BUF_MAX_LEN ];                     //�������������
	
  union 
  {
    __IO uint16_t InfAll;
    struct 
      {
		  __IO uint16_t FramLength       :15;        // 14:0 λ���������ʾFramLengthռ15λ   �������浱ǰ�����������֡�ڼ����ֽ�
		  __IO uint16_t FramFinishFlag   :1;        // 15   FramFinishFlagռ���һλ         �������жϺ����м�Ӹ��ⲿ�����ṩ����֡������־
	  } InfBit;
  }; 
	
};


//���յ�������   strEsp8266_Fram_Record.Data_RX_BUF ������
extern STRUCT_USART3_Fram strEsp8266_Fram_Record;




class ESP8266
{
    
    public:
    ESP8266(ESP8266_Gpio *ESP8266_gpio);
    void    Init();
    void    Rst();
    void    Set_AP_Mode();          //��ΪWIFI�ȵ�
    void    Set_STA_Mode();         //��Ϊ�ͻ��˷������ݸ�����(͸������ģʽ)
    void    STA_Send(char *str,...);   //�����������STAģʽ����
    
    
    
    protected:
    void    AT_Test();
    bool    Set_NetMode(ENUM_Net_Mode NetMode  );           //���ù���ģʽ
    bool    JoinAP(char * pSSID, char * pPassWord);        //�����ⲿWIFI
    bool    BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode PsdMode);   //�����ȵ�
    bool    MultipleId(FunctionalState ok);                 //������رն�����
    bool    LinkServer(ENUM_NetPro enumE, char * ip, char * ComNum, ENUM_ID_NO id);     //WF-ESP8266ģ�������ⲿ������
    bool    ServerMode(FunctionalState enumMode, char * pPortNum, char * pTimeOver);    //������رշ�����ģʽ
    ENUM_Link_Status    Get_LinkStatus_One();       //���ESP8266����״̬,���˿�
    uint8_t  Get_LinkStatus_Mul();                  //���ESP8266��˿�����״̬       
    bool    Get_APIP(char * p_ApIp, uint8_t ArrayLength);       //��ȡAPIP
    bool    UnvarnishSend(FunctionalState ok);          //������˳�͸������
    bool    SentString(FunctionalState EnUnvarnishTx, char * pStr, u32 StrLength, ENUM_ID_NO Id);       //ESP8226�����ַ���
    char*   ReceiveString(FunctionalState EnUnvarnishTx);                  //ESP8226���յ��ַ���
    bool    Get_IP(char * p_StaIp);             //��ѯ�ѽ����豸��IP
    bool    Set_APIP(char * p_ApIp);                         //����ESP8266��APIP
    

    private:
    ESP8266_Gpio*    gpio;
    void Init_Gpio();           //���ų�ʼ��
    void Init_USART3_IT();      //USART3�����жϳ�ʼ�� ������|�����ж�
    inline void CH_PD_Set();
    inline void CH_PD_Reset();
    inline void RST_Set();
    inline void RST_Reset();
    bool    Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime);

    
   
};








#endif


