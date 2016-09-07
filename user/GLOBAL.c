/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      GLOBAL.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:����ϵͳ�е�ȫ�ֱ��� 
*   Copyright (C) 
*
********************************************************************************/
#include "GLOBAL.h"

char OFTS_AS_VERSION[4]={'V','1','.','0'};//�汾��
//////////////////UART/////////////////////////
//byte COMM1RecvBuf[COMM1_BUF_LEN];
app_com app[5];//�����˿ں�������

///////////////////////////////////////////
volatile                  DBit _BitParament0;
volatile word   	  tickcount;      /* Timer tick count   		*/
volatile byte 	          sysTickfor1ms;	/* 10millisecond timer 		*/
volatile byte 	          sysTickfor1us=0;	/* 10millisecond timer 		*/
volatile byte            Markfor1us=0;
volatile word 	          sysTickfor10ms;	/* 10millisecond timer 		*/
volatile byte 		  sysTickfor100ms;/* 100millisecond timer 	*/
volatile byte 		  sysTickfor500ms;   /* 500millisecond timer 	*/
volatile byte 		  sysTickforRtc1s;   /* 1second timer 	*/
volatile byte 		  sysTickfor8s;   /* 8second timer 	*/
volatile byte 		  sysTickfor5s;   /* 5second timer 	*/
volatile byte 		  sysTickfor6s;   /* 6second timer 	*/
volatile word         Heartbeattimer5s=0;
volatile byte         Heartbeattimer5s_count=0;
volatile byte         Heartbeattimer5sflag=0;
volatile byte         flagGotAppCfgInfo=0;
//By HeYC 0906	volatile byte    SetOrUnsetStateflag=0;//By HeYC
volatile byte    curSecuModeShadow=0;//By HeYC 0906
byte HFlagWhereAuthSetZero=0;//By HeYC
////////////////////////////////////////////////////////////////////////////////////////
volatile u16		uartwifiTimer;	


flash  WIFISBUF[WIFILENGTH];	////wifi������
flash  WIFITxMessage;	////WiFi��������
u08    WIFI_Head;//WiFi���λ�����ͷ   ��ͷ��β����ȵ�ʱ���������Ҫ���ͣ���ͷ��βָ��ͬһ����ַ��ʱ���غ��ˣ�û��Ҫ���͵����ݻ����ݷ������
u08    WIFI_Hail;//WiFi���λ�����β   
//by HeYC 0906	u08    WIFIENBLE;     // //wifiʹ��
u08    WaitMONITOR_IDELSend; ////�ȴ�wifi����������ɱ�־λ
u08    WifiBusy=0; //ģ��æ�����ܻظ�����,He: let it equal WIFI_IDEL(0) at beginning.
int    smsSMSStep;//WiFi��Ϣ���Ͳ���
//By HeYC 0906	char   macid[20]="1AFE34F3AFD4";//����mac��ַ
char   macidHEX[6]={0};//By HeYC let it equal init valal
//by heyc 0906	char   revmacid[20];//�յ���mac���к�
//By HeYC 0906	char   staClientIP[20]="topansh.wicp.net";//STAģʽԶ�̷�����IP, He: init it with default para.
//By HeYC 0906	char   staClientPort[10]="7269";//STAʽԶ�̷������˿ں�He: init it with default para.

char   UDPPort[10]={"59999"};
//By HeYC 0906	WORD macpin;//mac����
//by heyc 0906	DWORD  macidlong;//macת������
u08  waitsmssend; //�յ��ȴ�'>'������־λ����ʾ���Է������ݵ�WIFI
//by heyc 0906	u08  smssenddata; 
//by heyc 0906	u08 keystate;//����״̬

//RTCTIME tm;

//u08 ENBALE_SET;//ȡ��
//by heyc 0906	u08 ENBALE_PROTECTION;//ȡ��
//By HeYC 0906	u08 ENBALE_KEY;//ȡ��
//char clock[128];
u08 eeprombuf[270];
u08 splflage[2]={0x55,0xaa};
//CLOCKTIME clcoks[10];
//char IRString[50];//��������
//u8 IRDatas[25];//����������
//By HeYC 0906	u16 delayOffTime;//ȡ��

//by heyc 0906	char ssid[20]="andy";//·�����˺�He:init it with para
//by heyc 0906	char ssidPassword[20]="andyandy";//·�������� He:init it with para
char apssid[20]={"CORX_HD"};//ģ����˺�
char apssidPassword[20]={"1234567"};//ģ�������
//current currents;//ȡ��
//u08 REALYState;//ȡ��
//By HeYC 0822	u08 setSSid=0;
//By HeYC 0822	u08 apsetSSid=0;
u08 ClientID;   //�������ֻ���ģ�飬��ô���ID��ָ�����ֻ���ID,1���ֻ���2���ֻ���������
//word AnalogData[REAL_ANALOG_NUM]; //ȡ��
//word delayoffCounter;//ȡ��
u08 wifiATCmdSetStat=0;//WIFI_STATE_PRE
//u08  adStart;//ȡ��
u08 STAEnable;
u08 flagConnetedWithSvr=0;//By HeYC from 1 to 0
u32 timerOfReConnectSvr=0;//
u08 hartconnectflag=0;

u08 staInitMsg;
u08 flagBgnConnectSvr=0;//By HeYC, let it equal 0 at begin;
//by HeYC 0907	u08 ATCWSAPEnable=0;//By HeYC let it euqal 0 at begin
AIRCON  air;
IR_  IR;				    //RFָ��
ALARM_RF  alarm;				    //RF����ָ��
u8 read_IR315_trig;        //?����?IR?D????��???
u8 read_IR433_trig; 
u8 send_flag=0;
u32 TimeOut;
u32 gAppCfgSuccTmout;
u08 CodeBuf[IRCODELENGTH];
u08 CodeHead,CodeTail;
RFS rfs;
//�������״̬
//by heyc 0906	u08 SetOrUnsetState[1];
u08 curSecuMode;
//By HeYC 	u08 SetOrUnsetStateflag;

//By HeYC 	char rfInterLock=0;//rfInterLock=2ʱ433��Ч��315��������=1ʱ315��Ч��433������,=3ʱ��
IDLE_RF_RCV_STAT rfInterLock=IRRS_IDLE;//By HeYC;
                  //��ʾ315�б����ź�δ�����ڼ�315,433��������������б����������������=4ʱ��ʾ344�б����ź�δ����
//By heyc 0830	u08 alarmActive=0;//�����źŶ���ɹ�
u08 CNT;
u08 Authenticationflag=0;
u08 wifiMDInitflag=0;

u08 ClientCounterA=0;//He:it means how many times prog tried to connected with sever by tcp, if exceed max times, shoule reset wifi module.

CfgParaInFlash   gCfgPara;//By HeYC 0906

/* EOF */
