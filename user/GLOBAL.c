/*******************************************************************************
*
*   DTS AS 
*   Soft version:   
*   File Name:      GLOBAL.c
*   Author   :      zzw (zhangzw_3@163.com)
*   creation date:  2011-05-22
*   module description:定义系统中的全局变量 
*   Copyright (C) 
*
********************************************************************************/
#include "GLOBAL.h"

char OFTS_AS_VERSION[4]={'V','1','.','0'};//版本号
//////////////////UART/////////////////////////
//byte COMM1RecvBuf[COMM1_BUF_LEN];
app_com app[5];//创建端口函数数组

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


flash  WIFISBUF[WIFILENGTH];	////wifi缓冲区
flash  WIFITxMessage;	////WiFi发送数据
u08    WIFI_Head;//WiFi环形缓冲区头   当头和尾不相等的时候就有数据要发送，当头和尾指到同一个地址的时候即重合了，没有要发送的数据或数据发送完毕
u08    WIFI_Hail;//WiFi环形缓冲区尾   
//by HeYC 0906	u08    WIFIENBLE;     // //wifi使能
u08    WaitMONITOR_IDELSend; ////等待wifi发送数据完成标志位
u08    WifiBusy=0; //模块忙，不能回复数据,He: let it equal WIFI_IDEL(0) at beginning.
int    smsSMSStep;//WiFi信息发送步奏
//By HeYC 0906	char   macid[20]="1AFE34F3AFD4";//网卡mac地址
char   macidHEX[6]={0};//By HeYC let it equal init valal
//by heyc 0906	char   revmacid[20];//收到的mac序列号
//By HeYC 0906	char   staClientIP[20]="topansh.wicp.net";//STA模式远程服务器IP, He: init it with default para.
//By HeYC 0906	char   staClientPort[10]="7269";//STA式远程服务器端口号He: init it with default para.

char   UDPPort[10]={"59999"};
//By HeYC 0906	WORD macpin;//mac密码
//by heyc 0906	DWORD  macidlong;//mac转长整形
u08  waitsmssend; //收到等待'>'启动标志位，表示可以发送数据到WIFI
//by heyc 0906	u08  smssenddata; 
//by heyc 0906	u08 keystate;//按键状态

//RTCTIME tm;

//u08 ENBALE_SET;//取消
//by heyc 0906	u08 ENBALE_PROTECTION;//取消
//By HeYC 0906	u08 ENBALE_KEY;//取消
//char clock[128];
u08 eeprombuf[270];
u08 splflage[2]={0x55,0xaa};
//CLOCKTIME clcoks[10];
//char IRString[50];//红外序列
//u8 IRDatas[25];//红外与数据
//By HeYC 0906	u16 delayOffTime;//取消

//by heyc 0906	char ssid[20]="andy";//路由器账号He:init it with para
//by heyc 0906	char ssidPassword[20]="andyandy";//路由器密码 He:init it with para
char apssid[20]={"CORX_HD"};//模块的账号
char apssidPassword[20]={"1234567"};//模块的密码
//current currents;//取消
//u08 REALYState;//取消
//By HeYC 0822	u08 setSSid=0;
//By HeYC 0822	u08 apsetSSid=0;
u08 ClientID;   //例如用手机连模块，那么这个ID就指的是手机的ID,1号手机，2号手机。。。。
//word AnalogData[REAL_ANALOG_NUM]; //取消
//word delayoffCounter;//取消
u08 wifiATCmdSetStat=0;//WIFI_STATE_PRE
//u08  adStart;//取消
u08 STAEnable;
u08 flagConnetedWithSvr=0;//By HeYC from 1 to 0
u32 timerOfReConnectSvr=0;//
u08 hartconnectflag=0;

u08 staInitMsg;
u08 flagBgnConnectSvr=0;//By HeYC, let it equal 0 at begin;
//by HeYC 0907	u08 ATCWSAPEnable=0;//By HeYC let it euqal 0 at begin
AIRCON  air;
IR_  IR;				    //RF指令
ALARM_RF  alarm;				    //RF报警指令
u8 read_IR315_trig;        //?áè?IR?D????μ???
u8 read_IR433_trig; 
u8 send_flag=0;
u32 TimeOut;
u32 gAppCfgSuccTmout;
u08 CodeBuf[IRCODELENGTH];
u08 CodeHead,CodeTail;
RFS rfs;
//设防撤防状态
//by heyc 0906	u08 SetOrUnsetState[1];
u08 curSecuMode;
//By HeYC 	u08 SetOrUnsetStateflag;

//By HeYC 	char rfInterLock=0;//rfInterLock=2时433有效，315被锁定；=1时315有效，433被锁定,=3时，
IDLE_RF_RCV_STAT rfInterLock=IRRS_IDLE;//By HeYC;
                  //表示315有报警信号未处理，期间315,433都被锁定，需进行报警解码后解除锁定，=4时表示344有报警信号未处理。
//By heyc 0830	u08 alarmActive=0;//报警信号对码成功
u08 CNT;
u08 Authenticationflag=0;
u08 wifiMDInitflag=0;

u08 ClientCounterA=0;//He:it means how many times prog tried to connected with sever by tcp, if exceed max times, shoule reset wifi module.

CfgParaInFlash   gCfgPara;//By HeYC 0906

/* EOF */
