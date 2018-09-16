#ifndef __ESP8266_H__
#define __ESP8266_H__

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#include "stm32f10x.h"                  // Device header
#include "gpio.h"


//利用串口3与ESP8266通信,中断服务函数在对应的 .cpp  文件中
//处理数据帧时可以先关闭串口中断之后再开启


#define ESP8266_USART_BAUD_RATE      115200                  //USART3波特率



//Set_AP_Mode()  设置
#define ESP8266_TcpServer_IP         "192.168.123.169"      //服务器开启的IP地址
#define ESP8266_BulitApSsid          "ESP8266模拟WIFI"      //建立的热点的名字
#define ESP8266_BulitApPwd           "abcd12345"            //建立的热点的密码 (至少8位)
#define ESP8266_BulitApEcn            WPA_PSK               //建立的热点的加密方式
#define ESP8266_TcpServer_Port       "8080"                 //服务器开启的端口   
#define ESP8266_TcpServer_OverTime   "1800"                 //服务器超时时间（单位：秒）



//Set_STA_Mode()  设置
#define ESP8266_ApSsid               "ESP8266模拟WIFI"                //要连接的热点的名称
#define ESP8266_APPwd                "abcd12345"                     //要连接的热点的密钥
#define ESP8266_NetPro               TCP                             //网络协议
#define ESP8266_Link_TcpServer_IP    "192.168.0.11"                   //要连接的服务器的 IP
#define ESP8266_TcpServer_Port       "8080"                          //要连接的服务器的端口




#define USART3_Printf( fmt, ... )           USART_printf ( USART3, fmt, ##__VA_ARGS__ ) 



struct ESP8266_Gpio
{
  GPIO  *CH_PD;
  GPIO  *RST;
  GPIO  *USART_Rx;
  GPIO  *USART_Tx;   
};

enum ENUM_Net_Mode  //工作模式
{
    STA,
    AP,
    STA_AP  
};

enum ENUM_AP_PsdMode    //加密方式
{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
};


enum ENUM_NetPro    //网络协议
{
    TCP,
    UDP,
};
	

enum ENUM_ID_NO     //模块连接服务器的ID
{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
};

enum ENUM_Link_Status    //连接状态
{
    IP_Get,
    Link_Set,
    Link_Lost,
    Status_Get_Fall,
};




#define RX_BUF_MAX_LEN     1024                              //最大接收缓存字节数
struct  STRUCT_USART3_Fram                                  //串口数据帧的处理结构体
{
  char  Data_RX_BUF [ RX_BUF_MAX_LEN ];                     //用来保存的数据
	
  union 
  {
    __IO uint16_t InfAll;
    struct 
      {
		  __IO uint16_t FramLength       :15;        // 14:0 位域操作，表示FramLength占15位   用来保存当前保存这个数据帧第几个字节
		  __IO uint16_t FramFinishFlag   :1;        // 15   FramFinishFlag占最后一位         用来在中断函数中间接给外部函数提供数据帧结束标志
	  } InfBit;
  }; 
	
};


//接收的数据在   strEsp8266_Fram_Record.Data_RX_BUF 数组中
extern STRUCT_USART3_Fram strEsp8266_Fram_Record;




class ESP8266
{
    
    public:
    ESP8266(ESP8266_Gpio *ESP8266_gpio);
    void    Init();
    void    Rst();
    void    Set_AP_Mode();          //作为WIFI热点
    void    Set_STA_Mode();         //作为客户端发送数据给电脑(透传发送模式)
    void    STA_Send(char *str,...);   //配置完成上面STA模式可用
    
    
    
    protected:
    void    AT_Test();
    bool    Set_NetMode(ENUM_Net_Mode NetMode  );           //设置工作模式
    bool    JoinAP(char * pSSID, char * pPassWord);        //连接外部WIFI
    bool    BuildAP(char * pSSID, char * pPassWord, ENUM_AP_PsdMode PsdMode);   //创建热点
    bool    MultipleId(FunctionalState ok);                 //开启或关闭多连接
    bool    LinkServer(ENUM_NetPro enumE, char * ip, char * ComNum, ENUM_ID_NO id);     //WF-ESP8266模块连接外部服务器
    bool    ServerMode(FunctionalState enumMode, char * pPortNum, char * pTimeOver);    //开启或关闭服务器模式
    ENUM_Link_Status    Get_LinkStatus_One();       //获得ESP8266连接状态,单端口
    uint8_t  Get_LinkStatus_Mul();                  //获得ESP8266多端口连接状态       
    bool    Get_APIP(char * p_ApIp, uint8_t ArrayLength);       //获取APIP
    bool    UnvarnishSend(FunctionalState ok);          //进入或退出透传发送
    bool    SentString(FunctionalState EnUnvarnishTx, char * pStr, u32 StrLength, ENUM_ID_NO Id);       //ESP8226发送字符串
    char*   ReceiveString(FunctionalState EnUnvarnishTx);                  //ESP8226接收的字符串
    bool    Get_IP(char * p_StaIp);             //查询已接入设备的IP
    bool    Set_APIP(char * p_ApIp);                         //设置ESP8266的APIP
    

    private:
    ESP8266_Gpio*    gpio;
    void Init_Gpio();           //引脚初始化
    void Init_USART3_IT();      //USART3及其中断初始化 ，接受|空闲中断
    inline void CH_PD_Set();
    inline void CH_PD_Reset();
    inline void RST_Set();
    inline void RST_Reset();
    bool    Cmd(char * cmd, char * reply1, char * reply2, uint32_t waittime);

    
   
};








#endif


