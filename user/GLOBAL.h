/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      GLOBAL.h
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:����DTS ASϵͳ�е�ȫ�ֱ��� 
*   Copyright (C) 
*
********************************************************************************/
#if !defined(_GLOBAL_H_)
#define _GLOBAL_H_
#include "stm8s_lib.h"
// ************************* Typedef data type  *********************************
typedef unsigned 	char  	u08;
//typedef unsigned 	char  	u8;
typedef          	char  	s08;
//typedef unsigned 	short 	u16;
//typedef          	short 	s16;
//typedef unsigned 	long 	u32;
//typedef          	long 	s32;
typedef unsigned 	char  	uchar;
typedef unsigned 	int   	uint;
typedef unsigned long long  u64;
typedef   signed long long  s64;

typedef unsigned char   	BOOL;
typedef unsigned char		BYTE;
typedef unsigned int		WORD;
typedef unsigned long		DWORD;

typedef unsigned char		byte;
typedef unsigned int		word;
typedef unsigned long		dword;

typedef unsigned char		UCHAR;
typedef unsigned int		UINT;
typedef unsigned short  	USHORT;
typedef unsigned long		ULONG;

#ifndef NULL
#define NULL 0
#endif

//stm8  eeprom

//By HeYC 0906	#define SW_SET_ADDR               ((u32)(FLASH_DATA_START_PHYSICAL_ADDRESS + 0))//������ʼ���õ�ַ0x004000
//By HeYC 0906	#define ENBALE_PROTECTION_ADDR    ((u32)(SW_SET_ADDR + 1))//��籣��ʹ��0x004001
//By HeYC 0906	#define ENBALE_KEY_ADDR           ((u32)(ENBALE_PROTECTION_ADDR + 1))//���ذ���ʹ��0x004002
#define SW_ID_ADDR                ((u32)(FLASH_DATA_START_PHYSICAL_ADDRESS + 0))//��SW��MAC��ַ��12�ֽ�0x004003
//By HeYC 0906	#define SW_PIN_ADDR               ((u32)(SW_ID_ADDR + 12))//PIN�룬3�ֽ�		0x00400F
//By HeYC 0906	#define SW_CLOCK_ADDR             ((u32)(SW_PIN_ADDR + 3))//128bytes len+10*(#1#120110#1)		0x004012
//By HeYC 0906	#define SW_DELAY_TIME_ADDR        ((u32)(SW_CLOCK_ADDR + 128))//2�ֽڣ���λ��	0x004092
//By HeYC 0906	#define SW_SERVICE_IP_ADDR          ((u32)(SW_DELAY_TIME_ADDR + 2))//���15�ֽڣ�210.209.73.47		0x004094
//By HeYC 0906	#define SW_SERVICE_PORT_ADDR        ((u32)(SW_SERVICE_IP_ADDR + 20))//7887								0x0040A8
//By HeYC 0906	#define SW_CURRENT_P_ADDR           ((u32)(SW_SERVICE_PORT_ADDR + 20))//��������					0x0040BC
//By HeYC 0906	#define SW_CURRENT_P_DAY_ADDR       ((u32)(SW_CURRENT_P_ADDR + 4))//�µ���31�죬ÿ�����		0x0040C0
//By HeYC 0906	#define SW_CURRENT_DAY_ADDR         ((u32)(SW_CURRENT_P_DAY_ADDR + 32))//���������������31�죬0����	0x0040E0
//By HeYC 0906	#define SW_NULL_ADDR                ((u32)(SW_CURRENT_P_DAY_ADDR +1))//							0x0040E1
//By HeYC 0906	#define SW_UDP_PORT_ADDR            ((u32)(SW_NULL_ADDR +1))//									0x0040E2
#define SW_SETorUNSETSTATE          ((u32)(SW_ID_ADDR +sizeof(CfgParaInFlash)))//							0x0040E3
//By HeYC 0906	#define SW_SSID_ADDR             ((u32)(SW_SETorUNSETSTATE +20))//								0x0040F7
//By HeYC 0906	#define SW_SSIDPASSWORD_ADDR             ((u32)(SW_SSID_ADDR +20))//							0x00410B

//stm8  eeprom


//at24c512
#define OffsetAddr                        128
#define INFRARED_PRE_ADDR                ((u16)(0))//
#define RF315_PRE_ADDR                   ((u16)(200*OffsetAddr))//
#define RF433_PRE_ADDR                   ((u16)(280*OffsetAddr))//
#define RF315_PRE_ADDR_SPL               ((u16)(360*OffsetAddr))//
#define RF433_PRE_ADDR_SPL               ((u16)(400*OffsetAddr))//


//RTCʱ��
typedef struct __RFS
{
  u08 type;
  u08 number;
  u08 state;
  u08 sendflag;
  u08 replyCounter;//RF�ط����� 
  int PulseN;
  int PulseLEN;
//By HeYC 0831  int sleep;
  int SENDSTEP;
//By HeYC 0831	  int SendFinish;

}RFS;//RTCʱ��ṹ��
extern RFS rfs;

//at24c512

//extern char OFTS_AS_VERSION[4];	//�汾��

//RTCʱ��
/*typedef struct __RTCTIME
{
   WORD  year;//��
   byte month;//��
   byte date;//��
   byte day;//����
   byte hour;//ʱ
   byte min;//��
   byte sec;//��
   

}RTCTIME;//RTCʱ��ṹ��*/

//CLOCKʱ��
/*typedef struct __CLOCKTIME
{
   byte hour;
   byte min;
   byte sec;
   byte state;
   byte num;
   
}CLOCKTIME;*/

typedef  struct   flash_
{
  u16  UDL;     //û��
  u16  len;
  u08  buf[100];
  u08  clientID;   //�������ֻ���ģ�飬��ô���ID��ָ�����ֻ���ID,1���ֻ���2���ֻ���������
} flash;
///
typedef struct __AIRCON
{
   byte func;
   byte codeValH;
   byte codeValL;
   byte TimeMin;
   byte onoff;
   byte mode;
   byte temp;
   byte windspeed;
   byte winddir;
   byte enable;//���Ʊ�־λ����1ʱ��ʾ��Ҫ����
   byte step;//����״̬������
   byte ack;//״̬����
   byte codeask;//������ֵ�ɹ���־λ
   byte cmdsetp;
   
}AIRCON;
//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// UART��Ϣ������ID����
//
#define COM1		0 // 
#define COM3		2 // 

#define COM_WIFI	COM1 
#define COM_IRBALL	COM3 




#define UART0_RX_BUFFER_SIZE		400	//COM1
#define UART3_RX_BUFFER_SIZE		1	//COM1
#define COMM1_BUF_LEN	10
#define TIMING_MONITOR_WIFI		3600*2	// 

/////////////////////////////////////////////////////////
typedef  byte (*AppCommRecvPtr)(const byte *buf, byte len, byte Addr);
typedef struct APP
{
	byte Port;	// ����˿ں�
	AppCommRecvPtr fcom; //Ӧ�ò㺯��
} app_com;


/*typedef struct current_
{
	WORD Iave;
        WORD I;	// 
        DWORD P;
        byte DAYP;
        byte day;
        byte Pbuf[40];
}current;*/
//////////////////////////////////////////////////////////////


//extern byte COMM1RecvBuf[COMM1_BUF_LEN];
extern app_com app[5];//�����˿ں�������


/************************ λ�������� ***********************/
typedef union {
  byte Byte;
  struct {
    byte bit0        :1;                                       
    byte bit1        :1;                                       
    byte bit2        :1;                                      
    byte bit3        :1;                                       
    byte bit4        :1;                                       
    byte bit5        :1; 
    byte bit6        :1; 
    byte bit7        :1;                                       
  } Bits;
} DBit;

////////////////RF//////////////////////////////////////////////////////////
#define  IR_END        800 //500mS  �����ж�
#define  IR_START      1000 //5mS, ��ʼ�ж�
#define  IR_LEARN_TIMEOUT 1000000 //10S�˳�ѧϰ
#define  IR_MAX_LEN    243//ѧϰ����ֽ���
#define  IRCODELENGTH		   10

#define  ALARM_TIMEOUT        5000 //5�볬ʱ�˳�
enum RF_IR_COMMAND
{
	IR_PRE = 0,
	IR_GUIDE,
	IR_DECODE,
	IR_STOP
};

enum RF_IR_SEND
{
	IRS_PRE = 0,
	IRS_H,
	IRS_L,
	IRS_STOP
};

//Bgn:By HeYC 0830
typedef enum _IDLE_RF_RCV_STAT
{
	IRRS_IDLE=0,
	IRRS_315_GUIDE_RCVD,
	IRRS_433_GUIDE_RCVD,
	IRRS_315_RCV_SUCC,
	IRRS_433_RCV_SUCC
} IDLE_RF_RCV_STAT;

#define RF_TYPE_NULL	0
#define RF_TYPE_315		1
#define RF_TYPE_433		2
#define RF_TYPE_INFRA	3
//End:By HeYC 0830
typedef struct _IR
{
 u16 Rcounter; //���ռ�����
 u16 Scounter; //���ͼ�����
 u08 REVFLAG;  //���ձ�־λ
//By HeYC 0831	 u08 SEDFLAG;  //���ͱ�־λ
//By HeYC 0831 u08 COPYFLAG; //��ʼ���뿽��
 u08 DECODESTEP; //���벽��
 u08 SENDSTEP;   //���Ͳ���
 u08 Learnflag;//�����־��Ϊ1��ʾ����ɹ�
//By HeYC 0831	 u08 SendFinish; //������ɱ�־
//By HeYC 0831 u08 sleep;
 u08 guide;//�������־
 //u08 num;    
 u08 NO;         //��ǰָ���
 u32 timeout;  //����ʱ�䵽
 u32 PulseN;   //������������
 u32 PulseLEN; //�洢���峤�ȣ�����flash��
//By HeYC 0831 u32 ADDR_QUEUE[2];//��ַ���У�����������
 u16 cmd[256];     //�������д洢
 u32 rfRcvTimer;//By HeYC from Tout to rfRcvTimer.
}IR_;
extern IR_  IR;				    //RFָ��

typedef struct _ALARM_RF
{
 u16 Rcounter; //���ռ�����
 //by HeYC 0831	u16 Scounter; //���ͼ�����
 //u08 REVFLAG;  //���ձ�־λ
//By HeYC 0831	 u08 SEDFLAG;  //���ͱ�־λ
//By HeYC 0831	 u08 COPYFLAG; //��ʼ���뿽��
 u08 DECODESTEP; //���벽��
//By HeYC 0901	 u08 SENDSTEP;   //���Ͳ���
//By HeYC 0901	 u08 Learnflag;//�����־��Ϊ1��ʾ����ɹ�
 //By HeYC 0831	u08 SendFinish; //������ɱ�־
//By HeYC 0831 u08 sleep;
 u08 guide;//�������־
 //u08 num;    
 u08 NO;         //��ǰָ���
//By HeYC 0831 u32 timeout;  //����ʱ�䵽
 u32 PulseN;   //������������
//By HeYC 0831 u32 PulseLEN; //�洢���峤�ȣ�����flash��
 u32 Tout;
 u32 timeCounter;//��ʱ������
 u08 sending;//�����ź��ѷ���
}ALARM_RF;


//Bgn:By HeYC 0906
typedef struct _CfgParaInFlash
{
	char magicStr[5];		//"topan"
	u08  reserved1;		// reserved for other used
	u08 macAddr[12];
	char serverAddr[20];	//ex:"topansh.wicp.net"
	char serverPort[10];	//"7269"
	char SSIDToJoin[20];
	char SSIDPwd[20];
}CfgParaInFlash;
//End:By HeYC 0906
extern ALARM_RF  alarm;				    //RF����ָ��

/////////////////////////////RF//////////////////////////////////////////////

extern volatile              DBit _BitParament0;
#define sys10msFlag 		_BitParament0.Bits.bit0 	/* 10ms��־ 			*/
#define sys1msFlag	 	    	_BitParament0.Bits.bit1 	/* CAN���ݽ��ձ�־ 	*/
#define KEYflag		        _BitParament0.Bits.bit2 	/* �������Ʊ�־   */
#define beepout		        _BitParament0.Bits.bit3 	/* 0����ͣ�1����� */
#define KEYControlState		_BitParament0.Bits.bit4 	/* ����״̬��0�����У�1���������ڶ���  */
#define Nightflag     		_BitParament0.Bits.bit5 	/* ҹ���־��ҹ���ͱ����� */		  
#define LONGKEYFALG         _BitParament0.Bits.bit6 	/* ��������־ */
#define BEEPflag            _BitParament0.Bits.bit7 	/* 0�أ�1�� */


extern volatile word   	    tickcount;      /* Timer tick count   		*/
extern volatile byte 	    sysTickfor1ms;	/* 10millisecond timer 		*/
extern volatile byte 	    sysTickfor1us;	/* 10millisecond timer 		*/
extern volatile byte            Markfor1us;
extern volatile word 		sysTickfor10ms;	/* 10millisecond timer 		*/
extern volatile byte 		sysTickfor100ms;/* 100millisecond timer 	*/
extern volatile byte 		sysTickfor500ms;   /* 500millisecond timer 	*/
extern volatile byte 		sysTickforRtc1s;   /* 1second timer 	*/
extern volatile byte 		  sysTickfor8s;   /* 8second timer 	*/
extern volatile byte 		  sysTickfor5s;   /* 5second timer 	*/
extern volatile byte 		  sysTickfor6s;   /* 6second timer 	*/
extern volatile u16		uartwifiTimer;	//���ڶ�ʱ������ʱ��ջ�����
extern volatile word         Heartbeattimer5s;
extern volatile byte         Heartbeattimer5s_count;
extern volatile byte         Heartbeattimer5sflag;
extern volatile byte           flagGotAppCfgInfo;

#define WIFILENGTH  2
extern flash  WIFISBUF[WIFILENGTH];	////wifi������
extern flash  WIFITxMessage;	//WiFi��������
extern u08    WIFI_Head;//WiFi���λ�����ͷ
extern u08    WIFI_Hail;//WiFi���λ�����β
//by HeYC 0906	extern u08    WIFIENBLE;     // //wifiʹ��
extern u08    WaitMONITOR_IDELSend; ////�ȴ�wifi����������ɱ�־λ
extern int smsSMSStep;//WiFi��Ϣ���Ͳ���
//By HeYC 0906	extern char macid[20];//����mac��ַ
extern char macidHEX[6];//����mac��ַ
//by heyc 0906	extern char   revmacid[20];//�յ���mac���к�
//BY HeYC 0906	extern char   staClientIP[20];//STAģʽԶ�̷�����IP
//By HeYC 0906	extern char   staClientPort[10];//STAʽԶ�̷������˿ں�
extern char   UDPPort[10];//��������ʱ��UDP�˿�
//by heyc 0906	extern char   revmacid[20];
//By HeYC 0906	extern WORD macpin;//mac����
//by heyc 0906	extern DWORD  macidlong;//macת������
extern u08  waitsmssend;  //�յ��ȴ�'>'������־λ����ʾ���Է������ݵ�WIFI
//by heyc 0906	extern u08  smssenddata;
//by heyc 0906	extern u08 keystate;//����״̬
extern u08  WifiBusy; //ģ��æ�����ܻظ�����

///rtc
//extern  RTCTIME tm;
//extern  CLOCKTIME clcoks[10];

//extern u08 ENBALE_SET;//ȡ��
//extern u08 ENBALE_PROTECTION;//ȡ��
//By HeYC 0906	extern u08 ENBALE_KEY;//ȡ��
//extern char clock[128];
extern u08 eeprombuf[270];
extern u08 splflage[2];
//extern char IRString[50];//��������
//extern u8 IRDatas[25];//����������
//By HeYC 0906	extern u16 delayOffTime;//ȡ��

//by heyc 0906	extern char ssid[20];//·�����˺�
//by heyc 0906	extern char ssidPassword[20];//·��������
extern char apssid[20];//ģ����˺�
extern char apssidPassword[20];//ģ�������
//extern current currents;//ȡ��
//extern u08 REALYState;//ȡ��
//By HeYC 0822	extern u08 setSSid;
//By HeYC 0822	extern u08 apsetSSid; 
extern u08 ClientID;

//#define REAL_ANALOG_NUM		1	// ʵ�ʵ�ģ��ͨ������
//extern word AnalogData[REAL_ANALOG_NUM]; //ȡ��
//extern word delayoffCounter;//ȡ��
extern u08 wifiEnable;//wifiʹ��
//extern u08  adStart;//ȡ��
extern u08 STAEnable;//staģʽ����·�����ɹ�
extern u08 staClientConnent;//1��ʾԶ�����ӳɹ�
extern u32 staClientCounter;//
extern u08 staInitMsg;
extern u08 staClientConnentStart;
extern u08 ATCWSAPEnable;
extern AIRCON  air;
extern u08 read_IR315_trig;        //
extern u08 read_IR433_trig;
extern u8 send_flag;
extern u32 TimeOut;
extern u08 CodeBuf[IRCODELENGTH];
extern u08 CodeHead,CodeTail;
extern u32 gAppCfgSuccTmout;
//�������״̬
//By HeYC 0906	extern u08 SetOrUnsetState[1];
extern u08	curSecuMode;//by heyc 0906
extern volatile byte curSecuModeShadow;
//By HeYC 	extern char rfInterLock;
extern IDLE_RF_RCV_STAT rfInterLock;//By HeYC
//By HeYC 0830	extern u08 alarmActive;//�����źŶ���ɹ�
extern u08 Authenticationflag;
extern u08 CNT;
extern u08 wifiMDInitflag;
extern u08 ClientCounterA;
extern u08 hartconnectflag;
extern byte HFlagWhereAuthSetZero;
extern volatile byte secModChangedFlag;//By HeYC
extern int wifiInitMsg;//By HeYC 
extern CfgParaInFlash   gCfgPara;//By HeYC 0906
/////////////////////////////////////////////////////////////////////

#endif // !defined(_GLOBAL_H_) 
		
/* EOF */
